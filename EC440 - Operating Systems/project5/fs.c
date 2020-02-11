#include "disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

// Size constraints
#define MAX_F_NAME 15
#define MAX_FILES 64
#define MAX_FILDES 32
#define MAX_FILESIZE 4096*4096

// Enums for FAT
#define EoF -1
#define FREE -2
#define RESERVED -3

// Booleans
#define TRUE 1
#define FALSE 0

// Super block to keep track of metadata on disk
struct super_block{
    int fat_idx;  // First block of the FAT
    int fat_len;  // Length of FAT in blocks
    int dir_idx;  // First block of directory
    int dir_len;  // Length of directory in blocks
    int data_idx; // First block of file-data
};

// Directory entry - 1 per file
struct dir_entry{
    int used; // Is this file-slot in use
    char name[MAX_F_NAME + 1];
    int size; // file size
    int head; // first data block of file
    int ref_cnt;
    // how many open file descriptors are there?
    // ref_cnt > 0 -> cannot delete file
};

// File descriptor - any amount per file (max 32)
struct file_descriptor{
    int used;   // fd in use
    int file;   // the first block of the file
                // (f) to which fd refers to
    int offset; // position on fd within f
};

struct super_block fs; // will be populated with metadata locations in block
struct file_descriptor fildes[MAX_FILDES]; // will be populated with active file descriptors
int *FAT; // will be populated with the FAT data
struct dir_entry *DIR; // will be populated with the directory data

int active_fildes; // keeps track of how many active file descriptors there are
int num_files; // keeps track of how many files are on the disk

int make_fs(char *disk_name){
    if(make_disk(disk_name) < 0){
        printf("ERROR: make_fs: Could not create disk\n");
        return -1;
    }
    if(open_disk(disk_name) < 0){
        printf("ERROR: make_fs: Could not open newly created disk\n");
        return -1;
    }

    // Initializing the super block for new disk
    struct super_block new_fs;
    new_fs.fat_idx = 1;
    new_fs.fat_len = (sizeof(int)*DISK_BLOCKS) / BLOCK_SIZE;
    new_fs.dir_idx = new_fs.fat_idx + new_fs.fat_len;
    new_fs.dir_len = (sizeof(struct dir_entry)*MAX_FILES) / BLOCK_SIZE + 1;
    new_fs.data_idx = new_fs.dir_idx + new_fs.dir_len;

    // write super block to disk
    char* buff = malloc(BLOCK_SIZE);
    memcpy(buff, &new_fs, sizeof(struct super_block));
    block_write(0, buff);
    
    // initialize the FAT for new disk
    free(buff);
    buff = malloc(BLOCK_SIZE);
    int fat_init_free = FREE;
    int fat_init_reserved = RESERVED;
    int i, j;
    for(i=0; i<new_fs.fat_len; i++){
        for(j=0; j<BLOCK_SIZE/sizeof(int); j++){
            // set blocks with metadata to RESERVED, all else to FREE
            if((i*BLOCK_SIZE/sizeof(int) + j) < new_fs.data_idx)
                memcpy(buff+(j*sizeof(int)), &fat_init_reserved, sizeof(int));
            else
                memcpy(buff+(j*sizeof(int)), &fat_init_free, sizeof(int));
        }
        // write FAT to disk
        block_write(new_fs.fat_idx+i, buff);
    }

    // Initialize DIR for new disk
    free(buff);
    buff = malloc(BLOCK_SIZE);
    struct dir_entry dir_init;
    dir_init.used = FALSE;
    for(i=0; i<new_fs.dir_len; i++){
        for(j=0; j<MAX_FILES; j++){
            memcpy(buff+(j*sizeof(struct dir_entry)), &dir_init, sizeof(struct dir_entry));
        }
        // write DIR to disk
        block_write(new_fs.dir_idx+i, buff);
    }
    free(buff);

    if(close_disk() < 0){
        printf("ERROR: make_fs: Could not close newly created disk\n");
        return -1;
    }

    return 0;
}

