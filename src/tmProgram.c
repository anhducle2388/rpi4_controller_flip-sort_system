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
#include "ethercat.h"

#ifdef NUM_THREADS
   #define NUM_THREADS         2
#endif

#ifdef INTERVAL_IN_MSEC
    #define INTERVAL_IN_MSEC   500
#endif

// Dev-defined Functions 
int hwGpioConfigure(void);
int fwGpioConfigure(void);
int interptConfigure(void);
int programStatusVerification(void);
int programJsonConfig(void);

/* ################################################ */
/* ################# MAIN PROGRAM ################# */
/* ################################################ */

int main(void) {
    /*
    Main program execution, includes 3 main section:
    - hwGpioConfigure() -> Config Gpio as digital/analog IO
    - fwGpioConfigure() -> Config advanced funtions for IO port like UART...
    - interptConfigure() -> Interupt config + Routine timer interupt config
    - loopProgram()      -> Conituous looping execution program 
    - execProgram()      -> Routine execution program with pre-defined interval
    */

    system("clear");
    
    if (programStatusVerification() == 0) {

        // If the program inits the first instance
        programJsonConfig();
        hwGpioConfigure();
        fwGpioConfigure();
        interptConfigure();
        threadConfigure();

        if (ec_init("eht0") > 0) {
        printf("Ethercat init success.\n");
        }
        else {
            printf("Ethercat init success.\n");
        }

        logTsMsg(LOG_MSG, LOGPATH_OPERATION, "Complete initializing program.");

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

int programStatusVerification(void) {

    // Path to the lock file
    char *lock_file = "./bin/app.pid";

    // Open the lock file
    int fd = open(lock_file, O_RDWR | O_CREAT, 0640);
    if (fd < 0) {
        logTsMsg(ERR_MSG, LOGPATH_OPERATION,"Fail to create program instance. Unable to open lock file.");
        return 1;
    }

    // Try to acquire a lock on the file
    if (flock(fd, LOCK_EX | LOCK_NB) < 0) {
        logTsMsg(ERR_MSG, LOGPATH_OPERATION,"Fail to create program instance. Unable to acquire lock on file.");
        close(fd);
        return 1;
    }

    // Write the PID to the lock file
    pid_t pid = getpid();
    char pid_str[16];
    snprintf(pid_str, sizeof(pid_str), "%d", pid);
    if (write(fd, pid_str, strlen(pid_str)) < 0) {
        logTsMsg(ERR_MSG, LOGPATH_OPERATION,"Fail to create program instance. Unable to write PID to lock file.");
        close(fd);
        return 1;
    }
    
    logTsMsg(LOG_MSG, LOGPATH_OPERATION, "Start initializing program instance.");
    return 0;
}

int programJsonConfig(void) {

    double val;

    logTsMsg(LOG_MSG, LOGPATH_OPERATION, "Loading parameters from  /dat/calib.json.");

    getDeviceCalibParams("LoadCell-1", "slope", &val);
    getDeviceCalibParams("LoadCell-1", "const", &val);

    logTsMsg(LOG_MSG, LOGPATH_OPERATION, "Successfully load calibration parameters.");
    return 0;
}

int hwGpioConfigure(void) {
    wiringPiSetupPhys();
    pinMode(40, OUTPUT);
    pinMode(38, OUTPUT);
    return 0;
}

int fwGpioConfigure(void) {
    return 0;
}

int interptConfigure() {
    // Config routine uAlarm singal to execute execProgram() in precise interval timing
    signal(SIGALRM, execProgram);   
    ualarm(INTERVAL_IN_MSEC * 1000, INTERVAL_IN_MSEC * 1000);
    return 0;
}