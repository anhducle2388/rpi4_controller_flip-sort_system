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
   int8_t    curWkc;
   uint8_t    expectedWkc;
   uint8_t    isRun;
} cfgEcat;

int getJsonEcatComm(cfgEcat * cfgEcat);
int cfgHdwrEcatComm(cfgEcat * cfgEcat);
int chkEcatDiagnosis(cfgEcat * cfgEcat);
int getEcatIoFrame(cfgEcat * cfgEcat);

int wrDigOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, boolean setval);
int rdDigOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, boolean *retval);
int rdDigIn(uint8_t slvId, uint8_t terminalId, uint8_t channelId, boolean *retval);

int wrAnlgOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, uint16_t retval);
int rdAnlgOut(uint8_t slvId, uint8_t terminalId, uint8_t channelId, uint16_t *retval);
int rdAnlgIn(uint8_t slvId, uint8_t terminalId, uint8_t channelId, uint16_t *retval);


typedef union {
   uint8_t    data_frame : 4;
   struct {
         uint8_t channel_1 : 1;
         uint8_t channel_2 : 1;
         uint8_t channel_3 : 1;
         uint8_t channel_4 : 1;
   } ;
} do_KL2134t;

typedef union {
   uint8_t    data_frame : 4;
   struct {
         uint8_t channel_1 : 1;
         uint8_t channel_2 : 1;
         uint8_t channel_3 : 1;
         uint8_t channel_4 : 1;
   } ;
} di_KL1104t;

// Analog In KL3202 2 x 16 bit data (2 x 8 bit control/status optional)
// https://download.beckhoff.com/download/Document/io/bus-terminals/kl320xen.pdf
// 42 00 34 21 = STATUS __ BYTEH? BYTEHL?
      // STTS        == 0x42 ~ 0b 0100 0010
      // STTS.B7     -> 0: Mode: Process data exchange
      // STTS.B6     -> 1: Error
      // STTS.B5..B2 -> 
      // STTS.B1     -> 1: Overrange
      // STTS.B0     -> 1: Underrange
      // CTRL.B7     -> Mode. 0: Process data exchange
      // CTRL.B6     -> Terminal compensation function
      // CTRL.B4     -> Calibration gain
      // CTRL.B3     -> Offset calibration
      // CTRL.B2     -> 0: slower cycle = 1000 ms 1: fast cycle = 50 ms
      // CTRL.B1     -> up
      // CTRL.B0     -> down
typedef union {
   uint64_t data_frame;
   struct {
      struct {
         uint8_t  stts;
         uint8_t  rsrv;
         uint16_t data;
      } channel_1;
      struct {
         uint8_t  stts;
         uint8_t  rsrv;
         uint16_t data;
      } channel_2;
   } ;
} ai_KL3202t;

// Analog Out KL4002  2 x 16 bit data (2 x 8 bit control/status optional)
// https://download.beckhoff.com/download/Document/io/bus-terminals/kl400xen.pdf
// 00 00 00 00 = CTRL __ BYTEH? BYTEHL?
      // CTRL.B7     -> Mode. 0: Process data exchange
      // CTRL.B6     -> Terminal compensation function
      // CTRL.B4     -> Calibration gain
      // CTRL.B3     -> Offset calibration
      // CTRL.B2     -> 0: slower cycle = 1000 ms 1: fast cycle = 50 ms
      // CTRL.B1     -> up
      // CTRL.B0     -> down
      // STTS        -> No function
typedef union {
   uint64_t data_frame;
   struct {
      struct {
         uint8_t  stts;
         uint8_t  rsrv;
         uint16_t data;
      } channel_1;
      struct {
         uint8_t  stts;
         uint8_t  rsrv;
         uint16_t data;
      } channel_2;
   } ;
} ao_KL4002t;