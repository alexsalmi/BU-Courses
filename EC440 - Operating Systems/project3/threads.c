#include <pthread.h>
#include <semaphore.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "ec440threads.h"

#define MAX_THREADS 128
#define STACK_SIZE 32767
#define QUANTUM 50

#define JB_RSP 6
#define JB_PC 7

#define TRUE 1
#define FALSE 0

struct TCB {
    pthread_t ID;
    jmp_buf registers;
    unsigned long int* stack;
    int status;
    int* joined;
    int numJoined;
    int waitingOn;
    int blockedBySem;
    void** exit_status;
};

int activeThreads = 0;
int availableIndex = 0;
int currentThread = 0;
pthread_t latestID = 1;
struct TCB threads[128];
enum status {Exited, Ready, Running, Blocked};

void schedule(){
    // If setjmp returns anything other than 0, we are returning from longjmp, so ignore the rest of the function
    if(setjmp(threads[currentThread].registers) == 0)
	{
        // Set the previously running thread to ready state
        if(threads[currentThread].status != Exited && threads[currentThread].status != Blocked){
            threads[currentThread].status = Ready;
        }
        
        // Find the next thread in the ready state by incrementing up through the threads array, wrapping around
        do{
            currentThread = (currentThread+1)%MAX_THREADS;
        }while(threads[currentThread].status != Ready && threads[currentThread].status != Running);

        // Set the new thread to running, and longjmp to its context
        threads[currentThread].status = Running;
        longjmp(threads[currentThread].registers, 1);
    }
}

void initialize(){
    // Initialize the signal handler to go to schedule(). SA_NODEFER is used to prevent blocking of the timer
    struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = schedule;
    sa.sa_flags = SA_NODEFER;
	sigaction(SIGALRM, &sa, NULL);

    // Initialize the timer with an interval time of 50ms (real time)
    struct itimerval timer;
    timer.it_value.tv_usec = QUANTUM*1000;
    timer.it_value.tv_sec = 0;
    timer.it_interval.tv_usec = QUANTUM*1000;
    timer.it_interval.tv_sec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    //Initialize main thread context
    threads[0].ID = 0;
    threads[0].stack = NULL;
    threads[0].status = Ready;
    threads[0].joined = malloc(sizeof(int)*MAX_THREADS);
    threads[0].numJoined = 0;
    threads[0].waitingOn = 0;
    threads[0].blockedBySem = FALSE;
    setjmp(threads[0].registers);

    activeThreads++;
}

void pthread_exit_wrapper()
{
    unsigned long int res;
    asm("movq %%rax, %0\n":"=r"(res));
    pthread_exit((void *) res);
}

void pthread_exit(void *value_ptr){
    // Set the finished thread's status to exited, and the exit status to 0
    threads[currentThread].status = Exited;
    
    // If the current thread is being waited on by another thread (through pthread_join), save the return value in exit_status
    if(threads[currentThread].exit_status != NULL){
        memcpy(threads[currentThread].exit_status, &value_ptr, sizeof(value_ptr));
    }

    // Loop through all threads that are waiting on the current thread to finish.
    int i;
    for(i=0; i<threads[currentThread].numJoined; i++){
        // If the thread is not waiting for any threads other than this one, and it's not waiting on a semaphore, set it to ready
        if(threads[threads[currentThread].joined[i]].waitingOn == 1 && threads[threads[currentThread].joined[i]].blockedBySem == FALSE){
            threads[threads[currentThread].joined[i]].status = Ready;
        }
        threads[threads[currentThread].joined[i]].waitingOn--;
    }

    // Free joining info in current thread's TCB
    threads[currentThread].numJoined = 0;
    free(threads[currentThread].joined);

    activeThreads--;

    // If there are threads remaining, call schedule(). Otherwise, exit with status 0
    if(activeThreads == 0){
        exit(0);
    }
    else{
        schedule();
    }
	__builtin_unreachable();
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg){
    // If this is the first time pthread_create is being called, initialize the subsystems
    if(activeThreads == 0){
        initialize();
    }
    // If we are at the thread cap, don't create a new thread
    if(activeThreads == MAX_THREADS){
        printf("ERROR: Too many active threads. Thread was not created\n");
        return -1;
    }

    // Find the next index in the threads array that is available 
    do{
        availableIndex = (availableIndex + 1) % MAX_THREADS;
    }while(threads[availableIndex].status != Exited && threads[availableIndex].status != Blocked);

    // If the TCB in the found index of the threads array has been used previously, free its stack
    if(threads[availableIndex].stack != NULL){
        free(threads[availableIndex].stack);
    }

    // Save the thread ID
    threads[availableIndex].ID = latestID;
    *thread = latestID;

    // Prepare joining values
    threads[availableIndex].joined = malloc(sizeof(int)*MAX_THREADS);
    threads[availableIndex].numJoined = 0;
    threads[availableIndex].waitingOn = 0;
    threads[availableIndex].blockedBySem = FALSE;
    
    // Allocate the thread's stack, and place adress of pthread_exit() at the top of the stack
    threads[availableIndex].stack = malloc(sizeof(unsigned long int)*(STACK_SIZE));
    threads[availableIndex].stack[STACK_SIZE - 1] = (unsigned long int) pthread_exit_wrapper;

    threads[availableIndex].exit_status = NULL;

    // Place start routine and args in the correct registers using given functions
    wr12((unsigned long int) start_routine);
    wr13((unsigned long int) arg);

    // Save current context to the thread's registers
    setjmp(threads[availableIndex].registers);

    // Change the thread's stack pointer to point to the location of the adress of pthread_exit(), so that pthread_exit() will be called when the startroutine returns
    threads[availableIndex].registers[0].__jmpbuf[JB_RSP] = ptr_mangle((unsigned long int) (threads[availableIndex].stack + STACK_SIZE - 1));
    // Change the program counter to the adress of start_thunk, so the thread starts correctly
    threads[availableIndex].registers[0].__jmpbuf[JB_PC] = ptr_mangle((unsigned long int) start_thunk);

    // Set the thread's status to ready
    threads[availableIndex].status = Ready;
    activeThreads++;
    latestID++;

    return 0;
}

