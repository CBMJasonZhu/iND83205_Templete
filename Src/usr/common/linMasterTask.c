#include <linMasterTask.h>

#if LIN_MASTER_EN == 1

TaskState_t linmTaskState = TASK_STATE_INIT;
LIN_Device_Frame_t activelinmFrame;

uint8_t commandCount = 0;

typedef struct{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
}RGB_t;

RGB_t rgbmTable[7] = {
  {255,0,0},
  {0,255,0},
  {0,0,255},
  {255,255,0},
  {0,255,255},
  {255,0,255},
  {255,255,255},
};
uint8_t index = 0;


LIN_Device_Frame_t linm_cmd_table[] = {
    [0] = {
        .frame_id = 0x20,
        .msg_type = LIN_MSG_TYPE_TX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length   = LIN_BUFF_SIZE,
    },
};

static uint8_t red = 0, green = 0, blue = 0;
void linmSendTimerExpired(SoftTimer_t *timer)
{
    linm_cmd_table[0].data[0] = 0x01;
    linm_cmd_table[0].data[1] = 0x03;
    linm_cmd_table[0].data[2] = red;
    linm_cmd_table[0].data[3] = green;
    linm_cmd_table[0].data[4] = blue;
    linm_cmd_table[0].data[5] = 0;
    
    if (red == 0){
        red   = 5;
        green = 5;
        blue  = 5;
    }else{
        red   = 0;
        green = 0;
        blue  = 0;
    }
    
    LINM_SendFrame(&linm_cmd_table[0]);
}


SoftTimer_t linmSendTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 1000,
    .handler  = linmSendTimerExpired
};


void LIN_Master_ISR(LIN_Device_Frame_t const *frame)
{
    activelinmFrame = *frame;
    TM_PostTask(TASK_ID_LINM);
}

void hanldeCommands(LIN_Device_Frame_t *frame)
{
    // Handle received command
}

void LINM_TaskHandler(void)
{
    switch(linmTaskState){
    case TASK_STATE_ACTIVE:
      hanldeCommands(&activelinmFrame);
      break;
    case TASK_STATE_INIT:
      LINM_Init();
      LINM_RegisterLINM_IRQ(LIN_Master_ISR);
      
      SoftTimer_Start(&linmSendTimer);
      linmTaskState = TASK_STATE_ACTIVE;
      break;
    }
}

#endif
