#include <stdio.h>
#include <stdlib.h>
#include "fs.h"


int main(int argc, char **argv) {
    make_fs("new_fs");
    mount_fs("new_fs");

    fs_create("File1");

    // int fd[40];
    int i;

    int fd1 = fs_open("File1");

    // for(i=0; i<35; i++){
    //     fd[i] = fs_open("File1");
    //     if(fd[i] == -1){
    //         printf("%d: FAIL\n", i);
    //     }
    //     else{
    //         printf("%d: SUCC\n", i);
    //     }
    // }


    char* msg1 = calloc(1, 15000000);
    char* msg2 = calloc(1, 2000000);

    printf("Wrote %d bytes to file 1\n", fs_write(fd1,msg1,15000000));
    printf("Wrote %d bytes to file 1\n", fs_write(fd1,msg2,2000000));

    printf("Filesize 1: %d\n", fs_get_filesize(fd1));

    fs_lseek(fd1, 0);

    fs_close(fd1);
    
    umount_fs("new_fs");
    mount_fs("new_fs");

    fd1 = fs_open("File1");

    char* recieve1 = malloc(15000000 + 2000000);

    printf("Reading %d bytes from file 1\n", fs_read(fd1, recieve1, 15000000 + 2000000));

    printf("Message 1: %s\n", recieve1);

    char*** list = malloc(sizeof(char**));
    *list = calloc(64, sizeof(char*));

    fs_listfiles(list);

    i=0;
    printf("FILES:\n");
    while((*list)[i] != NULL){
        printf("%s\n", (*list)[i]);
        i++;
    }
    
    fs_close(fd1);
    fs_delete("File1");
    umount_fs("new_fs");


    return 0;
}
