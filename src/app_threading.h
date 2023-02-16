#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "lib/libLogHandler.h"
#include "lib/libSoemBeckhoff.h"
#include "lib/libZeroMqComm.h"

void *Thread_IoTask(void *threadid);
void *Thread_DiagComm(void *threadid);
void *Thread_Handler(void *threadid);
int  cfgThreadMap(void);

