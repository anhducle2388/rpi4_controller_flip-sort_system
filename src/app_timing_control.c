#include "app_timing_control.h"
#include "./lib/libLogHandler.h"

#define OPMODE_STOP     0
#define OPMODE_OPER     1
#define OPMODE_HOME     2

#define COMMAND_STOP    0
#define COMMAND_OPER    1
#define COMMAND_HOME    2

extern int  OpMode, preOpMode;
extern char strMsg[250];
extern char strTmp[100];

int execTimingProgram(void) {
    /*
    Routined program execution - Control interval is defined in INTERVAL_IN_MSEC
    */

    #ifdef DEBUG_INTERVAL
    logTsMsg(LOG_MSG, TIME_LPATH, "");
    #endif

    if(preOpMode != OpMode)
    {
        snprintf(strMsg, sizeof(strMsg), "OpMode has been changed from %d to %d", preOpMode, OpMode);
        logTsMsg(LOG_MSG, OPER_LPATH, strMsg);
    }

    switch (OpMode)
    {
    case OPMODE_STOP:
        // logTsMsg(DBG_MSG, OPER_LPATH, "Operation Mode = STOP");
        break;

    case OPMODE_HOME:
        // logTsMsg(DBG_MSG, OPER_LPATH, "Operation Mode = HOME");
        break;

    case OPMODE_OPER:
        // logTsMsg(DBG_MSG, OPER_LPATH, "Operation Mode = OPER");
        break;

    default:
        break;
    }
    
    preOpMode = OpMode;
    return 0;
}