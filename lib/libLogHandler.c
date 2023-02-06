#include "../lib/libLogHandler.h"

void logTsMsg(char *mode, char *fpath, char *msg) {
    
    static u_int32_t curTsMs = 0;
    struct timeval curTimeVal;
    char   tmpMsg[128];

    // Get current timestamp in milisecs for return val + print timestamp and actual processing interval
    gettimeofday(&curTimeVal, NULL); 

    // Convert from usec to msec
    curTsMs = curTimeVal.tv_sec*1000LL + curTimeVal.tv_usec/1000; // calculate milliseconds
    strftime(tmpMsg, 200, "%Y-%m-%d %H:%M:%S", localtime(&curTimeVal.tv_sec));

    // Save to log file
    FILE *fptr;
    fptr = fopen(fpath, "a");
    fprintf(fptr, "%s.%03d ", tmpMsg, curTsMs % 1000);
    fprintf(fptr, "%s %s.\n", mode, msg);     
    fclose(fptr);

    // Print out to terminal
    if ((strcmp(mode, LOG_MSG) == 0) || (strcmp(mode, ERR_MSG) == 0)) {        
        printf("%s.%03d ", tmpMsg, curTsMs % 1000);
        printf("%s %s.\n", mode, msg);
    }
}