#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#define LOG_MSG "[LOG]"
#define DBG_MSG "[DBG]"
#define ERR_MSG "[ERR]"

#ifndef OPER_LPATH
#define OPER_LPATH  "./log/log_oper.txt"
#endif

void logTsMsg(char *mode, char *fpath, char *msg);
