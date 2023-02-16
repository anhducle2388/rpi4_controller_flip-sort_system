#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wiringPi.h>
#include <sys/file.h>

#include "app_timing_control.h"
#include "app_threading.h"
#include "app.h"

#include "lib/libLogHandler.h"
#include "lib/libJsonConfig.h"
#include "lib/libZeroMqComm.h"
#include "lib/libSoemBeckhoff.h"

// Dev-defined Functions

int getInstatnceStatus(void);
int getJsonDeviceCfg(void);
int cfgHdwrGpio(void);
int cfgSftwGpio(void);
int cfgInteruptTimer(void);


cfgOper   cfgAppInst  = { 
    .OperationMode.sts = MODE_STOP, 
    .OperationMode.cmd = MODE_STOP
    };

cfgEcat   cfgEcatInst = {.isRun = FALSE};
cfgZmq    cfgZmqInst  = {.isRun = FALSE};

/* ################################################ */
/* ################# MAIN PROGRAM ################# */
/* ################################################ */

int main(void) {

    /*
    Main program execution, includes 3 main section:
    - cfgHdwrGpio()         -> Config Gpio as digital/analog IO
    - cfgSftwGpio()         -> Config advanced funtions for IO port like UART...
    - cfgInteruptTimer()    -> Interupt config + Routine timer interupt config
    - exeCycleProgram()     -> Conituous looping execution program 
    - execTimingProgram()   -> Routine execution program with pre-defined interval
    */

    system("clear");

    uint8_t err = 0;
    uint8_t cntRetry = 1; 

    // Check if another instances is running
    if (getInstatnceStatus() == 0) {
        
        // Init the program with max INIT_RETRY_MAX times retry
        do {
            err += getJsonDeviceCfg();
            err += initZeroMq(&cfgZmqInst, "*", 5555);
            err += getJsonEcatComm(&cfgEcatInst);
            err += cfgHdwrEcatComm(&cfgEcatInst);

            err += cfgHdwrGpio();
            err += cfgSftwGpio();
            err += cfgInteruptTimer();
            err += cfgThreadMap();
        }
        while (FALSE); //(err && (cntRetry--));

        // Switch program state to Operation or Fail
        if (!err) {
            logTsMsg(LOG_MSG, OPER_LPATH, "Complete initializing program");
            cfgAppInst.OperationMode.sts = MODE_OPER;
        }
        
        // Main loop program
        while(1)
        {

        }

        // Exit program
        pthread_exit(NULL);
        dnitZeroMq(&cfgZmqInst);
        
        return 0;
    }
    else 
    {
        // If the program has been already running -> Log the error and terminate the duplicated process.
        return 1;
    }
}

/* ################################################ */
/* ################ CONFIG PROGRAM ################ */
/* ################################################ */

int getInstatnceStatus(void) {

    // Path to the lock file
    char *lock_file = "./bin/app.pid";

    // Open the lock file
    int fd = open(lock_file, O_RDWR | O_CREAT, 0640);
    if (fd < 0) {
        logTsMsg(ERR_MSG, OPER_LPATH,"Fail to create program instance. Unable to open lock file");
        return 1;
    }

    // Try to acquire a lock on the file
    if (flock(fd, LOCK_EX | LOCK_NB) < 0) {
        logTsMsg(ERR_MSG, OPER_LPATH,"Fail to create program instance. Unable to acquire lock on file");
        close(fd);
        return 1;
    }

    // Write the PID to the lock file
    pid_t pid = getpid();
    char pid_str[16];
    snprintf(pid_str, sizeof(pid_str), "%d", pid);
    if (write(fd, pid_str, strlen(pid_str)) < 0) {
        logTsMsg(ERR_MSG, OPER_LPATH,"Fail to create program instance. Unable to write PID to lock file");
        close(fd);
        return 1;
    }
    
    logTsMsg(LOG_MSG, OPER_LPATH, "Start initializing program instance");
    return 0;
}

int getJsonDeviceCfg(void) {

    double val;

    logTsMsg(LOG_MSG, OPER_LPATH, "Load device parameters");

    getDeviceCalibParams("LoadCell-1", "slope", &val);
    getDeviceCalibParams("LoadCell-1", "const", &val);

    logTsMsg(LOG_MSG, OPER_LPATH, "Load device parameters successfully");
    return 0;
}

int cfgHdwrGpio(void) {
    wiringPiSetupPhys();
    return 0;
}

int cfgSftwGpio(void) {
    return 0;
}

int cfgInteruptTimer() {
    // Config routine uAlarm singal to execute execTimingProgram() in precise interval timing
    signal(SIGALRM, execTimingProgram);   
    ualarm(EXECUTE_INTERVAL_ms * 1000, EXECUTE_INTERVAL_ms * 1000);
    return 0;
}