#include <sys/mman.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0


// TLS structure
typedef struct thread_local_storage{
    pthread_t tid;
    unsigned int size;
    unsigned int page_num;
    struct page **pages;
} TLS;

// Page structure
struct page{
    unsigned long int address;
    int ref_count;
};

// Linked list node
struct list_element{
    pthread_t tid;
    TLS *tls;
    struct list_element *next;
    struct list_element *prev;
};

_Bool initialized = FALSE;
unsigned int page_size;

struct list_element* list_head;


// Signal handler
void tls_handle_page_fault(int sig, siginfo_t *si, void *context){
    unsigned long int p_fault = ((unsigned long int) si->si_addr) & ~(page_size - 1);

    struct list_element* curr_elem = list_head;

    int i;
    // Search for page that caused signal, exit it if found
    while(curr_elem->next != NULL){
        curr_elem = curr_elem->next;
        for(i=0; i<curr_elem->tls->page_num; i++){
            if(curr_elem->tls->pages[i]->address == p_fault){
                pthread_exit(NULL);
                return;
            }
        }
    }

    signal(SIGSEGV, SIG_DFL);
    signal(SIGBUS, SIG_DFL);
    raise(sig);
}


// Initializing function
void tls_init(){
    struct sigaction sa;
    /* get the size of a page */
    page_size = getpagesize();

    /* install the signal handler for page faults (SIGSEGV, SIGBUS) */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO; /* use extended signal handling */
    sa.sa_sigaction = tls_handle_page_fault;
    sigaction(SIGBUS, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);

    // Initialize dummy head of linked list
    list_head = malloc(sizeof(struct list_element));
    list_head->tid = -1;
    list_head->tls = NULL;
    list_head->next = NULL;
    list_head->prev = NULL;

    initialized = TRUE;
}


// Protect a page
void tls_protect(struct page *p){
    if(mprotect((void *) p->address, page_size, 0)){
        fprintf(stderr, "tls_protect: could not protect page\n");
        exit(1);
    }
}


// Unprotect a page
void tls_unprotect(struct page *p){
    if(mprotect((void *) p->address, page_size, PROT_READ | PROT_WRITE)){
        fprintf(stderr, "tls_unprotect: could not unprotect page\n");
        exit(1);
    }
}


// Create a new TLS
int tls_create(unsigned int size){
    if(initialized == FALSE)
        tls_init();

    TLS* tls = NULL;

    // Search for the current thread's TLS
    struct list_element* curr_elem = list_head;
    while(curr_elem->next != NULL){
        curr_elem = curr_elem->next;
        if(curr_elem->tid == pthread_self()){
            // If the current thread already has a TLS with size > 0, return -1
            if(curr_elem->tls != NULL && curr_elem->tls->size > 0)
                return -1;
            else{
                // Otherwise set tls to the current thread's TLS and break out of loop
                tls = curr_elem->tls;
                break;
            }
        }
    }

    // If the current thread didn't already have a list node, initialize a new one
    if(tls == NULL){
        curr_elem->next = malloc(sizeof(struct list_element));
        curr_elem->next->prev = curr_elem;

        curr_elem = curr_elem->next;
        curr_elem->next = NULL;
        curr_elem->tid = pthread_self();
    }

    // Initialize new TLS and assign it to current thread's list node
    tls = calloc(1, sizeof(TLS));
    curr_elem->tls = tls;
    tls->tid = pthread_self();
    tls->size = size;
    tls->page_num = size/page_size;
    if(size%page_size > 0)
        tls->page_num++;
    tls->pages = calloc(tls->page_num, sizeof(struct page*));

    // Initialize new pages for the current thread's TLS
    int i;
    for(i=0; i<tls->page_num; i++){
        struct page* p = (struct page *) calloc(1, sizeof(struct page));
        p->address = (unsigned long int)mmap(0, page_size, 0, MAP_ANON | MAP_PRIVATE, 0, 0);
        p->ref_count = 1;
        tls->pages[i] = p;
    }

    return 0;
}


