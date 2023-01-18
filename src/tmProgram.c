#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wiringPi.h>

#include "app_timing_control.h"
#include "app_threading.h"

#ifndef NUM_THREADS
   #define NUM_THREADS         2
#endif

#ifndef INTERVAL_IN_MSEC
    #define INTERVAL_IN_MSEC   500
#endif

// Dev-defined Functions 
int hwGpioConfigure(void);
int fwGpioConfigure(void);
int interptConfigure(void);

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
    hwGpioConfigure();
    fwGpioConfigure();
    interptConfigure();
    threadConfigure();
    
    // Main loop program
    while(1)
    {
        
    }

    pthread_exit(NULL);
    return 0;
}

/* ################################################ */
/* ################ CONFIG PROGRAM ################ */
/* ################################################ */

int loopProgram(void) {
    return 0;
}

/* ################################################ */
/* ################ CONFIG PROGRAM ################ */
/* ################################################ */

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