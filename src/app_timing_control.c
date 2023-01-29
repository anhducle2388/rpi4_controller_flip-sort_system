#include "app_timing_control.h"
#include "./lib/libLogHandler.h"

// Debug variables
#define DEBUG_INTERVAL
#ifdef  DEBUG_INTERVAL
    #define NUM_TESTING_CYCLE 1000000
#endif

int execProgram(void) {
    /*
    Routined program execution - Control interval is defined in INTERVAL_IN_MSEC
    */

    #ifdef DEBUG_INTERVAL
    // Log control interval and number of testing cycles
    char strNumOfCyc[10] = "";
    snprintf(strNumOfCyc, 10, "%d", NUM_TESTING_CYCLE);  
    logTsMsg(LOG_MSG, LOGPATH_INTERVAL_CONTROL, strNumOfCyc);
    for(u_int64_t i = 0; i < NUM_TESTING_CYCLE; i++);
    #endif

    return 0;
}