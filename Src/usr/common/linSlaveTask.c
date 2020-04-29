#include <linSlaveTask.h>

#define LINS_SIGNAL_IDLE           (1U << 0U)
#define LINS_SIGNAL_LIN_BUS_ACTIVE (1U << 1U)
#define LINS_SIGNAL_DATE_REQ       (1U << 2U)

static uint8_t autoAddressingSTA = AUTO_ADDRESSING_IDLE;
static uint8_t lins_handle_id = LINS_SIGNAL_IDLE;

static TaskState_t        linsTaskState = TASK_STATE_INIT;
static LIN_Device_Frame_t activelinsFrame;

void hanldeLinCommands(LIN_Device_Frame_t *frame);
void LIN_Slave_ISR(LIN_Device_Frame_t const *frame);

#if ENABLE_LIN_BUS_IDLE_TIMEOUT_SLEEP == 1U
void linsBusTimeoutTimerExpired(SoftTimer_t *timer);

static SoftTimer_t linsBusTimeoutTimer = {
    .mode     = TIMER_ONE_SHOT_MODE,
    .interval = 4000U,
    .handler  = linsBusTimeoutTimerExpired
};
#endif

void LINS_AutoAddressingSwitchOff(void)
{
  IOCTRLA_SFRS->LIN.SWON = 0U;
  IOCTRLA_SFRS->LIN.DS_SWON = 0U;
}

//LININ LINOUT 打开
void LINS_AutoAddressingSwitchOn(void)
{
  IOCTRLA_SFRS->LIN.SWON = 1U;
  IOCTRLA_SFRS->LIN.DS_SWON = 0U;
}


LIN_Device_Frame_t lins_cmd_table[] = {
    [0] = {
        .frame_id = 0x3B,
        .msg_type = LIN_MSG_TYPE_RX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length   = LIN_BUFF_SIZE,
    },
    [1] = {
        .frame_id = 0x3C,
        .msg_type = LIN_MSG_TYPE_RX,
        .checksum = LIN_CHECKSUM_CLASSIC,
        .length   = LIN_BUFF_SIZE,
    },
    [2] = {
        .frame_id = 0x3D,
        .msg_type = LIN_MSG_TYPE_TX,
        .checksum = LIN_CHECKSUM_CLASSIC,
        .length   = LIN_BUFF_SIZE,
    },
    [3] = {
        .frame_id = 0x11,
        .msg_type = LIN_MSG_TYPE_TX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length   = 2,
    },
    [4] = {
        .frame_id = 0x02,
        .msg_type = LIN_MSG_TYPE_RX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length   = LIN_BUFF_SIZE,
    },
    [5] = {
        .frame_id = 0x03,
        .msg_type = LIN_MSG_TYPE_RX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length   = LIN_BUFF_SIZE,
    },
    [5] = {
        .frame_id = 0x06,
        .msg_type = LIN_MSG_TYPE_RX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length   = LIN_BUFF_SIZE,
    }
};

void LIN_Slave_ISR(LIN_Device_Frame_t const *frame)
{
    activelinsFrame = *frame;
    lins_handle_id |= LINS_SIGNAL_DATE_REQ;
    TM_PostTask(TASK_ID_LINS);
}

