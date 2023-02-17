#include <lib/libZeroMqComm.h>

int initZeroMq(cfgZmq * cfgZmq, char * ip_addr, int port)
{
    int err = 0;
    char strMsg[250];
    char strZmq[100];

    cfgZmq->ZmqContext = zmq_ctx_new();
    cfgZmq->ZmqSocket  = zmq_socket(cfgZmq->ZmqContext, ZMQ_REP);

    snprintf(strZmq, sizeof(strZmq), "tcp://%s:%d", ip_addr, port);
    err = zmq_bind(cfgZmq->ZmqSocket, strZmq);

    if (err)
    {
        snprintf(strMsg, sizeof(strMsg), "Failed to init ZeroMq at %s:%d. Status = %d", ip_addr, port, err);
        logTsMsg(ERR_MSG, OPER_LPATH, strMsg);
        cfgZmq->isRun = FALSE;
        return 1;
    }
    snprintf(strMsg, sizeof(strMsg), "Init ZeroMq successfully at %s:%d. Status = %d", ip_addr, port, err);
    logTsMsg(LOG_MSG, OPER_LPATH, strMsg);
    cfgZmq->isRun = TRUE;
    return 0;
}

int dnitZeroMq(cfgZmq * cfgZmq) 
{
    zmq_close(cfgZmq->ZmqSocket);
    zmq_ctx_destroy(cfgZmq->ZmqContext);
    return 0;
}


void setIntZmqVal(void *socket, char * set_var, int val) 
{
    // Create Json Obj
    json_object *jobj = json_object_new_object();

    // Create Json key:value pair
    json_object_object_add(jobj, "var", json_object_new_string(set_var));
    json_object_object_add(jobj, "val", json_object_new_int(val));

    // Convert json obj to string
    const char *json_string = json_object_to_json_string(jobj);

    #ifdef DEBUG_ZEROMQ
    char strZmq[300];
    snprintf(strZmq, sizeof(strZmq), "[0MQ] Send    = %s", json_string);
    logTsMsg(DBG_MSG, ZMQ_COMM_LPATH, strZmq);
    #endif

    // Send the string
    zmq_send(socket, json_string, strlen(json_string), 0);
    json_object_put(jobj);
}

void getIntZmqVal(void *socket, char * req_varname, int * cVar)
{
    // Setup buffer
    char buffer[BUFFER_SIZE];

    // Receiving zMq message and lock EOM char
    int recv_size = zmq_recv(socket, buffer, BUFFER_SIZE-1, 0);
    buffer[recv_size] = '\0';

    #ifdef DEBUG_ZEROMQ
    char strZmq[300];
    snprintf(strZmq, sizeof(strZmq), "[0MQ] Receive = %s", buffer);
    logTsMsg(DBG_MSG, ZMQ_COMM_LPATH, strZmq);
    #endif

    // Parse data into json obj
    json_object *jobj = json_tokener_parse(buffer);

    // Get proper key name and compare with requested key name
    const char * var  = json_object_get_string(json_object_object_get(jobj, "var"));
    if ((var != NULL) && (strcmp(req_varname, var) == 0))
    {
        if (json_object_get_type(jobj) == 4)
        {
            int val  = json_object_get_int(json_object_object_get(jobj, "val"));
            *cVar = val;
        }
    }
    json_object_put(jobj);
}

void setDoubleZmqVal(void *socket, char * set_var, double val) 
{
    // Create Json Obj
    json_object *jobj = json_object_new_object();

    // Create Json key:value pair
    json_object_object_add(jobj, "var", json_object_new_string(set_var));
    json_object_object_add(jobj, "val", json_object_new_double(val));

    // Convert json obj to string
    const char *json_string = json_object_to_json_string(jobj);

    #ifdef DEBUG_ZEROMQ
    char strZmq[300];
    snprintf(strZmq, sizeof(strZmq), "[0MQ] Send    = %s", json_string);
    logTsMsg(DBG_MSG, ZMQ_COMM_LPATH, strZmq);
    #endif

    // Send the string
    zmq_send(socket, json_string, strlen(json_string), 0);
    json_object_put(jobj);
}

void getDoubleZmqVal(void *socket, char * req_varname, double * cVar)
{
    // Setup buffer
    char buffer[BUFFER_SIZE];

    // Receiving zMq message and lock EOM char
    int recv_size = zmq_recv(socket, buffer, BUFFER_SIZE-1, 0);
    buffer[recv_size] = '\0';

    #ifdef DEBUG_ZEROMQ
    char strZmq[300];
    snprintf(strZmq, sizeof(strZmq), "[0MQ] Receive = %s", buffer);
    logTsMsg(DBG_MSG, ZMQ_COMM_LPATH, strZmq);
    #endif

    // Parse data into json obj
    json_object *jobj = json_tokener_parse(buffer);

    // Get proper key name and compare with requested key name
    const char * var  = json_object_get_string(json_object_object_get(jobj, "var"));
    if ((var != NULL) && (strcmp(req_varname, var) == 0))
    {
        if (json_object_get_type(jobj) == 3)
        {
            int val  = json_object_get_double(json_object_object_get(jobj, "val"));
            *cVar = val;
        }
    }
    json_object_put(jobj);
}