#include <stdio.h>
#include <string.h>
#include <zmq.h>
#include <json-c/json.h>

#include "lib/libLogHandler.h"

#define ZMQ_COMM_LPATH   "./log/log_zmq.txt"

#define BUFFER_SIZE 1024
#define TRUE   1
#define FALSE  0

#ifndef ZMQ_STRUCT
typedef struct {
   void * ZmqContext;
   void * ZmqSocket;
   uint8_t isRun;
} cfgZmq;
#define ZMQ_STRUCT
#endif

void setIntZmqVal(void *socket, char * set_var, int val);
void getIntZmqVal(void *socket, char * req_varname, int * cVar);
void setDoubleZmqVal(void *socket, char * set_var, double val) ;
void getDoubleZmqVal(void *socket, char * req_varname, double * cVar);

int initZeroMq(cfgZmq * cfgZmq, char * ip_addr, int port);
int dnitZeroMq(cfgZmq * cfgZmq);
