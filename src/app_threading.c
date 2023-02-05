#include "app_threading.h"

#define NUM_THREADS 3

#define IO_TASK_INTERVAL_uS 10000
#define IO_TASK_ECATDIAG_us 5000

extern int OpMode, preOpMode;
extern char strMsg[250];
extern char strTmp[100];

uint8_t stsUpdateVal = FALSE;
uint8_t curWkc = 0, preWkc = 0;
uint8_t LED21 = 0x01;
uint8_t LED43 = 0x84;

// Function for Thread #1 IO Scanning Task
void *Thread_IoTask(void *threadid) {
    uint8_t  i, j, oloop, iloop;
    retGoto_Thread1:
        if (OpMode)
        {
            if ((ec_slave[0].state == EC_STATE_OPERATIONAL))
            {
                for (i = 1; i <= ec_slavecount; i++)
                {
                    oloop = ec_slave[i].Obytes;
                    iloop = ec_slave[i].Ibytes;

                    if (stsUpdateVal)
                    {
                        stsUpdateVal = FALSE;
                        ec_slave[i].outputs[oloop - 1] = LED43;
                        ec_slave[i].outputs[oloop - 2] = LED21;
                    }

                    ec_send_processdata();
                    curWkc = ec_receive_processdata(EC_TIMEOUTRET);
                    if(ec_iserror() || (curWkc != preWkc)) {
                        snprintf(strMsg, sizeof(strMsg), "Wkc has been changed from %d to %d", preWkc, curWkc);
                        logTsMsg(LOG_MSG, OPER_LPATH, strMsg);
                    }

                    // if (( curWkc >= 3) && ((oloop > 0) || (iloop > 0)))
                    // {
                    //     snprintf(strTmp, sizeof(strTmp), "[THRD=1] [id=%d] [slv=%s] [wkc=%d] [O=]", i, ec_slave[i].name, curWkc);
                    //     strcat(strMsg, strTmp);
                    //     for(j = 0; j < oloop; j++)
                    //     {
                    //         snprintf(strTmp, sizeof(strTmp), "%3.2x", ec_slave[i].outputs[j]);
                    //         strcat(strMsg, strTmp);
                    //     }

                    //     strcat(strMsg, " [I=]");
                    //     for(j = 0; j < iloop; j++)
                    //     {
                    //         snprintf(strTmp, sizeof(strTmp), "%3.2x", ec_slave[i].inputs[j]);
                    //         strcat(strMsg, strTmp);
                    //     }
                    //     logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);
                    //     usleep(IO_TASK_INTERVAL_uS);
                    // }
                }
            }
            preWkc = curWkc;
            stsUpdateVal = 1;
        }
        usleep(IO_TASK_INTERVAL_uS);
    goto retGoto_Thread1;
}

// Function for Thread #2
void *Thread_DiagComm(void *threadid) {
    retGoto_Thread2:
        chkEcatDiagnosis();
        usleep(IO_TASK_ECATDIAG_us);
    goto retGoto_Thread2;
}

// Function for Thread #3
void *Thread_Handler(void *threadid) {
    retGoto_Thread3:

        // Toggle LED
        LED21 = LED21? 0: 0xf;

        // Update new value
        stsUpdateVal = TRUE;

        // Sleep to wait
        usleep(IO_TASK_ECATDIAG_us * 50);

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
