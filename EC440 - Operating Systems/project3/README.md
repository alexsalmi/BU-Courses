
## METHODOLOGY

To write my solution for this assignment, I more or less just followed the given instructions and the lecture slides on semaphores. 

### PTHREAD_JOIN():
To implement phtread_join, I first added the joined, numJoined, and waitingOn variables to the TCB struct. joined keeps track of all of the threads that are waiting on this thread to finish. numJoined keeps track of the next available index for joined. waitingOn keeps track of how many threads this thread is waiting on to finish. When pthread_join is called, the calling thread's status is set to Blocked, and its waitingOn value is incremented. Then we connect the exit_status of the input thread with the inputted value_ptr. Then we add the calling thread's ID to the inputted thread's joined list. 
When the inputted thread eventually exits, it will check all of the threads in its joined list, setting them all back to Ready if they are not waiting on any other threads.

### LOCK()/UNLOCK():
These functions were very simple to implement. All I had to do was use sigprocmask to block/unblock the SIGALRM signal from being recieved by the process.

### SEMAPHORES:
To implement the semaphore functionality, I created a semaphore structure, containing the current count of the semaphore, the queue of threads waiting on the semaphore to go above 0, the addresses of this queue where threads should be inputted/outputted, and an initialized flag. 

### SEM_INIT():
This function simply initializes a new instance of my semaphore struct, sets the count to the inputted value, allocates the queue, and sets the initialized flag to true. It then connects this custom semaphore to the inputted sem_t using the sem_t's __align value.

### SEM_WAIT():
If the semaphore's count is 0 when called, the calling thread is set to Blocked, and the thread's ID is added to the semaphore's queue. We then schedule a new thread before decrementing the semaphore's count, so that the decrementing will not happen again until the calling thread is set to Ready and scheduled again.

### SEM_POST():
Increments the semaphore's count. If the count is now 1, and there are threads in the queue, set the first thread in the queue to Ready, so that it can now be scheduled again.

### SEM_DESTROY():
If the inputted semaphore has not previously been initialized, return -1. If it has been initialized, simply free it's queue, and then free the custom semaphore itself. 

## Complications
When I first implemented pthread_join, the functionality of my other pthread methods stopped working correctly. Specifically, it started having trouble scheduling multiple threads, and launching different stat routines. I discovered that this was because I was not scheduling a new thread after blocking the current one in pthread_join. 

It took a little bit to figure out how masks and sigprocmask works correctly, but once I did, lock() and unlock() were fairly simple to implement.

When implementing the semaphores at first, I simply had a while(count == 0){} loop in sem_wait to have the threads wait on the semaphore to clear. I realized that this would be a waste of CPU over time, so I changed it to simply setting the calling thread to Blocked, and then set it back to Ready in sem_post when the semaphore goes back to >0. 
