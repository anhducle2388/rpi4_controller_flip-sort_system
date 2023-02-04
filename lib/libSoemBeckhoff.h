#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ethercat.h>
#include <lib/libJsonConfig.h>
#include <lib/libLogHandler.h>

#define ECAT_SOEM_CONFG   "./dat/comm_ecat.json"
#define ECAT_SOEM_LPATH   "./log/log_comm.txt"
#define ECAT_INIT_RETRY    200

#define DO_KL2134
#define DI_KL1104
#define AO_KL4002
#define AI_KL3202

typedef struct {
   char const *ifname;
   uint8_t    numOfNodes;
} cfgEcat;

int getJsonEcatComm(cfgEcat * cfgEcat);
int cfgHdwrEcatComm(cfgEcat * cfgEcat);

int wrDigOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, boolean setval);
int rdDigOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, boolean *retval);
int rdDigIn(uint8_t slvId, uint8_t terminalId, uint8_t channelId, boolean *retval);;

int wrAnlgOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, uint16_t retval);
int rdAnlgOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, uint16_t *retval);
int rdAnlgIn(uint8_t slvId, uint8_t terminalId, uint8_t channelId, uint16_t *retval);
