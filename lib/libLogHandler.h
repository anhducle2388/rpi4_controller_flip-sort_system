#define LOG_MSG "[LOG]"
#define DBG_MSG "[DBG]"
#define ERR_MSG "[ERR]"

#ifndef LOGPATH_OPERATION
#define LOGPATH_OPERATION  "./log/oplog.txt"
#endif

void logTsMsg(char *mode, char *fpath, char *msg);