// Write to the current thread's TLS
int tls_write(unsigned int offset, unsigned int length, char *buffer){
    if(initialized == FALSE)
        return -1;

    struct list_element* curr_elem = list_head;
    TLS* tls = NULL;

    // Search for the current thread's TLS
    while(curr_elem->next != NULL){
        curr_elem = curr_elem->next;
        if(curr_elem->tid == pthread_self()){
            tls = curr_elem->tls;
            break;
        }
    }

    // If the TLS was not found, or the specified offset + length is too large, return 0
    if(tls == NULL)
        return -1;
    if((offset + length) > tls->size)
        return -1;

    // Unprotect all of the TLS's pages
    int i;
    for(i=0; i<tls->page_num; i++)
        tls_unprotect(tls->pages[i]);

    // Perform write operation (provided by course staff)
    unsigned int cnt, idx;
    for(cnt = 0, idx = offset; idx < (offset + length); ++cnt, ++idx){
        struct page *p;
        unsigned int pn, poff;
        pn = idx / page_size;
        poff = idx % page_size;
        p = tls->pages[pn];

        // COW implementation
        if(p->ref_count > 1){
            struct page* copy = (struct page *) calloc(1, sizeof(struct page));
            copy->address = (unsigned long int) mmap(0, page_size, PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
            copy->ref_count = 1;
            tls->pages[pn] = copy;

            // Copy memory from shared page to new page
            memcpy((char*)copy->address, (char*)p->address, page_size); 

            p->ref_count--;
            tls_protect(p);
            p = copy;
        }

        char* dst = ((char *) p->address) + poff;
        *dst = buffer[cnt];
    }

    // Re protect all of the TLS's pages
    for(i=0; i<tls->page_num; i++)
        tls_protect(tls->pages[i]);

    return 0;
}


// Read data from the current thread's TLS
int tls_read(unsigned int offset, unsigned int length, char *buffer){
    if(initialized == FALSE)
        return -1;

    struct list_element* curr_elem = list_head;
    TLS* tls = NULL;

    // Search for the current thread's TLS
    while(curr_elem->next != NULL){
        curr_elem = curr_elem->next;
        if(curr_elem->tid == pthread_self()){
            tls = curr_elem->tls;
            break;
        }
    }

    // If the TLS was not found, or the specified offset + length is too large, return 0
    if(tls == NULL)
        return -1;
    if((offset + length) > tls->size)
        return -1;

    // Uprotect all of the TLS's pages
    int i;
    for(i=0; i<tls->page_num; i++)
        tls_unprotect(tls->pages[i]);

    // Perform read operation (provided by course staff)
    unsigned int cnt, idx;
    for(cnt = 0, idx = offset; idx < (offset + length); ++cnt, ++idx){
        struct page *p;
        unsigned int pn, poff;

        pn = idx / page_size;
        poff = idx % page_size;

        p = tls->pages[pn];
        char* src = ((char*) p->address) + poff;

        buffer[cnt] = *src;
    }

    // Reprotect all of the TLS's pages
    for(i=0; i<tls->page_num; i++)
        tls_protect(tls->pages[i]);
    
    return 0;
}


// Destroy the current thread's TLS
int tls_destroy(){
    if(initialized == FALSE)
        return -1;

    struct list_element* curr_elem = list_head;
    TLS* tls = NULL;

    // Search for the current thread's TLS
    while(curr_elem->next != NULL){
        curr_elem = curr_elem->next;
        if(curr_elem->tid == pthread_self()){
            tls = curr_elem->tls;
            break;
        }
    }

    // If the current thread's TLS was not found, return -1
    if(tls == NULL)
        return -1;

    // Free() all of the TLS's pages, if they are only referenced by this TLS
    int i;
    for(i=0; i<tls->page_num; i++){
        if(tls->pages[i]->ref_count == 1)
            free(tls->pages[i]);
        else
            tls->pages[i]->ref_count--;
    }

    // Free the TLS's page array, the TLS itself, and finally the list element containing the TLS
    free(tls->pages);
    free(tls);
    curr_elem->prev->next = curr_elem->next;
    if(curr_elem->next != NULL)
        curr_elem->next->prev = curr_elem->prev;
    free(curr_elem);

    return 0;
}


// Create a new TLS for the currently running thread by cloning thread tid's TLS
int tls_clone(pthread_t tid){
    if(initialized == FALSE)
        return -1;

    struct list_element* curr_elem = list_head;

    // Searc for thread tid's TLS
    TLS* targetTLS = NULL;
    while(curr_elem->next != NULL){
        curr_elem = curr_elem->next;
        if(curr_elem->tid == tid){
            targetTLS = curr_elem->tls;
            break;
        }
    }

    // If thread tis does not have a TLS, return 0
    if(targetTLS == NULL)
        return -1;

    // Search for the current thread's TLS
    TLS* newTLS = NULL;
    curr_elem = list_head;
    while(curr_elem->next != NULL){
        curr_elem = curr_elem->next;
        if(curr_elem->tid == pthread_self()){
            // If the current thread already has a TLS with size > 0, return -1
            if(curr_elem->tls != NULL && curr_elem->tls->size > 0)
                return -1;
            else{
                // Otherwise, save the current thread's empty TLS to newTLS
                newTLS = curr_elem->tls;
                break;
            }
        }
    }

    // Initialize the list node for the current thread
    if(newTLS == NULL){
        curr_elem->next = malloc(sizeof(struct list_element));
        curr_elem->next->prev = curr_elem;

        curr_elem = curr_elem->next;
        curr_elem->next = NULL;
        curr_elem->tid = pthread_self();
    }

    // Initialize the new TLS for the current thread, and save it to the newly created list node
    newTLS = calloc(1, sizeof(TLS));
    newTLS->tid = pthread_self();
    newTLS->size = targetTLS->size;
    newTLS->page_num = targetTLS->page_num;
    curr_elem->tls = newTLS;

    // Initialize a new array of pages, but not new pages
    newTLS->pages = calloc(newTLS->page_num, sizeof(struct page*));

    // Copy pages from thread tid's TLS, and increment their ref_counts
    int i;
    for(i=0; i<newTLS->page_num; i++){
        newTLS->pages[i] = targetTLS->pages[i];
        targetTLS->pages[i]->ref_count += 1;
    }

    return 0;    
}
