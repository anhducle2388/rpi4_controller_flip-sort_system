#include "app_threading.h"

#define IO_TASK_INTERVAL_uS 1000

extern char strMsg[250];
extern char strTmp[100];
extern int  opMode;

int stsUpdateVal = 0;
int LED21 = 0x01, LED43 = 0x84;

// Function for Thread #1 IO Scanning Task
void *Thread_IoTask(void *threadid) {
    uint8_t  i, j, oloop, iloop;
    retGoto_Thread1:
        if (opMode)
        {
            if ((ec_slave[0].state == EC_STATE_OPERATIONAL))
            {
                for (i = 1; i <= ec_slavecount; i++) 
                {
                    
                    oloop = ec_slave[i].Obytes;
                    iloop = ec_slave[i].Ibytes;
                    
                    if (stsUpdateVal)
                    {
                        stsUpdateVal = 0;
                        ec_slave[i].outputs[oloop - 1] = LED43;
                        ec_slave[i].outputs[oloop - 2] = LED21;
                    }

                    if ((oloop > 0) || (iloop > 0))
                    {
                        ec_send_processdata();
                        ec_receive_processdata(EC_TIMEOUTRET);

                        snprintf(strTmp, sizeof(strTmp), "[THRD=1] [id=%d] [slv=%s] [O=]", i, ec_slave[i].name);
                        strcat(strMsg, strTmp);
                        for(j = 0; j < oloop; j++)
                        {
                            snprintf(strTmp, sizeof(strTmp), "%3.2x", ec_slave[i].outputs[j]);
                            strcat(strMsg, strTmp);
                        }

                        strcat(strMsg, " [I=]");
                        for(j = 0; j < iloop; j++)
                        {
                            snprintf(strTmp, sizeof(strTmp), "%3.2x", ec_slave[i].inputs[j]);
                            strcat(strMsg, strTmp);
                        }
                        logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);
                        usleep(IO_TASK_INTERVAL_uS);
                    }
                }  
            }
            stsUpdateVal = 1;
        }
    goto retGoto_Thread1;
}

// Function for Thread #2
void *Thread_Comm(void *threadid) {
    retGoto_Thread2:

        // Toggle LED
        LED21 = LED21? 0: 0xf;

        // Update new value
        stsUpdateVal = 1;

        // Sleep to wait
        sleep(2);
    
    goto retGoto_Thread2;
}


// Function to initialize and init thread control
int cfgThreadMap() {
        
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