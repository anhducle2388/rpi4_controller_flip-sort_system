#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "../lib/libLogHandler.h"

void logTsMsg(char *mode, char *fpath, char *msg) {

    static u_int32_t curTsMs = 0, preTsMs = 0;
    struct timeval curTimeVal;
    char   strMsg[128];

    // Open file pointer
    FILE *fptr;
    fptr = fopen(fpath, "a");    

    // Save previous cycle timestamp
    preTsMs = curTsMs;

    // Get current timestamp in milisecs for return val + print timestamp and actual processing interval
    gettimeofday(&curTimeVal, NULL); 

    // Convert from usec to msec
    curTsMs = curTimeVal.tv_sec*1000LL + curTimeVal.tv_usec/1000; // calculate milliseconds
    strftime(strMsg, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTimeVal.tv_sec));

    // Log to Cmd and Logfile
    if (preTsMs == 0) {
        printf("%s.%03d ", strMsg, curTsMs % 1000);
        printf("%s %s\n", mode, "[INIT]");
        fprintf(fptr, "%s.%03d ", strMsg, curTsMs % 1000);
        fprintf(fptr, "%s %s\n", mode, "[INIT]");        

    }
    else {
        printf("%s.%03d ", strMsg, curTsMs % 1000);
        printf("%s [%dms] %s\n", mode, curTsMs - preTsMs, msg);
        fprintf(fptr, "%s.%03d ", strMsg, curTsMs % 1000);
        fprintf(fptr, "%s [%dms] %s\n", mode, curTsMs - preTsMs, msg);        
    }

    // Close file pointer
    fclose(fptr);
}