#define NAD_ADDR SAVED_NAD
void hanldeLinCommands(LIN_Device_Frame_t *frame)
{
    IntensityRatio_t iRatio = {255U,255U,255U};
    int8_t temperature;
    uint8_t on = FALSE;
    if(frame->frame_id == 0x3CU)//诊断帧
    {
      if((frame->data[0] == 0x7F) && (frame->data[1] == 0x06))//广播
      {
        if(frame->data[2] == 0xB5)//节点配置
        {
          uint8_t FunctionID0 = frame->data[5];
          switch(FunctionID0)
          {
          case 0x01://自动寻址初始化
            if(autoAddressingSTA == AUTO_ADDRESSING_IDLE)
            {
              autoAddressingSTA = AUTO_ADDRESSING_INIT;
              LINS_AutoAddressingSwitchOff();//这里关闭LININ LINOUT 开关
            }
            break;
          case 0x02://重新分配NAD
            if(autoAddressingSTA == AUTO_ADDRESSING_INIT)
            {
              autoAddressingSTA = AUTO_ADDRESSING_NAD;
              NAD_ADDR = frame->data[7];
            }
            break;
          case 0x03://保存NAD
            if(autoAddressingSTA == AUTO_ADDRESSING_NAD)
            {
              autoAddressingSTA = AUTO_ADDRESSING_STORE;
              //保存到flash
              Flash_devCopyToFlash(User_Param,USER_PARAM_ADDRESS_START,USER_PARAM_SIZE);
            }
            break;
          case 0x04://NAD配置完成
            if(autoAddressingSTA == AUTO_ADDRESSING_STORE)
            {
              autoAddressingSTA = AUTO_ADDRESSING_END;
              //这里打开LININ LINOUT开关
              LINS_AutoAddressingSwitchOn();
            }
            break;
          default:
            break; 
          }
        }
      }
      else if (frame->data[0] == NAD_ADDR)//诊断信息
      {
          
          DignoseRequest_t *req = (DignoseRequest_t *)frame->data;
          switch(req->command)
          {
          case DIGNOSE_CMD_SET_FRAME_ID:
            {
                if(req->service == 0xB7)
                {
                  lins_cmd_table[3].frame_id = req->data2;//更新frame ID信息
                  SAVED_PID = req->data2;
                  Flash_devCopyToFlash(User_Param,USER_PARAM_ADDRESS_START,USER_PARAM_SIZE);
                  lins_cmd_table[2].responseValid = true;
                  GetIDResponse_t *rsp = (GetIDResponse_t *)lins_cmd_table[2].data;
                  rsp->command = req->command;
                  rsp->nodeAddress = req->nodeAddress;
                  rsp->rservice = 0xF7;
                  rsp->unused1  = 0xFF;
                  rsp->unused2  = 0xFF;
                  rsp->unused3  = 0xFF;
                  rsp->unused4  = req->data2;//debug
                  rsp->unused5  = 0xFF;
                }
                break;
            }
          default:
            break;
          }
          /*switch(req->command){
          case DIGNOSE_CMD_GET_PN_VOLT:
            {
                lins_cmd_table[2].responseValid = true;
                GetLedPNVolt_t *rsp = (GetLedPNVolt_t *)lins_cmd_table[2].data;
                rsp->command = req->command;
                rsp->nodeAddress = NAD_ADDR;
                MES_MnfGetLedPNVoltage(&rsp->pnVolt[0],&rsp->pnVolt[1],&rsp->pnVolt[2]);
                break;
            }
          case DIGNOSE_CMD_GET_PN_TEMP:
            {
                lins_cmd_table[2].responseValid = true;
                GetLedPNTemp_t *rsp = (GetLedPNTemp_t *)lins_cmd_table[2].data;
                rsp->command = req->command;
                rsp->nodeAddress = NAD_ADDR;
                MES_MnfGetLedChannelTemperature(&rsp->pnTemperature[0],&rsp->pnTemperature[1],&rsp->pnTemperature[2]);
                break;
            }
            
          case DIGNOSE_CMD_GET_BATT_VOLT:
            {
                lins_cmd_table[2].responseValid = true;
                GetLedBattVolt_t *rsp = (GetLedBattVolt_t *)lins_cmd_table[2].data;
                rsp->command = req->command;
                rsp->nodeAddress = NAD_ADDR;
                MES_GetBatteryVolt(&rsp->volt);
                break;
            }
          case DIGNOSE_CMD_GET_CHIP_TEMP:
            {
                lins_cmd_table[2].responseValid = true;
                GetLedChipTemp_t *rsp = (GetLedChipTemp_t *)lins_cmd_table[2].data;
                rsp->command = req->command;
                rsp->nodeAddress = NAD_ADDR;
                MES_GetChipTemperature(&rsp->temperature);
                break;
            }
          case DIGNOSE_CMD_GET_TEST_PN_VOLT:
            {
                lins_cmd_table[2].responseValid = true;
                GetLedPNVolt_t *rsp = (GetLedPNVolt_t *)lins_cmd_table[2].data;
                rsp->command = req->command;
                rsp->nodeAddress = NAD_ADDR;
                MES_MnftGetLedPNVolt(&rsp->pnVolt[0], &rsp->pnVolt[1],&rsp->pnVolt[2]);
                break;
            }
          case DIGNOSE_CMD_GET_TEST_PN_TEMP:
            {
                lins_cmd_table[2].responseValid = true;
                GetLedPNTemp_t *rsp = (GetLedPNTemp_t *)lins_cmd_table[2].data;
                rsp->command = req->command;
                rsp->nodeAddress = NAD_ADDR;
                MES_MnftGetLedPNTemperature(&rsp->pnTemperature[0],&rsp->pnTemperature[1],&rsp->pnTemperature[2]);
                break;
            }
          case DIGNOSE_CMD_SET_COLOR_RED_PARAM:
            {
                ConfigLedParam_t *param = (ConfigLedParam_t *)frame->data;
                (void)CLM_SetLedPhyParams(LED_RED,   ROOM_TEMPERATURE, &param->color, param->typicalIntensity);
                break;
            }
          case DIGNOSE_CMD_GET_COLOR_RED_PARAM:
            {
                lins_cmd_table[2].responseValid = true;
                ConfigLedParam_t *param = (ConfigLedParam_t *)lins_cmd_table[2].data;
                param->command = req->command;
                param->nodeAddress = NAD_ADDR;
                (void)CLM_GetLedPhyParams(LED_RED, ROOM_TEMPERATURE, &param->color, &param->typicalIntensity);
                break;
            }
          case DIGNOSE_CMD_SET_COLOR_GREEN_PARAM:
            {
                ConfigLedParam_t *param = (ConfigLedParam_t *)frame->data;
                (void)CLM_SetLedPhyParams(LED_GREEN, ROOM_TEMPERATURE, &param->color, param->typicalIntensity);
                break;
            }
          case DIGNOSE_CMD_GET_COLOR_GREEN_PARAM:
            {
                lins_cmd_table[2].responseValid = true;
                ConfigLedParam_t *param = (ConfigLedParam_t *)lins_cmd_table[2].data;
                param->command = req->command;
                param->nodeAddress = NAD_ADDR;
                (void)CLM_GetLedPhyParams(LED_GREEN, ROOM_TEMPERATURE, &param->color, &param->typicalIntensity);
                break;
            }
          case DIGNOSE_CMD_SET_COLOR_BLUE_PARAM:
            {
                ConfigLedParam_t *param = (ConfigLedParam_t *)frame->data;
                (void)CLM_SetLedPhyParams(LED_BLUE,  ROOM_TEMPERATURE, &param->color, param->typicalIntensity);
                break;
            }
          case DIGNOSE_CMD_GET_COLOR_BLUE_PARAM:
            {
                lins_cmd_table[2].responseValid = true;
                ConfigLedParam_t *param = (ConfigLedParam_t *)lins_cmd_table[2].data;
                param->command = req->command;
                param->nodeAddress = NAD_ADDR;
                (void)CLM_GetLedPhyParams(LED_BLUE, ROOM_TEMPERATURE, &param->color, &param->typicalIntensity);
                break;
            }
          case DIGNOSE_CMD_SET_COLOR_WHITE_PARAM:
            {
                ConfigLedParam_t *param = (ConfigLedParam_t *)frame->data;
                (void)CLM_SetWhitePointParams(&param->color);
                break;
            }
          case DIGNOSE_CMD_GET_COLOR_WHITE_PARAM:
            {
                lins_cmd_table[2].responseValid = true;
                ConfigLedParam_t *param = (ConfigLedParam_t *)lins_cmd_table[2].data;
                param->command = req->command;
                param->nodeAddress = NAD_ADDR;
                (void)CLM_GetWhitePointParams(&param->color);
                break;
            }
          case DIGNOSE_CMD_SET_COLOR_RGB_MIN_INTENSITY:
            {
                ConfigMinLedIntensity_t *param = (ConfigMinLedIntensity_t *)frame->data;
                (void)CLM_SetMinimumIntensity(param->intensity[LED_RED], param->intensity[LED_GREEN], param->intensity[LED_BLUE]);
                break;
            }
          case DIGNOSE_CMD_GET_COLOR_RGB_MIN_INTENSITY:
            {
                lins_cmd_table[2].responseValid = true;
                ConfigMinLedIntensity_t *param = (ConfigMinLedIntensity_t *)lins_cmd_table[2].data;
                param->command = req->command;
                param->nodeAddress = NAD_ADDR;
                (void)CLM_GetMinimumIntensity(&param->intensity[LED_RED], &param->intensity[LED_GREEN], &param->intensity[LED_BLUE]);
                break;
            }
          case DIGNOSE_CMD_SET_PNVOLT_RGB:
            {
                ConfigTypicalLedPNVolt_t *param = (ConfigTypicalLedPNVolt_t *)frame->data;
                (void)CLM_SetLedPNVolts(ROOM_TEMPERATURE, param->pnVolt[LED_RED], param->pnVolt[LED_GREEN], param->pnVolt[LED_BLUE]);
                break;
            }
          case DIGNOSE_CMD_GET_PNVOLT_RGB:
            {
                lins_cmd_table[2].responseValid = true;
                ConfigTypicalLedPNVolt_t *param = (ConfigTypicalLedPNVolt_t *)lins_cmd_table[2].data;
                param->command = req->command;
                param->nodeAddress = NAD_ADDR;
                (void)CLM_GetLedPNVolts(ROOM_TEMPERATURE, &param->pnVolt[LED_RED], &param->pnVolt[LED_GREEN], &param->pnVolt[LED_BLUE]);
                break;
            }
          case DIGNOSE_CMD_SAVE_COLOR_PARAM:
            {
                (void)CLM_StoragColorParams();
                break;
            }
          default:
            break;
          }*/
          
      }
    }
    //RGB0 RGB1 RGB2cmd
    if(frame->frame_id == 0x02){
          Color_CMD_Frame_t *color = (Color_CMD_Frame_t *)((void *)frame->data);
          if(NAD_ADDR == 0x01)//第一个rgb
          {
            rgbs.target_color = (color->data[0])&0x0F;
            rgbs.target_type = ((color->data[1])&0x30)>>4;
            rgbs.target_time = ((color->data[2])&0x7C)>>2;
            rgbs.target_level = (color->data[5]<<2);
          }
          else if(NAD_ADDR == 0x02)//第二个rgb
          {
            rgbs.target_color = ((color->data[0])&0xF0)>>4;
            rgbs.target_type = ((color->data[1])&0xC0)>>6;//更新type
            rgbs.target_time = (color->data[3])&0x1F;//N/100ms            
            rgbs.target_level = color->data[6];
          }
          else if(NAD_ADDR == 0x03)//第三个rgb
          {
            rgbs.target_color = (color->data[1])&0x0F;
            rgbs.target_type = (color->data[2])&0x03;
            rgbs.target_time = (color->data[4])&0x1F;
            rgbs.target_level = color->data[7];
          }
            
            
        /*Color_CMD_Frame_t *color = (Color_CMD_Frame_t *)((void *)frame->data);
        if ((color->nodeAddress == NAD_ADDR) || (color->nodeAddress == NODE_ADDR_BROADCAST)){
            switch(color->command){
            case COLOR_CMD_XYY:
              on  = (color->ColorXY.Y > 0)? TRUE:FALSE;
              temperature = MES_GetLedTemperature(on);
              (void)CLM_SetAccurateXYY(iRatio, ROOM_TEMPERATURE, color->ColorXY.x, color->ColorXY.y, color->ColorXY.Y, (uint16_t)color->ColorXY.transitionTime*100U);
              break;
            case COLOR_CMD_HSL:
              on  = (color->ColorHSL.level > 0)? TRUE:FALSE;
              temperature = MES_GetLedTemperature(on);
              (void)CLM_SetHSL(temperature, color->ColorHSL.hue, color->ColorHSL.saturation, (uint16_t)color->ColorHSL.level << 2U, (uint16_t)color->ColorHSL.transitionTime*100U);
              break;
            case COLOR_CMD_RGBL:
              on  = (color->ColorRGBL.level > 0)? TRUE:FALSE;
              temperature = MES_GetLedTemperature(on);
              (void)CLM_SetRGBL(temperature, color->ColorRGBL.red, color->ColorRGBL.green, color->ColorRGBL.blue,(uint16_t)color->ColorRGBL.level << 2U, (uint16_t)color->ColorRGBL.transitionTime*100U);
              break;
            case COLOR_CMD_RGB:
              if ( (color->ColorRGB.green != 0U) || (color->ColorRGB.green) || (color->ColorRGB.blue) ){
                  on  = TRUE;
              }
              temperature = MES_GetLedTemperature(on);
              (void)CLM_SetRGB(temperature, color->ColorRGB.red, color->ColorRGB.green, color->ColorRGB.blue, (uint16_t)color->ColorRGB.transitionTime*100U);
              break;
            case COLOR_CMD_PWM:
              (void)CLM_SetPWMs((uint16_t)color->ColorPWM.PWM_Red<< 8, (uint16_t)color->ColorPWM.PWM_Green<< 8U, (uint16_t)color->ColorPWM.PWM_Blue << 8U,(uint16_t)color->ColorPWM.transitionTime*100U);
              break;
            default:
              break;
            }
        }*/
    }
    if(frame->frame_id == 0x03){
          Color_CMD_Frame_t *color = (Color_CMD_Frame_t *)((void *)frame->data);
          if(NAD_ADDR == 0x04)//第四个rgb
          {
            rgbs.target_color = (color->data[0])&0x0F;
            rgbs.target_type = (color->data[1])&0x03;
            rgbs.target_time = (color->data[2])&0x1F;
            rgbs.target_level = color->data[5];
          }           
    }
    if(frame->frame_id == 0x06)//更新START flag  or debug调试颜色用
    {
        if(frame->data[0] == NAD_ADDR)//客户debug颜色
        {
          rgbs.target_time = 1U;
          uint16_t luX;
          uint16_t luY;
          luX = (frame->data[4]<<8)+frame->data[5];
          luY = (frame->data[6]<<8)+frame->data[7];
          temperature = MES_GetLedTemperature(on);
          CLM_SetXYY(temperature, luX, luY, 1023U,0U);   
        }else
        {
          if(frame->data[2]&0x80){
              if((rgbs.transition_start&0x80) == 0x00)
                  rgbs.update_flag = 1U;
          }else{
             if(rgbs.transition_start&0x80)
                  rgbs.update_flag = 1U;         
          }
          rgbs.transition_start =  frame->data[2]&0x80;
        }
    }
}
#if ENABLE_LIN_BUS_IDLE_TIMEOUT_SLEEP == 1U
void linsBusTimeoutTimerExpired(SoftTimer_t *timer)
{
    PMU_EnterToDeepSleepMode(); /*  enter deep sleep mode if lin bus is inactive for 4s */
}

