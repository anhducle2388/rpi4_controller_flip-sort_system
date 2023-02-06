#include "lib/libSoemBeckhoff.h"

int getJsonEcatComm(cfgEcat *cfgEcat) {

   json_object *cfgEcatObj = json_object_from_file(ECAT_SOEM_CONFG);
   json_object *cfgEcatComm, *cfgEcatPcPort, *cfgNumOfNodes;

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

   cfgEcat->isRun = FALSE;

   return 0;
}

int cfgHdwrEcatComm(cfgEcat * cfgEcat) {

   uint8_t cntRetry = ECAT_INIT_RETRY;
   char    strMsg[250];

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

   snprintf(strMsg, sizeof(strMsg), "[ECAT] Number of slave(s) found and configured: %d", ec_slavecount);
   logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);

   for(uint8_t i = 1; i <= ec_slavecount; i++)
   {
      snprintf(strMsg, sizeof(strMsg), "[ECAT] Slave Id#%d Name=%s", i, ec_slave[i].name);
      logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);
   }

   // Init IOMap
   ec_config_map(cfgEcat->IOMap);
   ec_configdc();

   // Set the EtherCAT network at SAFE_OP
   ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

   cfgEcat->expectedWkc = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
   snprintf(strMsg, sizeof(strMsg), "Expected working counter Wkc of ECAT: %d",  cfgEcat->expectedWkc);
   logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);

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
   cfgEcat->isRun = TRUE;
   logTsMsg(LOG_MSG, ECAT_SOEM_LPATH, "Load and init ECAT communication successfully. All slave(s) in OP state");
   return 0;
}

int chkEcatDiagnosis(cfgEcat * cfgEcat) 
{
   #define EC_TIMEOUTMON 500
   char strMsg[250];
   static preWkc = 0;

   if ( ((ec_group[0].docheckstate) || (cfgEcat->curWkc < cfgEcat->expectedWkc)) && (cfgEcat->isRun == TRUE) )
   {
      if (preWkc != cfgEcat->curWkc) {
         snprintf(strMsg, sizeof(strMsg), "[ECAT] One ore more slaves are not responding. Wkc = %d, Expected Wkc = %d", cfgEcat->curWkc, cfgEcat->expectedWkc);
         logTsMsg(ERR_MSG, OPER_LPATH, strMsg);
      }

      ec_group[0].docheckstate = FALSE;
      ec_readstate();

      for (uint8_t i = 1; i <= ec_slavecount; i++)
      {
         if ((ec_slave[i].group == 0) && (ec_slave[i].state != EC_STATE_OPERATIONAL))
         {
            ec_group[0].docheckstate = TRUE;
            if (ec_slave[i].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
            {
               snprintf(strMsg, sizeof(strMsg), "[ECAT] Slave %d %s is in SAFE_OP + ERROR, attempting ack", i, ec_slave[i].name);
               logTsMsg(ERR_MSG, ECAT_SOEM_LPATH, strMsg);
               ec_slave[i].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
               ec_writestate(i);
            }
            else if(ec_slave[i].state == EC_STATE_SAFE_OP)
            {
               snprintf(strMsg, sizeof(strMsg), "[ECAT] Slave %d %s is in SAFE_OP, change to OPERATIONAL", i, ec_slave[i].name);
               logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);
               ec_slave[i].state = EC_STATE_OPERATIONAL;
               ec_writestate(i);
            }
            else if(ec_slave[i].state > EC_STATE_NONE)
            {
               if (ec_reconfig_slave(i, EC_TIMEOUTMON))
               {
                  ec_slave[i].islost = FALSE;
                  snprintf(strMsg, sizeof(strMsg), "[ECAT] Slave %d %s reconfigured", i, ec_slave[i].name);
                  logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);
               }
            }
            else if(!ec_slave[i].islost)
            {
               /* re-check state */
               ec_statecheck(i, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
               if (ec_slave[i].state == EC_STATE_NONE)
               {
                  ec_slave[i].islost = TRUE;
                  snprintf(strMsg, sizeof(strMsg), "[ECAT] Slave %d %s lost", i, ec_slave[i].name);
                  logTsMsg(ERR_MSG, ECAT_SOEM_LPATH, strMsg);
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
                  snprintf(strMsg, sizeof(strMsg), "[ECAT] Slave %d %s recovered", i, ec_slave[i].name);
                  logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);
               }
            }
            else
            {
               ec_slave[i].islost = FALSE;
               snprintf(strMsg, sizeof(strMsg), "[ECAT] Slave %d %s found", i, ec_slave[i].name);
               logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);
            }
         }
      }
      if(!ec_group[0].docheckstate)
      {        
         snprintf(strMsg, sizeof(strMsg), "[ECAT] All slaves resumed OPERATIONAL");
         logTsMsg(LOG_MSG, ECAT_SOEM_LPATH, strMsg);
      }
   }
   preWkc = cfgEcat->curWkc;
   return 0;
}

int getEcatIoFrame(cfgEcat * cfgEcat) 
{
   for (uint8_t i = 1; i <= ec_slavecount; i++)
   {
      uint8_t oloop = ec_slave[i].Obytes;
      uint8_t iloop = ec_slave[i].Ibytes;
      char    strMsg[500], strTmp[100];

      if (( cfgEcat->curWkc >= cfgEcat->expectedWkc) && ((oloop > 0) || (iloop > 0)))
      {
         snprintf(strMsg, sizeof(strMsg), "[id=%d] [slv=%s] [O=]", i, ec_slave[i].name);
         for(uint8_t j = 0; j < oloop; j++)
         {
            snprintf(strTmp, sizeof(strTmp), "%3.2x", ec_slave[i].outputs[j]);
            strcat(strMsg, strTmp);
         }

         strcat(strMsg, " [I=]");
         for(uint8_t j = 0; j < iloop; j++)
         {
            snprintf(strTmp, sizeof(strTmp), "%3.2x", ec_slave[i].inputs[j]);
            strcat(strMsg, strTmp);
         }
         logTsMsg(DBG_MSG, ECAT_SOEM_LPATH, strMsg);
      }  
   }
   return 0;
}
