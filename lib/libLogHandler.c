#include "../lib/libLogHandler.h"

char strMsg[250];
char strTmp[10];

void logTsMsg(char *mode, char *fpath, char *msg) {

    static u_int32_t curTsMs = 0, preTsMs = 0;
    struct timeval curTimeVal;
    char   tmpMsg[128];

    // Open file pointer
    FILE *fptr;
    fptr = fopen(fpath, "a");    

    // Save previous cycle timestamp
    preTsMs = curTsMs;

    // Get current timestamp in milisecs for return val + print timestamp and actual processing interval
    gettimeofday(&curTimeVal, NULL); 

    // Convert from usec to msec
    curTsMs = curTimeVal.tv_sec*1000LL + curTimeVal.tv_usec/1000; // calculate milliseconds
    strftime(tmpMsg, 200, "%Y-%m-%d %H:%M:%S", localtime(&curTimeVal.tv_sec));

    // Log to Cmd and Logfile
    if (preTsMs == 0) {
        printf("%s.%03d ", tmpMsg, curTsMs % 1000);
        printf("%s %s.\n", mode, msg);
        fprintf(fptr, "%s.%03d ", tmpMsg, curTsMs % 1000);
        fprintf(fptr, "%s %s.\n", mode, msg);        
    }
    else {
        printf("%s.%03d ", tmpMsg, curTsMs % 1000);
        printf("%s %s.\n", mode, msg);
        fprintf(fptr, "%s.%03d ", tmpMsg, curTsMs % 1000);
        fprintf(fptr, "%s %s.\n", mode, msg);        
    }

    // Clear string message after log
    strcpy(strMsg,"");

    // Close file pointer
    fclose(fptr);
}