void LIN_Slave_Active_ISR(void)
{
    lins_handle_id |= LINS_SIGNAL_LIN_BUS_ACTIVE;
    TM_PostTask(TASK_ID_LINS);
}
#endif
void LINS_TaskHandler(void)
{
    switch(linsTaskState){
    case TASK_STATE_ACTIVE:
#if ENABLE_LIN_BUS_IDLE_TIMEOUT_SLEEP == 1U
      if ((lins_handle_id & LINS_SIGNAL_LIN_BUS_ACTIVE) == LINS_SIGNAL_LIN_BUS_ACTIVE){
          lins_handle_id &= (~LINS_SIGNAL_LIN_BUS_ACTIVE);
          SoftTimer_Stop(&linsBusTimeoutTimer);
          SoftTimer_Start(&linsBusTimeoutTimer);
      }
#endif
      if ((lins_handle_id & LINS_SIGNAL_DATE_REQ) == LINS_SIGNAL_DATE_REQ){
          lins_handle_id &=(~LINS_SIGNAL_DATE_REQ);
          hanldeLinCommands(&activelinsFrame);
      }
      break;
    case TASK_STATE_INIT:
      LINS_Init();
      LINS_RegisterLINS_IRQ(LIN_Slave_ISR);
#if ENABLE_LIN_BUS_IDLE_TIMEOUT_SLEEP == 1U
      LINS_RegisterLINS_Active_IRQ(LIN_Slave_Active_ISR);
#endif
      lins_cmd_table[3].frame_id = SAVED_PID;
      LINS_RegisterServices(lins_cmd_table, (uint8_t)(sizeof(lins_cmd_table)/sizeof(LIN_Device_Frame_t)));
      linsTaskState = TASK_STATE_ACTIVE;
      break;
    default:
      break;
    }
}

uint8_t LINS_GetNAD(void)
{
    return 1U;
}

void LINS_SetNAD(uint8_t nad)
{
    
    
}