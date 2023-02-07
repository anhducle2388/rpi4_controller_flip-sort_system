#include "app_timing_control.h"
#include "app.h"

#include "./lib/libLogHandler.h"
#include "./lib/libSoemBeckhoff.h"


extern cfgOper cfgAppInst;
extern cfgEcat cfgEcatJson;

int execTimingProgram(void) {
    // Routined program execution - Control interval is defined in INTERVAL_IN_MSEC
    char   strMsg[250];
    static uint8_t preOpMode = MODE_STOP;

    if(preOpMode != cfgAppInst.OperationMode.sts)
    {
        snprintf(strMsg, sizeof(strMsg), "OpMode has been changed from %d to %d", preOpMode, cfgAppInst.OperationMode.sts);
        logTsMsg(LOG_MSG, OPER_LPATH, strMsg);
    }

    switch (cfgAppInst.OperationMode.cmd)
    {
    case MODE_STOP:
        break;

    case MODE_OPER:
        break;

    case MODE_HOME:
        break;

    case MODE_CALB:
        break;
    }

    switch (cfgAppInst.OperationMode.sts)
    {
    case MODE_STOP:
        break;

    case MODE_OPER:
        break;

    case MODE_HOME:
        break;
    case MODE_CALB:
        break;
    }
    
    preOpMode = cfgAppInst.OperationMode.sts;
    return 0;
}