#include <stdio.h>
#include <string.h>
#include <zmq.h>
#include <json-c/json.h>

#define BUFFER_SIZE 1024

#ifndef ZMQ_STRUCT
typedef struct {
   void * ZmqContext;
   void * ZmgSocket;
   uint8_t isRun;
} cfgZmq;
#define ZMQ_STRUCT
#endif

void setIntZmqVal(void *socket, char * set_var, int val);
void getIntZmqVal(void *socket, char * req_varname, int * cVar);
void setDoubleZmqVal(void *socket, char * set_var, double val) ;
void getDoubleZmqVal(void *socket, char * req_varname, double * cVar);
