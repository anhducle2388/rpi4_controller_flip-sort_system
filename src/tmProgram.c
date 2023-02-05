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

#ifdef NUM_THREADS
   #define NUM_THREADS         3
#endif

#ifdef INTERVAL_IN_MSEC
    #define INTERVAL_IN_MSEC   50
#endif

#define OPMODE_STOP     0
#define OPMODE_RUN      1
#define OPMODE_HOME     2

#define COMMAND_STOP    0
#define COMMAND_OPER     1
#define COMMAND_HOME    2

// Dev-defined Functions

int getInstatnceStatus(void);
int getJsonDeviceCfg(void);
int cfgHdwrGpio(void);
int cfgSftwGpio(void);
int cfgInteruptTimer(void);

int OpMode = OPMODE_STOP, preOpMode;

uint8_t IOMap[4096];
cfgEcat cfgEcatJson;

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
    
    if (getInstatnceStatus() == 0) {

        // If the program inits the first instance
        getJsonDeviceCfg();
        getJsonEcatComm(&cfgEcatJson);
        cfgHdwrEcatComm(&cfgEcatJson);   
        
        cfgHdwrGpio();
        cfgSftwGpio();
        cfgInteruptTimer();

        

        cfgThreadMap();


        logTsMsg(LOG_MSG, OPER_LPATH, "Complete initializing program");
        
        OpMode = OPMODE_RUN;
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
    pinMode(40, OUTPUT);
    pinMode(38, OUTPUT);
    return 0;
}

int cfgSftwGpio(void) {
    return 0;
}

int cfgInteruptTimer() {
    // Config routine uAlarm singal to execute execTimingProgram() in precise interval timing
    signal(SIGALRM, execTimingProgram);   
    ualarm(INTERVAL_IN_MSEC * 1000, INTERVAL_IN_MSEC * 1000);
    return 0;
}