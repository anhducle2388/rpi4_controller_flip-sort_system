#include "app_threading.h"

// Function for Thread #1
void *Thread1(void *threadid){
    while(1){
        logTsMsg(LOG_MSG, LOGPATH_OPERATION, "Execute thread #1.");
        sleep(3);
    }
}

// Function for Thread #2
void *Thread2(void *threadid){
    while(1){
        logTsMsg(LOG_MSG, LOGPATH_OPERATION, "Execute thread #2.");
        sleep(4);
    }
}

// Function to initialize and init thread control
int threadConfigure() {
        
    // Init thread list
    pthread_t threads[NUM_THREADS];
    int rc;

    // Mapping thread #1 to map list
    logTsMsg(LOG_MSG, LOGPATH_OPERATION, "Creating thread #1.");
    if ( (rc = pthread_create(&threads[0], NULL, Thread1, NULL)) ){
        logTsMsg(ERR_MSG, LOGPATH_OPERATION, "Error:unable to create thread.");
        return 0;
    }

    // Mapping thread #2 to map list
    logTsMsg(LOG_MSG, LOGPATH_OPERATION, "Creating thread #2");
    if ( (rc = pthread_create(&threads[1], NULL, Thread2, NULL)) ){
        logTsMsg(ERR_MSG, LOGPATH_OPERATION, "Error:unable to create thread.");
        return 0;
    }
 
    return 0;
}