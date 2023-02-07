#include <stdio.h>
#include <time.h>
#include <json-c/json.h>
#include <sys/time.h>

#define DEVICE_CALIB_LOG "./dat/device_calib.json"

int getDeviceCalibParams(char *devName,char *paramName, double *retVar);
int setDeviceCalibParams(char *devName,char *paramName, double setVal);
