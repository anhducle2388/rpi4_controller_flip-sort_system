#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ethercat.h>
#include <lib/libJsonConfig.h>
#include <lib/libLogHandler.h>

#define ECAT_SOEM_CONFG   "./dat/comm_ecat.json"
#define ECAT_SOEM_LPATH   "./log/comm_log.txt"
#define ECAT_INIT_RETRY    200

#define DO_KL2134
#define DI_KL1104
#define AO_KL4002
#define AI_KL3202

typedef struct {
   char     *ifname;
   uint8_t  numOfNodes;
} cfgEcat;

int getEcatCommJson(char * jsonPath, cfgEcat * cfgEcat);
int cfgHardwareEcatSoem(cfgEcat * cfgEcat);