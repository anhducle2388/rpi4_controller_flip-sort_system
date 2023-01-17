#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wiringPi.h>
#include "../lib/libLogHandler.h"

#define INTERVAL_IN_MSEC  10

// Debugging
#define DEBUG_INTERVAL
#ifdef  DEBUG_INTERVAL
    #define NUM_TESTING_CYCLE 1000000
#endif

// Defined Directory
#define LOG_FILEPATH     "./log/log.txt"

// Dev-defined Functions 
int hwGpioConfigure(void);
int fwGpioConfigure(void);
int interptConfigure(void);
int loopProgram(void);
int execProgram(void);

/* ################################################ */
/* ################# MAIN PROGRAM ################# */
/* ################################################ */

int main(int argc, char *argv[]) {
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
    
    // Main loop program
    while(1)
    {
        loopProgram();
    }

    return 0;
}

/* ################################################ */
/* ################ CONFIG PROGRAM ################ */
/* ################################################ */

int execProgram(void) {
    /*
    Routined program execution - Control interval is defined in INTERVAL_IN_MSEC
    */

    #ifdef DEBUG_INTERVAL
    // Log control interval and number of testing cycles
    char strNumOfCyc[10] = "";
    snprintf(strNumOfCyc, 10, "%d", NUM_TESTING_CYCLE);  
    logTsMsg(LOG_MSG, LOG_FILEPATH, strNumOfCyc);
    for(u_int64_t i = 0; i < NUM_TESTING_CYCLE; i++);
    #endif

    return 0;
}


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
