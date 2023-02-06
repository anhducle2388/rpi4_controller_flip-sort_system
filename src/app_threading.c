#include "app_threading.h"

#define NUM_THREADS 3

#define IO_TASK_INTERVAL_uS 1000
#define IO_TASK_ECATDIAG_us 100

extern int  OpMode, preOpMode;

extern cfgEcat cfgEcatJson;

do_KL2134t * do_KL2134_1, * do_KL2134_3;
di_KL1104t * di_KL1104_1, * di_KL1104_3;
ao_KL4002t * ao_KL4002;
ai_KL3202t * ai_KL3202;

// Function for Thread #1 IO Scanning Task
void *Thread_IoTask(void *threadid) {

    char strMsg[250];

    do_KL2134_1 = (do_KL2134t *) ec_slave[1].outputs + ec_slave[1].Obytes-2;
    do_KL2134_3 = (do_KL2134t *) ec_slave[1].outputs + ec_slave[1].Obytes-1;

    do_KL2134_1->data_frame = 0x01;

    retGoto_Thread1:
    cfgEcatJson.curWkc = ec_receive_processdata(EC_TIMEOUTRET);
    if (ec_slave[0].state == EC_STATE_OPERATIONAL) 
    {
        for (uint8_t i = 1; i <= ec_slavecount; i++)
        {

        }
    }
    ec_send_processdata();
    usleep(IO_TASK_INTERVAL_uS);
    goto retGoto_Thread1;
}

// Function for Thread #2
void *Thread_DiagComm(void *threadid) {
    retGoto_Thread2:
        chkEcatDiagnosis(&cfgEcatJson);
        usleep(IO_TASK_ECATDIAG_us);
    goto retGoto_Thread2;
}

// Function for Thread #3
void *Thread_Handler(void *threadid) {

    retGoto_Thread3:
    // Tasking
    // getEcatIoFrame(&cfgEcatJson);
    do_KL2134_1->data_frame = ( do_KL2134_1->data_frame >> 1) | ( do_KL2134_1->data_frame << (4 - 1));

    // Sleep to wait
    usleep(1000000);

    goto retGoto_Thread3;
}

// Function to initialize and init thread control
int cfgThreadMap() {

    // Init thread list
    pthread_t threads[NUM_THREADS];

    // Mapping Io Scanning to map list
    logTsMsg(LOG_MSG, OPER_LPATH, "Creating thread #1 Io task");
    if ( pthread_create(&threads[0], NULL, Thread_IoTask, NULL) ){
        logTsMsg(ERR_MSG, OPER_LPATH, "Error: unable to create thread #1 Io Task");
        return 0;
    }

    // Mapping thread #2 to map list
    logTsMsg(LOG_MSG, OPER_LPATH, "Creating thread #2 Io task diag");
    if ( pthread_create(&threads[1], NULL, Thread_DiagComm, NULL) ){
        logTsMsg(ERR_MSG, OPER_LPATH, "Error: unable to create thread #2 Io Task Diag");
        return 0;
    }
 
    // Mapping thread #2 to map list
    logTsMsg(LOG_MSG, OPER_LPATH, "Creating thread #3");
    if ( pthread_create(&threads[2], NULL, Thread_Handler, NULL) ){
        logTsMsg(ERR_MSG, OPER_LPATH, "Error: unable to create thread #3");
        return 0;
    }

    return 0;
}
