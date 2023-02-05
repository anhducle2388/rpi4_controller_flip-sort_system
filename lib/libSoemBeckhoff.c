#include "lib/libSoemBeckhoff.h"

extern char strMsg[250];
extern char strTmp[100];
extern uint8_t IOMap[4096];

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
   logTsMsg(LOG_MSG, ECAT_SOEM_LPATH, "Load and init ECAT communication");

   // Init and retrieve number of ECAT slaves
   if (ec_config_init(FALSE) == 0)
   {
      logTsMsg(ERR_MSG, ECAT_SOEM_LPATH, "Fail to communication with slave(s).");
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
   logTsMsg(LOG_MSG, ECAT_SOEM_LPATH, "Load and init ECAT communication successfully. All slave(s) in OP state");
   return 0;
}

int chkEcatDiagnosis(void) 
{
   #define EC_TIMEOUTMON 500

   if(ec_group[0].docheckstate)
   {
      /* one ore more slaves are not responding */
      ec_group[0].docheckstate = FALSE;
      ec_readstate();

      for (uint8_t i = 1; i <= ec_slavecount; i++)
      {
         if ((ec_slave[i].group == 0) && (ec_slave[i].state != EC_STATE_OPERATIONAL))
         {
            ec_group[0].docheckstate = TRUE;
            if (ec_slave[i].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
            {
               printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", i);
               ec_slave[i].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
               ec_writestate(i);
            }
            else if(ec_slave[i].state == EC_STATE_SAFE_OP)
            {
               printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", i);
               ec_slave[i].state = EC_STATE_OPERATIONAL;
               ec_writestate(i);
            }
            else if(ec_slave[i].state > EC_STATE_NONE)
            {
               if (ec_reconfig_slave(i, EC_TIMEOUTMON))
               {
                  ec_slave[i].islost = FALSE;
                  printf("MESSAGE : slave %d reconfigured\n",i);
               }
            }
            else if(!ec_slave[i].islost)
            {
               /* re-check state */
               ec_statecheck(i, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
               if (ec_slave[i].state == EC_STATE_NONE)
               {
                  ec_slave[i].islost = TRUE;
                  printf("ERROR : slave %d lost\n",i);
               }
            }
         }
         if (ec_slave[i].islost)
         {
            if(ec_slave[i].state == EC_STATE_NONE)
            {
               if (ec_recover_slave(i, EC_TIMEOUTMON))
               {
                  ec_slave[i].islost = FALSE;
                  printf("MESSAGE : slave %d recovered\n",i);
               }
            }
            else
            {
               ec_slave[i].islost = FALSE;
               printf("MESSAGE : slave %d found\n",i);
            }
         }
      }
      if(!ec_group[0].docheckstate)
         printf("OK : all slaves resumed OPERATIONAL.\n");
   }
   return 0;
}