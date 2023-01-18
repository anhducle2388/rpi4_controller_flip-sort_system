#include "app_timing_control.h"
#include "./lib/libLogHandler.h"

// Debug variables
#define DEBUG_INTERVAL
#ifdef  DEBUG_INTERVAL
    #define NUM_TESTING_CYCLE 1000000
#endif

// Defined Directory
#define LOG_FILEPATH  "./log/log.txt"

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