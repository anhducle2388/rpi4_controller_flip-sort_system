#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wiringPi.h>
#include <sys/file.h>

#include "app_timing_control.h"
#include "app_threading.h"

#include "lib/libLogHandler.h"
#include "lib/libJsonConfig.h"
#include "lib/libSoemBeckhoff.h"

#ifdef NUM_THREADS
   #define NUM_THREADS         2
#endif

#ifdef INTERVAL_IN_MSEC
    #define INTERVAL_IN_MSEC   500
#endif

// Dev-defined Functions 
int cfgHardwareGpio(void);
int cfgSoftwareGpio(void);
int cfgTimerInterupt(void);
int getInstatnceStatus(void);
int getJsonConfig(void);

uint8_t  IOmap[4096];
cfgEcat cfgEcatJson;

/* ################################################ */
/* ################# MAIN PROGRAM ################# */
/* ################################################ */

int main(void) {
    /*
    Main program execution, includes 3 main section:
    - cfgHardwareGpio() -> Config Gpio as digital/analog IO
    - cfgSoftwareGpio() -> Config advanced funtions for IO port like UART...
    - cfgTimerInterupt() -> Interupt config + Routine timer interupt config
    - exeCycleProgram()      -> Conituous looping execution program 
    - execTimingProgram()      -> Routine execution program with pre-defined interval
    */

    system("clear");
    
    if (getInstatnceStatus() == 0) {

        // If the program inits the first instance
        getJsonConfig();
        cfgHardwareGpio();
        cfgSoftwareGpio();
        cfgTimerInterupt();

        getEcatCommJson(ECAT_SOEM_CONFG, &cfgEcatJson);
        cfgHardwareEcatSoem(&cfgEcatJson);   

        threadConfigure();

        logTsMsg(LOG_MSG, OPER_LPATH, "Complete initializing program.");

        // Main loop program
        while(1)
        {
            
        }
        pthread_exit(NULL);
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

int getJsonConfig(void) {

    double val;

    logTsMsg(LOG_MSG, OPER_LPATH, "Loading parameters from  /dat/device_calib.json");

    getDeviceCalibParams("LoadCell-1", "slope", &val);
    getDeviceCalibParams("LoadCell-1", "const", &val);

    logTsMsg(LOG_MSG, OPER_LPATH, "Successfully load calibration parameters.");
    return 0;
}

int cfgHardwareGpio(void) {
    wiringPiSetupPhys();
    pinMode(40, OUTPUT);
    pinMode(38, OUTPUT);
    return 0;
}

int cfgSoftwareGpio(void) {
    return 0;
}

int cfgTimerInterupt() {
    // Config routine uAlarm singal to execute execTimingProgram() in precise interval timing
    signal(SIGALRM, execTimingProgram);   
    ualarm(INTERVAL_IN_MSEC * 1000, INTERVAL_IN_MSEC * 1000);
    return 0;
}