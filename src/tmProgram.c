#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wiringPi.h>
#include "../lib/libLogHandler.h"

#define INTERVAL_IN_MSEC  10
#define NUM_TESTING_CYCLE 1000000

// DEBUG LOGGING
#define LOG_FILEPATH     "./log/log.txt"
#define DEBUG_INTERVAL

int excProgram(void);

int main(int argc, char *argv[])
{
    //use the physical pin numbers on the P1 connector
    wiringPiSetupPhys();
    pinMode(40, OUTPUT);
    pinMode(38, OUTPUT);

    // Config routine uAlarm singal to execute excProgram() in precise interval timing
    signal(SIGALRM, excProgram);   
    ualarm(INTERVAL_IN_MSEC * 1000, INTERVAL_IN_MSEC * 1000);

    // Main loop program
    while(1)
    {
        
    }

    return 0;
}

int excProgram(void) {
    #ifdef DEBUG_INTERVAL
    // Log control interval and number of testing cycles
    char strNumOfCyc[10] = "";
    snprintf(strNumOfCyc, 10, "%d", NUM_TESTING_CYCLE);  
    logTsMsg(LOG_MSG, LOG_FILEPATH, strNumOfCyc);
    #endif

    for(u_int64_t i = 0; i < NUM_TESTING_CYCLE; i++);
    return 0;
}
