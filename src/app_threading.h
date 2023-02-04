#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "lib/libLogHandler.h"
#include "lib/libSoemBeckhoff.h"

#define NUM_THREADS 2

void *Thread_IoTask(void *threadid);
void *Thread_Comm(void *threadid);
int cfgThreadMap(void);

