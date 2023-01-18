#include "app_threading.h"

// Function for Thread #1
void *Thread1(void *threadid){
    while(1){
        printf("thread 1 : hello\n");
        sleep(3);
    }
}

// Function for Thread #2
void *Thread2(void *threadid){
    while(1){
        printf("thread 2 : hello\n");
        sleep(4);
    }
}

// Function to initialize and init thread control
int threadConfigure() {
        
    // Init thread list
    pthread_t threads[NUM_THREADS];
    int rc, i = 0;

    // Mapping thread #1 to map list
    printf("Creating thead %d, \n",i);
    if ( (rc = pthread_create(&threads[i], NULL, Thread1, NULL)) ){
        printf("Error:unable to create thread, %d\n", rc);
        return 0;
    }
    i++;

    // Mapping thread #1 to map list
    printf("Creating thead %d, \n",i);
    if ( (rc = pthread_create(&threads[i], NULL, Thread2, NULL)) ){
        printf("Error:unable to create thread, %d\n",rc);
        return 0;
    }
    i++;
 
    return 0;
}