int mount_fs(char *disk_name){
    if(open_disk(disk_name) < 0){
        printf("ERROR: mount_fs: Could not open disk\n");
        return -1;
    }

    // read super block from disk and assign to global variable
    char* buff = malloc(BLOCK_SIZE);
    block_read(0, buff);
    memcpy(&fs, buff, sizeof(struct super_block));

    // read FAT from disk and assign to global variable
    free(buff);
    buff = malloc(BLOCK_SIZE);
    FAT = calloc(DISK_BLOCKS, sizeof(int));
    int i,j;
    for(i=0; i<fs.fat_len; i++){
        block_read(fs.fat_idx+i, buff);
        for(j=0; j<BLOCK_SIZE/sizeof(int); j++){
            memcpy(&FAT[i*BLOCK_SIZE/sizeof(int) + j], buff+(j*sizeof(int)), sizeof(int));
        }
    }

    // read DIR from disk and assign to global variable
    free(buff);
    buff = malloc(BLOCK_SIZE);
    DIR = calloc(MAX_FILES, sizeof(struct dir_entry));
    for(i=0; i<fs.dir_len; i++){
        block_read(fs.dir_idx+i, buff);
        for(j=0; j<MAX_FILES; j++){
            memcpy(&DIR[j], buff+(j*sizeof(struct dir_entry)), sizeof(struct dir_entry));
        }
    }
    free(buff);

    // Initialize file descriptors as unused
    for(i=0; i<MAX_FILDES; i++){
        fildes[i].used = FALSE;
    }

    // Initialize global counters to 0
    active_fildes = 0;
    num_files = 0;

    return 0;
}

int umount_fs(char *disk_name){
    // write active super block to disk
    char* buff = malloc(BLOCK_SIZE);
    memcpy(buff, &fs, sizeof(struct super_block));
    block_write(0, buff);
    
    // write active FAT to disk
    free(buff);
    buff = malloc(BLOCK_SIZE);
    int i, j;
    for(i=0; i<fs.fat_len; i++){
        for(j=0; j<BLOCK_SIZE/sizeof(int); j++){
            memcpy(buff+(j*sizeof(int)), &FAT[i*BLOCK_SIZE/sizeof(int) + j], sizeof(int));
        }
        block_write(fs.fat_idx+i, buff);
    }

    // write active DIR to disk
    free(buff);
    buff = malloc(BLOCK_SIZE);
    for(i=0; i<fs.dir_len; i++){
        for(j=0; j<MAX_FILES; j++){
            memcpy(buff+(j*sizeof(struct dir_entry)), &DIR[j], sizeof(struct dir_entry));
        }
        block_write(fs.dir_idx+i, buff);
    }
    free(buff);
    free(DIR);
    free(FAT);

    if(close_disk() < 0){
        printf("ERROR: unmount_fs: Could not close disk\n");
        return -1;
    }

    return 0;
}

int fs_open(char *name){
    if(active_fildes >= MAX_FILDES){
        printf("ERROR: fs_open: Max number of file descriptors in use\n");
        return -1;
    }

    // Find active file with given filename
    int i;
    int dirIdx = -1;
    for(i=0; i<MAX_FILES; i++){
        if(DIR[i].used == TRUE && strcmp(name, DIR[i].name) == 0){
            dirIdx = i;
            break;
        }
    }
    if(dirIdx == -1){
        printf("ERROR: fs_open: File with given name not found\n");
        return -1;
    }

    // Create new file descriptor for this file, and increment reference count for corresponding file
    for(i=0; i<MAX_FILDES; i++){
        if(fildes[i].used == FALSE){
            fildes[i].used = TRUE;
            fildes[i].file = DIR[dirIdx].head;
            fildes[i].offset = 0;

            DIR[dirIdx].ref_cnt++;

            active_fildes++;
            return i;
        }
    }

    return -1;
}

int fs_close(int fd){
    if(fd < 0 || fd >= MAX_FILDES || fildes[fd].used == FALSE){
        printf("ERROR: fs_close: Invalid file descriptor\n");
        return -1;
    }

    // Set file descriptor as unused
    fildes[fd].used = FALSE;

    // decrease reference count for corresponding file
    int i;
    for(i=0; i<MAX_FILES; i++){
        if(DIR[i].used == TRUE && DIR[i].head == fildes[fd].file){
            DIR[i].ref_cnt--;
            break;
        }
    }

    // decrease number of active file descriptors
    active_fildes--;

    return 0;
}

int fs_create(char *name){
    if(num_files >= MAX_FILES){
        printf("ERROR: fs_create: Too many open files\n");
        return -1;
    }
    if(strlen(name) > MAX_F_NAME){
        printf("ERROR: fs_create: File name too long\n");
        return -1;
    }

    // search for existing file with given name, or find first unused spot in DIR
    int i;
    int dirIdx = MAX_FILES+1;
    for(i=0; i<MAX_FILES; i++){
        if(DIR[i].used == TRUE && strcmp(DIR[i].name, name) == 0){
            printf("ERROR: fs_create: File with given name already exists\n");
            return -1;
        }
        if(DIR[i].used == FALSE && dirIdx > i){
            dirIdx = i;
        }
    }

    // Initialize DIR entry for new file
    DIR[dirIdx].used = TRUE;
    strcpy(DIR[dirIdx].name, name);
    DIR[dirIdx].size = 0;
    DIR[dirIdx].ref_cnt = 0;

    // Find first empty block, assign it to this new file
    for(i=0; i<DISK_BLOCKS; i++){
        if(FAT[i] == FREE){
            DIR[dirIdx].head = i;
            FAT[i] = EoF;
            break;
        }
    }

    // Increment number of files on disk
    num_files++;

    return 0;
}

