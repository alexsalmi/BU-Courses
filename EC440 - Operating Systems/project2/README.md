
## METHODOLOGY

To write my solution for this assignment, I more or less just followed the given instructions and the discussion powerpoint.

### INITIALIZE():
First, set up the alarm and signal to set off the schedule function. create a sigaction for the SIGALRM signal, setting the sigaction to schedule(), and use the SA_NODEFER flag, so that the alarm will not be blocked, and will keep going off in every thread. Then create the timer using setitimer, with interval values of 50ms. Then create a context for the main thread, without initializing a stack for it, and setting its status to ready. Then call setjmp to store the current context to its registers.

### PTHREAD_CREATE():
If this is the first time calling pthread_create(), call the initialize function. Find the first index in the threads array that is available, and create the new context at this index. Save its ID to the context and *thread. Allocate the memory to the given size, and place the address of pthread_exit() at the top of the stack. Use wr12() and wr13() to set the startroutine and its arguments, and then call setjmp, storing the jmp_buf in the thread's context. Then change the stack pointer to the top of the allocated stack (the address of pthread_exit), and change the program counter to the address of start_thunk. Then set the thread status to Ready.

### PTHREAD_EXIT():
Set the current thread's status to Exited. I was freeing the stack of the current thread in this function, but it was breaking on the last non-main thread, and TA Alex said that we do not need to free the stack for this assignment, so I removed this functionality. After updating the status, if there are still active threads remaining, call the schedule function, otherwise exit with value 0.

### SCHEDULE():
Call setjmp on the current threads registers. If this returns 0, we are returning from a longjmp, so we simply exit the function. If setjmp does not return 0, the current context was saved to the registers, and we need to switch threads. Incrementing up from the current thread, find the first thread that is in the Ready state, and set this thread as the current state. Set the status to running, and then call longjmp on its registers.

### PTHREAD_SELF():
Returns the ID of the current thread context.

## Complications
It took a while to figure out how to allocate the stack to the correct size. At first I was just trying to pass in 32,767 to malloc(), which wasn't working. I changed it to sizeof(int)*32,767/4, so that it is precisely 32,767 bytes.

I was also having issues with the timer. At first the SIGALRM handler would only be set off in the main thread, with all of the other threads simply running to completion before switching. I added the SA_NODEFER flag to the sigaction, and made sure to set up the timer before I setjmp the main thread in the initialze function, and this seemed to fix the issue.

I was also having issues with freeing the stacks in pthread_exit(). It would work correctly until there was only the main thread and one other thread remaining. When this last non-main thread would exit, it would try to free its stack, and cause a segmentation fault. I am not sure why this was happening, but I found out that freeing the stacks was not required for this assignment, so I simply removed this functionality. Instead, I free the stack in pthread_create() if a new thread is being created in a PCB that has already been used and exited by a different thread.