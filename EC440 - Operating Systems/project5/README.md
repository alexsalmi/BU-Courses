## METHODOLOGY

To write my solution for this assignment, I decided to go with the recommended FAT system. This system consisted of four main global struct to keep track of all the file data/metadata:

- The super block hold the indexes and lengths of the FAT, DIR, and data when on disk. This is only used for mounting/umounting a disk
- The FAT contains metadata on what each block is assigned to. A FAT entry, each corresponding to its own block on disk, can have any of four values. If it is >=0, it is pointing to the next block that its file is occupying. If it is -1, this block is the last block used by its file, the EOF. If it is -2, the block is not being used by a file, and is free to be used by a file. If it is -3, it is a block being used by one of the metadata objects (super block, FAT, or DIR)
- The DIR contains all of the metadata about each file that is saved on our disk. A DIR entry, each corresponding to its own file, contains a flag to mark if the entry is currently being used for a file, the name of the file, the size of the file, the first block being used by the file, and the reference count, which tells us how many file descriptors are currently open for this file.
- The file descriptor array is the only one of these metadata structs that is not saved to the disk. This is because there is no reason to save active file descriptors when the disk is unmounted, as file descriptors are only used by a single process. A fildes entry, each corresponding to its own file descriptor (but multiple entries can be an fd for the same files), contains a flag to mark if the entry is currently being used for a fd, the first block being used by the file, and the offset, which is basically which byte in the file the fd is currently pointing to. 

Once I fully grasped what all of these metadata structs were used for, it was fairly simple to implement all of the required functionality, other than a couple complications.

## Complications
When I first started, this project seemed fairly daunting, because I didn't fully understand how the FAT system worked. Therefore, before getting too far into implementing the functionality, I reviewed past lectures and discussion slides to fully comprehend how the system worked, and how I would implement everything. 

Once I started, I was slightly confused about how I was meant to read and write from/to the disk. Initially, while I was working on create_fs(), I was simply passing the pointers to the different metadata objects into block_write(). Then I realized that this would not be persistent, as the data at the addresses in these pointers could change between processes. So then I changed the function to iterate through each metadata object and write all of its data to the disk.

Finally, my main complication with this project was reading and writing with a file on my disk. At first I didn't realize that we could read/write only part of a block, so I was only dealing with full blocks, which obviously became an issue. It then took me a while to figure out how to only read/write part of a block, but once I thought about it and planned it out on paper, the implementation became fairly straight forward.

