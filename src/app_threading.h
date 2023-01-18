#include <stdio.h>
#include <unistd.h>
#include "pthread.h"

#define NUM_THREADS 2

void *Thread1(void *threadid);
void *Thread2(void *threadid);
int threadConfigure(void);

