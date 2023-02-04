#include "app_threading.h"

// Function for Thread #1
void *Thread_IoTask(void *threadid){
    while(1)
    {
        // logTsMsg(LOG_MSG, OPER_LPATH, "Execute thread #1 io task");
        usleep(1000);
    }
}

// Function for Thread #2
void *Thread_Comm(void *threadid){
    while(1)
    {
        // logTsMsg(LOG_MSG, OPER_LPATH, "Execute thread #2");
        sleep(4);
    }
}

// Function to initialize and init thread control
int threadConfigure() {
        
    // Init thread list
    pthread_t threads[NUM_THREADS];

    // Mapping Io Scanning to map list 
    logTsMsg(LOG_MSG, OPER_LPATH, "Creating thread #1 Io task");
    if ( pthread_create(&threads[0], NULL, Thread_IoTask, NULL) ){
        logTsMsg(ERR_MSG, OPER_LPATH, "Error: unable to create  thread #1 IO task\n");
        return 0;
    }

    // Mapping thread #2 to map list
    logTsMsg(LOG_MSG, OPER_LPATH, "Creating thread #2");
    if ( pthread_create(&threads[1], NULL, Thread_Comm, NULL) ){
        logTsMsg(ERR_MSG, OPER_LPATH, "Error:unable to create thread");
        return 0;
    }
 
    return 0;
}