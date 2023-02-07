#define NUM_THREADS 3

#define IO_TASK_INTERVAL_uS 1000
#define IO_TASK_ECATDIAG_us 1000
#define EXECUTE_INTERVAL_ms 10

#define OPER_LPATH   "./log/log_oper.txt"
#define EXEC_LPATH   "./log/log_exec.txt"

typedef struct {
   struct {
      uint8_t sts;
      uint8_t cmd;
   } OperationMode;
} cfgOper;

#define MODE_STOP    0
#define MODE_OPER    1
#define MODE_HOME    2
#define MODE_ERRO    3

#define CMMD_STOP    0
#define CMMD_OPER    1
#define CMMD_HOME    2

#define DEBUG_EXECUTE_INTERVAL
#define DEBUG_IO_TASK_INTERVAL