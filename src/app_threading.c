#include "app_threading.h"

#define NUM_THREADS 3

#define IO_TASK_INTERVAL_uS 1000
#define IO_TASK_ECATDIAG_us 100

extern int  OpMode, preOpMode;

uint8_t stsUpdateVal = FALSE;
uint8_t LED21 = 0x01;
uint8_t LED43 = 0x84;

extern cfgEcat   cfgEcatJson; 

// Function for Thread #1 IO Scanning Task
void *Thread_IoTask(void *threadid) {

    char strMsg[250];

    uint8_t i, j, oloop, iloop;
    uint8_t preWkc = 0;

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
                    cfgEcatJson.curWkc = ec_receive_processdata(EC_TIMEOUTRET);
                    
                    // Log comm oper if the Wkc has been changed
                    if(cfgEcatJson.curWkc != preWkc) {
                        snprintf(strMsg, sizeof(strMsg), "[ECAT] Wkc has been changed from %d to %d", preWkc, cfgEcatJson.curWkc);
                        logTsMsg(LOG_MSG, ECAT_SOEM_LPATH, strMsg);
                    }

                    // Log error if Wkc not at expected value.
                    if(cfgEcatJson.curWkc < cfgEcatJson.expectedWkc) {
                        snprintf(strMsg, sizeof(strMsg), "[ECAT] Wkc = %d < Expected Wkc %d", cfgEcatJson.curWkc, cfgEcatJson.expectedWkc);
                        logTsMsg(ERR_MSG, ECAT_SOEM_LPATH, strMsg);                        
                    }

                    // if (( curWkc >= 3) && ((oloop > 0) || (iloop > 0)))
                    // {
                    //     char strTmp[100];
                    //     snprintf(strMsg, sizeof(strMsg), "[THRD=1] [id=%d] [slv=%s] [wkc=%d] [O=]", i, ec_slave[i].name, curWkc);
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
            preWkc = cfgEcatJson.curWkc;
            stsUpdateVal = 1;
        }
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

        // Toggle LED
        LED21 = LED21? 0: 0xf;

        // Update new value
        stsUpdateVal = TRUE;

        // Sleep to wait
        usleep(500000);

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
