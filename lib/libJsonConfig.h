#include <stdio.h>
#include <json-c/json.h>
#include <time.h>
#include <sys/time.h>

#define DEVICE_CALIB_LOG "./dat/calib.json"

int getDeviceCalibParams(char *devName,char *paramName, double *retVar);
int setDeviceCalibParams(char *devName,char *paramName, double setVal);
