#include "./lib/libJsonConfig.h"

int getDeviceCalibParams(char *devName,char *paramName, double *retVar) {

    json_object *logCalData = json_object_from_file(DEVICE_CALIB_LOG);
    json_object *devInstance, *paramVal, *paramDat, *calDate;

    // Retrieve data from json data hierarchy
    json_object_object_get_ex(logCalData, devName, &devInstance);
    json_object_object_get_ex(devInstance, "CalibDate", &calDate);
    json_object_object_get_ex(devInstance, "Params", &paramDat);
    json_object_object_get_ex(paramDat, paramName, &paramVal);

    // Pass value to pointer var
    *retVar = json_object_get_double(paramVal);

    json_object_put(logCalData);

    return 0;
}

int setDeviceCalibParams(char *devName,char *paramName, double setVal) {
    
    json_object *logCalData = json_object_from_file(DEVICE_CALIB_LOG);
    json_object *devInstance, *paramDat;

    struct timeval curTimeVal;
    char   strTs[30];

    // Get current timestamp to update with new calibration value
    gettimeofday(&curTimeVal, NULL);
    strftime(strTs, 30, "%Y-%m-%d %H:%M:%S", localtime(&curTimeVal.tv_sec));

    // Retrieve data from json data hierarchy
    json_object_object_get_ex(logCalData, devName, &devInstance);
    json_object_object_get_ex(devInstance, "Params", &paramDat);

    // Update new calib value and calib date
    json_object_object_add(paramDat, paramName, json_object_new_double(setVal));
    json_object_object_add(devInstance, "CalibDate", json_object_new_string(strTs));
    json_object_to_file(DEVICE_CALIB_LOG, logCalData);

    json_object_put(logCalData);

    return 0;
}