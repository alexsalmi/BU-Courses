#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "ec440threads.h"

#define MAX_THREADS 128
#define STACK_SIZE 32767
#define QUANTUM 50

#define JB_RSP 6
#define JB_PC 7

#define TRUE 1
#define FALSE 0

struct TCB {
    int ID;
    jmp_buf registers;
    unsigned int* stack;
    int status;
    int exit_status;
};

int activeThreads = 0;
int availableIndex = 0;
int currentThread = 0;
int latestID = 1;
struct TCB threads[128];
enum status {Exited, Ready, Running};

void schedule(){
    // If setjmp returns anything other than 0, we are returning from longjmp, so ignore the rest of the function
    if(setjmp(threads[currentThread].registers) == 0)
	{
        // Set the previously running thread to ready state
        if(threads[currentThread].status != Exited){
            threads[currentThread].status = Ready;
        }

        // Find the next thread in the ready state by incrementing up through the threads array, wrapping around
        do{
            currentThread = (currentThread+1)%MAX_THREADS;
        }while(threads[currentThread].status != Ready);

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
    setjmp(threads[0].registers);

    activeThreads++;
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
    }while(threads[availableIndex].status != Exited);

    // If the TCB in the found index of the threads array has been used previously, free its stack
    if(threads[availableIndex].stack != NULL){
        free(threads[availableIndex].stack);
    }

    // Save the thread ID
    threads[availableIndex].ID = latestID;
    *thread = latestID;
    
    // Allocate the thread's stack, and place adress of pthread_exit() at the top of the stack
    threads[availableIndex].stack = malloc(sizeof(int)*(STACK_SIZE/4));
    threads[availableIndex].stack[STACK_SIZE/4 - 1] = (unsigned long int) pthread_exit;

    // Place start routine and args in the correct registers using given functions
    wr12((unsigned long int) start_routine);
    wr13((unsigned long int) arg);

    // Save current context to the thread's registers
    setjmp(threads[availableIndex].registers);

    // Change the thread's stack pointer to point to the location of the adress of pthread_exit(), so that pthread_exit() will be called when the startroutine returns
    threads[availableIndex].registers[0].__jmpbuf[JB_RSP] = ptr_mangle((unsigned long int) (threads[availableIndex].stack + STACK_SIZE/4 - 1));
    // Change the program counter to the adress of start_thunk, so the thread starts correctly
    threads[availableIndex].registers[0].__jmpbuf[JB_PC] = ptr_mangle((unsigned long int) start_thunk);

    // Set the thread's status to ready
    threads[availableIndex].status = Ready;
    activeThreads++;
    latestID++;

    return 0;
}

void pthread_exit(void *value_ptr){
    // Set the finished thread's status to exited, and the exit status to 0
    threads[currentThread].status = Exited;
    threads[currentThread].exit_status = 0;

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

pthread_t pthread_self(void) {
    // Return the current thread's context
    return threads[currentThread].ID;
}
