#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>

#define INTERVAL_IN_MSEC  10
#define NUM_TESTING_CYCLE 1000000

#define LOG_FILEPATH     "./log/log.txt"
#define LOG_MSG "[LOG]"
#define DBG_MSG "[DBG]"
#define ERR_MSG "[ERR]"

#define DEBUG

void excProgram(int sig_num);
void logTsMsg(char *mode, char *fpath, char *msg);


int main(int argc, char *argv[])
{
    //use the physical pin numbers on the P1 connector
    wiringPiSetupPhys();
    pinMode(40, OUTPUT);
    pinMode(38, OUTPUT);

    // Config routine uAlarm singla to execute excProgram() in precise interval timing
    signal(SIGALRM, excProgram);   
    ualarm(INTERVAL_IN_MSEC * 1000, INTERVAL_IN_MSEC * 1000);

    // Main loop program
    while(1)
    {
        
    }

    return 0;
}

void excProgram(int sig_num)
{
    #ifdef DEBUG
    // Log control interval and number of testing cycles
    char strNumOfCyc[10] = "";
    snprintf(strNumOfCyc, 10, "%d", NUM_TESTING_CYCLE);  
    logTsMsg(LOG_MSG, LOG_FILEPATH, strNumOfCyc);
    #endif

    for(u_int64_t i = 0; i < NUM_TESTING_CYCLE; i++);
    
}

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