int fs_delete(char *name){
    if(strlen(name) > MAX_F_NAME){
        printf("ERROR: fs_delete: Invalid file name\n");
        return -1;
    }

    // Find file with given file name
    int i;
    int dirIdx = -1;
    for(i=0; i<MAX_FILES; i++){
        if(DIR[i].used == TRUE && strcmp(DIR[i].name, name) == 0){
            dirIdx = i;
            break;
        }
    }
    if(dirIdx == -1){
        printf("ERROR: fs_delete: File not found\n");
        return -1;
    }
    if(DIR[dirIdx].ref_cnt > 0){
        printf("ERROR: fs_delete: Cannot delete open file\n");
        return -1;
    }

    // Set DIR entry to unused
    DIR[dirIdx].used = FALSE;

    // Free all blocks assigned to this file
    int block = DIR[dirIdx].head;
    int newBlock;
    while(block != EoF){
        newBlock = FAT[block];
        FAT[block] = FREE;
        block = newBlock;
    }
    FAT[block] = FREE;

    // Decrement number of files on disk
    num_files--;

    return 0;
}

int fs_read(int fd, void *buf, size_t nbyte){
    if(fd < 0 || fd >= MAX_FILDES || fildes[fd].used == FALSE){
        printf("ERROR: fs_read: Invalid file descriptor\n");
        return -1;
    }

    // Find size of the file
    int i;
    int size;
    for(i=0; i<MAX_FILES; i++)
        if(DIR[i].used == TRUE && DIR[i].head == fildes[fd].file)
            size = DIR[i].size;

    // if foffset + nbyte is greater than the file size, only read to the end of the file       
    int offset = fildes[fd].offset;
    int bytes_left = nbyte;
    if(offset+bytes_left > size)
        bytes_left = size-offset;
    int actual_bytes = bytes_left;
    if(actual_bytes == 0)
        return 0;
    
    // go to block where the offset is currently located
    int block = fildes[fd].file;
    for(i=0; i<offset/BLOCK_SIZE; i++){
        block = FAT[block];
    }
    offset = offset%BLOCK_SIZE;

    // read data from blocks
    char* buff = malloc(BLOCK_SIZE);
    i=0;
    while(bytes_left>BLOCK_SIZE){
        block_read(block, buff);
        memcpy(buf+i, buff+offset, BLOCK_SIZE-offset);
        bytes_left = bytes_left - (BLOCK_SIZE+offset);
        i = i + BLOCK_SIZE - offset;
        block = FAT[block];
        offset = 0;
    }

    // read partial data from last block
    block_read(block, buff);
    memcpy(buf+i, buff+offset, bytes_left);

    // increment offset to end of where we read
    fildes[fd].offset = fildes[fd].offset+actual_bytes;
    
    free(buff);

    return actual_bytes;
}

