#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ethercat.h>
#include <lib/libJsonConfig.h>
#include <lib/libLogHandler.h>

#define ECAT_SOEM_CONFG   "./dat/comm_ecat.json"
#define ECAT_SOEM_LPATH   "./log/log_comm.txt"
#define ECAT_INIT_RETRY    200

#define ECAT_IOMAP_BUFFER  4096

typedef struct {
   char const *ifname;
   uint8_t     numOfNodes;
   uint8_t    IOMap[ECAT_IOMAP_BUFFER];
   uint8_t    curWkc;
   uint8_t    expectedWkc;
   uint8_t    isRun;
} cfgEcat;

int getJsonEcatComm(cfgEcat * cfgEcat);
int cfgHdwrEcatComm(cfgEcat * cfgEcat);
int chkEcatDiagnosis(cfgEcat * cfgEcat);

int wrDigOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, boolean setval);
int rdDigOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, boolean *retval);
int rdDigIn(uint8_t slvId, uint8_t terminalId, uint8_t channelId, boolean *retval);;

int wrAnlgOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, uint16_t retval);
int rdAnlgOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, uint16_t *retval);
int rdAnlgIn(uint8_t slvId, uint8_t terminalId, uint8_t channelId, uint16_t *retval);

