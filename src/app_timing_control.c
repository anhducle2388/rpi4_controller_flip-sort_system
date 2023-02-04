#include "app_timing_control.h"
#include "./lib/libLogHandler.h"

#define OPMODE_STOP     0
#define OPMODE_RUN      1
#define OPMODE_HOME     2

#define COMMAND_STOP    0
#define COMMAND_RUN     1
#define COMMAND_HOME    2

extern int opMode;

int execTimingProgram(void) {
    /*
    Routined program execution - Control interval is defined in INTERVAL_IN_MSEC
    */

    #ifdef DEBUG_INTERVAL
    logTsMsg(LOG_MSG, TIME_LPATH, "");
    #endif
        
    // Reading digital/analog signal sequence and update to appropriate variables -> Modbus Comm with Remote IO

    
    // Operation mode instructions
    

    switch (opMode)
    {
    case OPMODE_STOP:
        /* code */
        break;

    case OPMODE_HOME:
        /* code */
        break;

    case OPMODE_RUN:
        /* code */
        break;

    default:
        break;
    }

    return 0;
}