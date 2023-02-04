#include "lib/libSoemBeckhoff.h"

extern char strMsg[250];
extern char strTmp[10];
extern char IOMap[4096];

int getJsonEcatComm(cfgEcat *cfgEcat) {

   json_object *cfgEcatObj = json_object_from_file(ECAT_SOEM_CONFG);
   json_object  *cfgEcatComm, *cfgEcatPcPort, *cfgNumOfNodes;

   // Retrieve data from json data hierarchy
   json_object_object_get_ex(cfgEcatObj,   "config_comm",    &cfgEcatComm);
   json_object_object_get_ex(cfgEcatComm,  "nic_port",       &cfgEcatPcPort);
   json_object_object_get_ex(cfgEcatComm,  "node_num",       &cfgNumOfNodes);

   // Pass value to pointer var
   cfgEcat->ifname     = json_object_get_string(cfgEcatPcPort);
   cfgEcat->numOfNodes = json_object_get_int(cfgNumOfNodes);

   json_object_put(cfgEcatObj);
   json_object_put(cfgEcatComm);
   json_object_put(cfgEcatPcPort);
   json_object_put(cfgNumOfNodes);

   return 0;
}

int cfgHdwrEcatComm(cfgEcat * cfgEcat) {

   uint8_t cntRetry = ECAT_INIT_RETRY;

   // Init ECAT lib and context
   if (ec_init(cfgEcat->ifname) == 0 )
   {
      logTsMsg(ERR_MSG, ECAT_SOEM_LPATH, "Fail to load and init ECAT SOEM lib due to invalid ECAT port config");
      return 1;
   }
   logTsMsg(LOG_MSG, ECAT_SOEM_LPATH, "Load and init ECAT SOEM");

   // Init and retrieve number of ECAT slaves
   if (ec_config_init(FALSE) == 0)
   {
      logTsMsg(ERR_MSG, ECAT_SOEM_LPATH, "Fail to communication with slaves");
      return 1;      
   }

   strcpy(strMsg, "[ECAT] Number of slave(s) found and configured: ");
   snprintf(strTmp, sizeof(strTmp), "%d", ec_slavecount); strcat(strMsg, strTmp);
   logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);

   for(uint8_t i = 1; i <= ec_slavecount; i++)
   {
      strcpy(strMsg, "[ECAT] Slave Id#");
      snprintf(strTmp, sizeof(strTmp), "%d", i); strcat(strMsg, strTmp);
      strcat(strMsg, " Name=");
      strcat(strMsg, ec_slave[i].name);
      logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);
   }

   // Init IOMap
   ec_config_map(&IOMap);
   ec_configdc();

   // Set the EtherCAT network at SAFE_OP
   ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

   // Set the EtherCAT network at OP
   ec_slave[0].state = EC_STATE_OPERATIONAL;
   ec_send_processdata();  
   ec_receive_processdata(EC_TIMEOUTRET);
   ec_writestate(0);
    
   do
   {
      ec_send_processdata();
      ec_receive_processdata(EC_TIMEOUTRET);
      ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
   }
   while ((ec_slave[0].state != EC_STATE_OPERATIONAL) && cntRetry--);

   if (ec_slave[0].state != EC_STATE_OPERATIONAL)
   {
      if (cntRetry)
      logTsMsg(ERR_MSG, ECAT_SOEM_LPATH, "[ECAT] Timeout expired and ECAT has been initialized fail. Not all slave(s) in OP state");
      return 1;
   }
   logTsMsg(LOG_MSG, ECAT_SOEM_LPATH, "Load and init ECAT successfully. All slave(s) in OP state.");
   return 0;
}
