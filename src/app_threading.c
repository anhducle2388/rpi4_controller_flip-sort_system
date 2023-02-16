#include "app_threading.h"
#include "app.h"

#define NUM_THREADS 3

#define IO_TASK_INTERVAL_uS 1000
#define IO_TASK_ECATDIAG_us 100

extern cfgOper cfgAppInst;
extern cfgEcat cfgEcatInst;
extern cfgZmq  cfgZmqInst;

ddo_KL2134t * ddo_KL2134_12;
ddo_KL2134t * ddo_KL2134_34;
ddi_KL1104t * ddi_KL1104_12;
ddi_KL1104t * ddi_KL1104_34;
ao_KL4002t  * ao_KL4002_1;
ai_KL3202t  * ai_KL3202_1;

// User defined function for user variable to Io Mapping
void setUserVariableToIoMapping(void) {

    /* [slv=BK1120] [Phs]  [BK1120] -> [DO1|DO2|DO3|DO4] ->  [DI1|DI2|DI3|DI4] -> [AO] -> [AI]
                            __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ ___ ___ ___ ___
                    [Idx]  | 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|     18|     19|
                    [Out]  |00 00 00 00|00 00 00 00|00 00 00 00|00 00 00 00|  0   8|  0   0|   From Master to Slave
                           |  AO_CH1   |  AO_CH2   |   AI_CH1  |   AI_CH2  |DO2-DO1|DO4-DO3|
                           |CT __ BH BL|CT __ BH BL|CT __ __ __|CT __ __ __|___ ___ ___ ___|
                    [In]   |00 00 00 00|00 00 00 00|42 00 34 21|42 00 34 21|  0|  0|  0|  0|   From Slave  to Master
                           |  AO_CH1   |  AO_CH2   |   AI_CH1  |   AI_CH2  |DI2-DI1|DI4-DI3|
                           |__ __ __ __|__ __ __ __|ST __ BH BL|ST __ BH BL|___ ___ ___ ___|
    */
    
    // Function to define struct pointer to IO Map and config/init value
    ddo_KL2134_12 = (ddo_KL2134t *) &ec_slave[1].outputs[18];
    ddo_KL2134_34 = (ddo_KL2134t *) &ec_slave[1].outputs[19];
    ao_KL4002_1   = (ao_KL4002t *) &ec_slave[1].outputs[6];

    ddi_KL1104_12 = (ddi_KL1104t *) &ec_slave[1].inputs[18];
    ddi_KL1104_34 = (ddi_KL1104t *) &ec_slave[1].inputs[19];
    ai_KL3202_1   = (ai_KL3202t *) &ec_slave[1].inputs[10];

    // Define IO port macro for later use
    #define rly_STATUS_LED     ddo_KL2134_12->idx1_channel_1
    #define rly_TRIGGER_ON_2   ddo_KL2134_34->idx1_channel_3
    #define snr_SAFETY_1       ddi_KL1104_12->idx1_channel_1

    #define sts_TEMPERATURE_1  ai_KL3202_1->channel_2.stts
    #define val_TEMPERATURE_1  ai_KL3202_1->channel_1.data

    // Init state for Digital / Analog Output
    rly_STATUS_LED   = TRUE;
    rly_TRIGGER_ON_2 = TRUE;
}

// Function for Thread #1 IO Scanning Task
void *Thread_IoTask(void *threadid) {

    // setUserVariableToIoMapping();

    retGoto_Thread1:
        if (cfgEcatInst.isRun == TRUE) 
        {
            printf("%d\n", cfgEcatInst.isRun);
            // Send and recevie ECAT process data -> IOMap
            ec_send_processdata();
            cfgEcatInst.curWkc = ec_receive_processdata(EC_TIMEOUTRET);
            
            // Sleep
            usleep(IO_TASK_INTERVAL_uS);
        }
    goto retGoto_Thread1;
}

// Function for Thread #2
void *Thread_DiagComm(void *threadid) {

    retGoto_Thread2:
        // Ecat Diag
        if (cfgEcatInst.isRun == TRUE) 
        {
            chkEcatDiagnosis(&cfgEcatInst);
            usleep(IO_TASK_ECATDIAG_us);
        }
    goto retGoto_Thread2;
}

// Function for Thread #3
void *Thread_Handler(void *threadid) {

    retGoto_Thread3:
        // Toggle to inducate tool still running'
        #ifdef DEBUG_IO_TASK_INTERVAL
        // if (cfgEcatInst.isRun == TRUE) getEcatIoFrame(&cfgEcatInst);
        // TOGGLE(rly_STATUS_LED);
        #endif              
        
        // Do something like Rest API handing and Database Logging.
        if (cfgZmqInst.isRun == TRUE)
        {
            int retval;
            
            // Receive
            getIntZmqVal(cfgZmqInst.ZmqSocket, "python", &retval);

            // Sending
            setIntZmqVal(cfgZmqInst.ZmqSocket, "c", 20);
        }
        // Sleep
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
        return 1;
    }

    // Mapping thread #2 to map list
    logTsMsg(LOG_MSG, OPER_LPATH, "Creating thread #2 Io task diag");
    if ( pthread_create(&threads[1], NULL, Thread_DiagComm, NULL) ){
        logTsMsg(ERR_MSG, OPER_LPATH, "Error: unable to create thread #2 Io Task Diag");
        return 1;
    }
 
    // Mapping thread #2 to map list
    logTsMsg(LOG_MSG, OPER_LPATH, "Creating thread #3");
    if ( pthread_create(&threads[2], NULL, Thread_Handler, NULL) ){
        logTsMsg(ERR_MSG, OPER_LPATH, "Error: unable to create thread #3");
        return 1;
    }

    return 0;
}