int pthread_join(pthread_t thread, void **value_ptr){
    // If the requested thread has already finished, return
    if(threads[thread].status == Exited){
        return -1;
    }

    // Set calling thread to blocked, increment the number of threads it's waiting on
    threads[currentThread].status = Blocked;
    threads[currentThread].waitingOn++;

    // If input value_ptr is a valid address, set exit_status pointer of requested thread to value_ptr
    if(value_ptr != NULL){
        threads[thread].exit_status = value_ptr;
    }

    // Add the requesting thread to the list of threads the target thread is joined with, and increment how many it's joined with
    threads[thread].joined[threads[thread].numJoined] = currentThread;
    threads[thread].numJoined++;

    // Since current thread is now blocked, schedule
    schedule();

    return 0;
}

pthread_t pthread_self(void) {
    // Return the current thread's context
    return threads[currentThread].ID;
}

void lock(){
    sigset_t mask;

    // Block alarm signal
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, NULL);
}

void unlock(){
    sigset_t mask;

    // Unblock alarm signal
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

// SEMAPHORES //

struct semaphore{
    unsigned count;     // Semaphore value
    pthread_t* queue;   // Threads waiting on the semaphore to go above 0
    int nextIn;         // Position in queue for next thread that needs to wait to be inputted
    int nextOut;        // Position in queue for next thread that needs to stop waiting to be taken from
    int initialized;    // Is the semaphore initialized or not
};

int sem_init(sem_t *sem, int pshared, unsigned value){
    // Initialize the new semaphore
    struct semaphore* newSem = malloc(sizeof(*newSem));
    newSem->count = value;
    newSem->queue = malloc(sizeof(pthread_t)* MAX_THREADS);
    newSem->nextIn = 0;
    newSem->nextOut = 0;
    newSem->initialized = TRUE;

    // Save the custom semaphore in the supplied sem_t
    sem->__align = (long) newSem;

    return 0;
}

int sem_wait(sem_t *sem){
    struct semaphore* currSem = (struct semaphore*) sem->__align;

    // If the count is 0, block the calling thread and add it's ID to the semaphore's queue
    if(currSem->count == 0){
        threads[currentThread].status = Blocked;
        threads[currentThread].blockedBySem = TRUE;
        currSem->queue[currSem->nextIn] = threads[currentThread].ID;
        currSem->nextIn = (currSem->nextIn+1) % MAX_THREADS;

        // Schedule a new thread
        schedule();
    }
    
    // Decrement value of semaphore
    currSem->count--;
    return 0;    
}

int sem_post(sem_t *sem){
    struct semaphore* currSem = (struct semaphore*) sem->__align;

    // Increment value of semaphore
    currSem->count++;

    // If we are exiting a state of semaphore being 0, and there is a thread in the queue, set that thread to ready
    if(currSem->count == 1 && currSem->nextIn != currSem->nextOut){
        int index = currSem->queue[currSem->nextOut];
        threads[index].blockedBySem = FALSE;
        currSem->nextOut = (currSem->nextOut+1) % MAX_THREADS;

        // Make sure the thread is not joined to another before setting it to Ready
        if(threads[index].waitingOn == 0){
            threads[index].status = Ready;
        }
    }

    return 0;
}

int sem_destroy(sem_t *sem){
    struct semaphore* currSem = (struct semaphore*) sem->__align;

    // If the semaphore has been inititalized by sem_init, clear its variables
    if(currSem->initialized == TRUE){
        currSem->initialized = FALSE;
        free(currSem->queue);
        free(currSem);
    }
    else{
        return -1;
    }

    return 0;
}