int fs_write(int fd, void *buf, size_t nbyte){
    if(fd < 0 || fd >= MAX_FILDES || fildes[fd].used == FALSE){
        printf("ERROR: fs_write: Invalid file descriptor\n");
        return -1;
    }

    // find DIR entry of file we are writing to
    int i;
    int dirIdx;
    for(i=0; i<MAX_FILES; i++){
        if(DIR[i].used == TRUE && DIR[i].head == fildes[fd].file){
            dirIdx = i;
            break;
        }
    }

    // go to block where offset is currently located
    int block = fildes[fd].file;
    int prevBlock = block;
    int offset = fildes[fd].offset;
    int bytes_left = nbyte;
    if(offset + bytes_left > 4096*BLOCK_SIZE)
        bytes_left = 4096*BLOCK_SIZE - offset;
    int actual_bytes = bytes_left;
    if(actual_bytes == 0)
        return 0;

    for(i=0; i<offset/BLOCK_SIZE; i++){
        prevBlock = block;
        block = FAT[block];
    }
    offset = offset%BLOCK_SIZE;

    // if the last block is full, find next available block to extend to before starting write operation
    if(block == EoF && offset == 0){
            int j;
            for(j=0; j<DISK_BLOCKS; j++){
                if(FAT[j] == FREE){
                    FAT[prevBlock] = j;
                    block = FAT[prevBlock];
                    FAT[j] = EoF;
                    break;
                }
            }
            // if there are no more available blocks, cap the write operation here (0 bytes written)
            if(block == EoF)
                return 0;
    }
    
    // write to blocks
    char* buff = malloc(BLOCK_SIZE);
    i=0;
    while(bytes_left>BLOCK_SIZE){
        block_read(block, buff);
        memcpy(buff+offset, buf+i, BLOCK_SIZE-offset);
        block_write(block, buff);
        bytes_left = bytes_left - (BLOCK_SIZE+offset);
        i = i + BLOCK_SIZE - offset;
        // if the current block is EOF, find next available block to extend to
        if(FAT[block] == EoF){
            int j;
            for(j=0; j<DISK_BLOCKS; j++){
                if(FAT[j] == FREE){
                    FAT[block] = j;
                    FAT[j] = EoF;
                    break;
                }
            }
            // if there are no more available blocks, cap the write operation here
            if(FAT[block] == EoF){
                DIR[dirIdx].size = fildes[fd].offset + actual_bytes - bytes_left;
                fildes[fd].offset = DIR[dirIdx].size;

                return actual_bytes-bytes_left;
            }
        }
        block = FAT[block];
        offset = 0;
    }

    // write remaining data to last block
    block_read(block, buff);
    memcpy(buff+offset, buf+i, (actual_bytes-i));
    block_write(block, buff);

    // update size of file and offset 
    if(fildes[fd].offset + actual_bytes > DIR[dirIdx].size)
        DIR[dirIdx].size = fildes[fd].offset + actual_bytes;
    fildes[fd].offset = fildes[fd].offset + actual_bytes;

    free(buff);

    return actual_bytes;
}

int fs_get_filesize(int fd){
    if(fd < 0 || fd >= MAX_FILDES || fildes[fd].used == FALSE){
        printf("ERROR: fs_get_filesize: Invalid file descriptor\n");
        return -1;
    }

    // find corresponding file, return its size
    int i;
    for(i=0; i<MAX_FILES; i++)
        if(DIR[i].used == TRUE && DIR[i].head == fildes[fd].file)
            return DIR[i].size;

    return -1;
}

int fs_listfiles(char ***files){
    int files_found = 0;
    int i;

    // find all DIR entries that are currently used and copy their filenames to list
    char** temp_list = calloc(MAX_FILES, sizeof(char*));
    for(i=0; i<MAX_FILES; i++){
        if(DIR[i].used == TRUE){
            temp_list[files_found] = DIR[i].name;
            files_found++;
        }
    }

    // cut off end of list with NULL pointer and copy list to input pointer
    temp_list[files_found] = NULL;
    memcpy(files, &temp_list, sizeof(char**));

    return 0;
} 

int fs_lseek(int fd, off_t offset){
    if(fd < 0 || fd >= MAX_FILDES || fildes[fd].used == FALSE){
        printf("ERROR: fs_get_filesize: Invalid file descriptor\n");
        return -1;
    }

    // find size of corresponding file to error check given offset
    int i;
    int size = -1;
    for(i=0; i<MAX_FILES; i++){
        if(DIR[i].used == TRUE && DIR[i].head == fildes[fd].file){
            size = DIR[i].size;
            break;
        }
    }
    if(offset < 0 || offset > size){
        printf("ERROR: fs_get_filesize: Invalid offset\n");
        return -1;
    }

    // change offset to requested value
    fildes[fd].offset = offset;

    return 0;
}

int fs_truncate(int fd, off_t length){
    if(fd < 0 || fd >= MAX_FILDES || fildes[fd].used == FALSE){
        printf("ERROR: fs_get_filesize: Invalid file descriptor\n");
        return -1;
    }

    // find size of corresponding file to error check new length
    int i;
    int size = -1;
    for(i=0; i<MAX_FILES; i++){
        if(DIR[i].used == TRUE && DIR[i].head == fildes[fd].file){
            size = DIR[i].size;
            break;
        }
    }

    if(length < 0 || length > size){
        printf("ERROR: fs_truncate: Invalid offset\n");
        return -1;
    }

    // go to block where new length would be
    int block = fildes[fd].file;
    for(i=0; i<length/BLOCK_SIZE; i++){
        block = FAT[block];
    }
    int newEOF = block;

    // set all remaining blocks to FREE
    while(FAT[block] != EoF){
        int temp = block;
        block = FAT[block];
        FAT[temp] = FREE;
    }

    // set old EOF to FREE, and assign new EOF to the block where 'length' is, and update size
    FAT[block] = FREE;
    FAT[newEOF] = EoF;
    DIR[i].size = length;

    return 0;
}
