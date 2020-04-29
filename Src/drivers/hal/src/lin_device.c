/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file lin_device.c
 */

#include <stdint.h>
#include <string.h>

#include <appConfig.h>
#include "realplum.h"
#include "hwcfg.h"
#include "linstack.h"
#include "timer_device.h"
#include "errno.h"
#include <isrfuncs.h>

static lin_active_cb_func_t  lins_active_callback = NULL;
static lin_cb_func_t lins_callback = NULL;
static lin_cb_func_t linm_callback = NULL;
static uint8_t deviceFrameTableSize = 0U;
static LIN_Device_Frame_t *deviceFrameTable = NULL;

typedef struct {
    uint16_t divider;
    uint16_t multiplier;
    uint8_t prescale;
} lin_speed_setting_t;

/**********************************************************************  
 *    Local definitions
 *********************************************************************/   

/**
 * At 16MHz clock, the clock divider settings for each LIN speed.
 */

#if SYS_MAIN_CLOCK_DIV != CLOCK_DIV_4
#error MAIN_CPU_CLOCK MUST BE at 4MHz system colck!
#endif

#if SYS_MAIN_CLOCK_DIV == CLOCK_DIV_1   /* 16MHz */
  static const lin_speed_setting_t lins_speed_map[E_LIN_BAUDRATE_MAX + 1U] = {
      [E_LIN_BAUDRATE_9600]  = {.divider = 200U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_19200] = {.divider = 200U, .multiplier = 0U, .prescale = 1U},
  };
  static const lin_speed_setting_t linm_speed_map[E_LIN_BAUDRATE_MAX + 1U] = {
      [E_LIN_BAUDRATE_9600]  = {.divider = 200U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_19200] = {.divider = 200U, .multiplier = 0U, .prescale = 1U},
  };
#elif SYS_MAIN_CLOCK_DIV == CLOCK_DIV_2 /* 8MHz */
  static const lin_speed_setting_t lins_speed_map[E_LIN_BAUDRATE_MAX + 1U] = {
      [E_LIN_BAUDRATE_9600]  = {.divider = 417U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_19200] = {.divider = 200U, .multiplier = 0U, .prescale = 0U},
  };
  static const lin_speed_setting_t linm_speed_map[E_LIN_BAUDRATE_MAX + 1U] = {
      [E_LIN_BAUDRATE_9600]  = {.divider = 417U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_19200] = {.divider = 200U, .multiplier = 0U, .prescale = 0U},
  };
#elif SYS_MAIN_CLOCK_DIV == CLOCK_DIV_4 /* 4MHz */
  static const lin_speed_setting_t lins_speed_map[E_LIN_BAUDRATE_MAX + 1U] = {
      [E_LIN_BAUDRATE_9600]  = {.divider = 208U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_19200] = {.divider = 100U, .multiplier = 0U, .prescale = 0U},
  };
  static const lin_speed_setting_t linm_speed_map[E_LIN_BAUDRATE_MAX + 1U] = {
      [E_LIN_BAUDRATE_9600]  = {.divider = 208U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_19200] = {.divider = 100U, .multiplier = 0U, .prescale = 0U},
  };
#else   /* 2MHz */
  static const lin_speed_setting_t lins_speed_map[E_LIN_BAUDRATE_MAX + 1U] = {
      [E_LIN_BAUDRATE_9600]  = {.divider = 104U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_19200] = {.divider = 50U, .multiplier = 0U, .prescale = 0U},
  };
  static const lin_speed_setting_t linm_speed_map[E_LIN_BAUDRATE_MAX + 1U] = {
      [E_LIN_BAUDRATE_9600]  = {.divider = 104U, .multiplier = 0U, .prescale = 0U},
      [E_LIN_BAUDRATE_19200] = {.divider = 50U, .multiplier = 0U, .prescale = 0U},
  };
#endif

void lins_SetTiming(LIN_BaudRate_t BaudRate);
int8_t LINS_WriteBuffer(uint8_t buff[], uint32_t dataLength);
int8_t LINS_ReadBuffer(uint8_t *buff, uint8_t dataLength);
int8_t is_valid_frame(LIN_Device_Frame_t *frameTable, uint8_t id);


int8_t LINM_WriteBuffer(uint8_t buff[], uint32_t dataLength);
int8_t LINM_ReadBuffer(uint8_t buff[], uint32_t dataLength);
void linm_SetTiming(LIN_BaudRate_t BaudRate);

 void lins_SetTiming(LIN_BaudRate_t BaudRate)
{
    LINS_SFRS->BTDIV07          = (uint32_t)lins_speed_map[BaudRate].divider & 0xFFU;
    LINS_SFRS->BITTIME.BTDIV8   = (uint32_t)lins_speed_map[BaudRate].divider >> 8U;
    LINS_SFRS->BITTIME.PRESCL   = (uint32_t)lins_speed_map[BaudRate].prescale;
    
    /*configure the Bit Rate for 19200Hz */
/*    LINS_SFRS->BTDIV07 = 0xC8;  
    LINS_SFRS->BITTIME.BTDIV8 = 0x0;  
    LINS_SFRS->BITTIME.PRESCL = 0x1; 
*/
}


int8_t LINS_WriteBuffer(uint8_t buff[], uint32_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            LINS_SFRS->DATABUFF[i].DATA = (uint32_t)buff[i];
        }
    }
    return result;
}


int8_t LINS_ReadBuffer(uint8_t *buff, uint8_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            buff[i] = (uint8_t)LINS_SFRS->DATABUFF[i].DATA;
        }
    }
    return result;
}


void LINS_Init(void)
{
    /* Config GPIO to LIN mode, enable transmission */
    IOCTRLA_SFRS->LIN.LINS_HWMODE       = 1U;     /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
    IOCTRLA_SFRS->LIN.LINS_RXENA        = 1U;     /* LIN receive enable. */
    IOCTRLA_SFRS->LIN.LINS_TXENA        = 1U;     /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINS_PU30K_ENA    = 1U;     /* LIN 30K pullup enable.*/
    
    IOCTRLA_SFRS->LIN.SWON    = 1U;      /* 1: disable Master function, 0: enable Master function on LINOUT pin */
    IOCTRLA_SFRS->LIN.DS_SWON = 0U;      /* 1: LIN Downstream Off, 0: LIN Downstream On */
  
    LINS_SFRS->BUSTIME.BUSINACTIVE      = E_LIN_TIME_INACTIVE_MAX;
    LINS_SFRS->BUSTIME.WUPREPEAT        = E_LIN_TIME_WAKEUP_REPEAT_MS_180;
    LINS_SFRS->CTRL.RSTERR              = 1U;    /* reset error */
    LINS_SFRS->CTRL.RSTINT              = 1U;    /* reset interrupt */
    
    lins_SetTiming(E_LIN_BAUDRATE_19200);       /* set the maximum supported baud rate */

}

void LINS_SwitchDownstreamTo(DownSwitchState_t state)
{
   IOCTRLA_SFRS->LIN.SWON = (uint32_t)state;
}


void LINS_RegisterServices(LIN_Device_Frame_t *lin_cmd_table, uint8_t size)
{
    deviceFrameTable = lin_cmd_table;
    deviceFrameTableSize = size;
}

void LINS_RegisterLINS_IRQ(lin_cb_func_t cb)
{
    lins_callback = cb;
    NVIC_EnableIRQ(LIN_IRQn);
}


void LINS_UnRegisterLINS_IRQ(void)
{
    lins_callback = NULL;
    NVIC_DisableIRQ(LIN_IRQn);
}

void LINS_RegisterLINS_Active_IRQ(lin_active_cb_func_t cb)
{
    lins_active_callback = cb;
    NVIC_EnableIRQ(LIN_IRQn);
}

/**
 * @brief Check if the message id is in the frame list
 *
 * @param dev The pointer to LIN device.
 * @param id The ID of received frame.
 * @return the index of the ID the the supported frame list, or -1 if not supported.
 */
int8_t is_valid_frame(LIN_Device_Frame_t *frameTable, uint8_t id)
{
    int8_t index = -1;
    if ((id == 0x00U) || (id == 0xFFU)){
        index = -1;
    }else{
        for (uint8_t i = 0U; i < deviceFrameTableSize; i++){
            if (frameTable[i].frame_id == id){
                  index = (int8_t)i;
                  break;
            }
        }
    }
    return index;
}

void LINS_Handler(void)
{
    static int8_t index = 0;
    uint8_t status = (uint8_t)LINS_STATUS_REG;

    if (lins_active_callback != NULL){ /* lin bus is active */
        lins_active_callback();
    }
    
    if ((status & E_LIN_STATUS_DATA_REQ) != 0U){
        index = is_valid_frame(deviceFrameTable, (uint8_t)LINS_SFRS->ID);
        if (index >= 0){
            if (deviceFrameTable[index].msg_type == LIN_MSG_TYPE_RX){
                LINS_SFRS->CTRL.TRANSMIT = 0U;/* it's a data receive operation */
                LINS_SFRS->DL.LENGTH = deviceFrameTable[index].length;
                LINS_SFRS->DL.ENHCHK = deviceFrameTable[index].checksum;
                LINS_SFRS->CTRL.DATAACK = 1U;
            }else{
                if(deviceFrameTable[index].frame_id == 0x3D)
                {
                  if (deviceFrameTable[index].responseValid != 0U){
                      deviceFrameTable[index].responseValid = false;
                      LINS_SFRS->CTRL.TRANSMIT = 1U;/* it's data request operation */
                      LINS_SFRS->DL.LENGTH = deviceFrameTable[index].length;
                      LINS_SFRS->DL.ENHCHK = deviceFrameTable[index].checksum;
                      (void)LINS_WriteBuffer(deviceFrameTable[index].data, (uint32_t)deviceFrameTable[index].length);
                      LINS_SFRS->CTRL.DATAACK = 1U;
                  }                 
                }
                else
                {
                  LINS_SFRS->CTRL.TRANSMIT = 1U;/* it's data request operation */
                  LINS_SFRS->DL.LENGTH = deviceFrameTable[index].length;
                  LINS_SFRS->DL.ENHCHK = deviceFrameTable[index].checksum;
                  (void)LINS_WriteBuffer(deviceFrameTable[index].data, (uint32_t)deviceFrameTable[index].length);
                  LINS_SFRS->CTRL.DATAACK = 1U;                           
                }
            }
        }else{
            LINS_SFRS->CTRL.STOP = 1U;
        }
    }else if ((status & (uint8_t)E_LIN_STATUS_ERROR) != 0U){
    }else if ((status & (uint8_t)E_LIN_STATUS_IDLE_TIMEOUT) != 0U){
        /* LINS_SFRS->CTRL.SLEEP = 1U; */
    }else if ((status & (uint8_t)E_LIN_STATUS_WAKEUP) != 0U){
        
    }else if ((status & (uint8_t)E_LIN_STATUS_COMPLETE) != 0U){/*receive finished */
        (void)LINS_ReadBuffer(deviceFrameTable[index].data, deviceFrameTable[index].length);
        if(lins_callback != NULL){
            lins_callback(&deviceFrameTable[index]);
        }
    }else{
      
    }
    LINS_SFRS->CTRL.RSTERR  = 1U;    /*reset error */
    LINS_SFRS->CTRL.RSTINT  = 1U;    /*reset interrupt */
}

static LIN_Device_Frame_t linmFrame;


int8_t LINM_WriteBuffer(uint8_t buff[], uint32_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            LINM_SFRS->DATABUFF[i].DATA = (uint32_t)buff[i];
        }
    }
    return result;
}


int8_t LINM_ReadBuffer(uint8_t buff[], uint32_t dataLength)
{
    int8_t result;
    if (dataLength > 8U){
        result = -1;
    }else{
        result = 0;
        for (uint8_t i = 0U; i < dataLength; i++){
            buff[i] = (uint8_t)LINM_SFRS->DATABUFF[i].DATA;
        }
    }
    return result;
}


void linm_SetTiming(LIN_BaudRate_t BaudRate)
{
    LINM_SFRS->BTDIV07 = (uint32_t)linm_speed_map[BaudRate].divider & 0xFFU;
    LINM_SFRS->BITTIME.BTDIV8 = (uint32_t)linm_speed_map[BaudRate].divider >> 8U;
    LINM_SFRS->BITTIME.BTMULT = (uint32_t)linm_speed_map[BaudRate].multiplier;
    LINM_SFRS->BITTIME.PRESCL = (uint32_t)linm_speed_map[BaudRate].prescale;
}


void LINM_Init(void)
{
    IOCTRLA_SFRS->LIN.LINM_HWMODE       = 1U;    /* Hardware Mode Enabled. LIN slave peripheral writes/read the LIN I/O pin.*/
    IOCTRLA_SFRS->LIN.LINM_RXENA        = 1U;    /* LIN receive enable. */
    IOCTRLA_SFRS->LIN.LINM_TXENA        = 1U;    /* LIN transmit enable.*/
    IOCTRLA_SFRS->LIN.LINM_PU1K_ENA     = 1U;    /* LIN 1K pullup enable.*/
    IOCTRLA_SFRS->LIN.SWON = 0U;                 /* 1: disable Master function, 0: enable Master function on LINOUT pin */

    LINM_SFRS->BUSTIME.BUSINACTIVE      = E_LIN_TIME_INACTIVE_SEC_4;
    LINM_SFRS->BUSTIME.WUPREPEAT        = E_LIN_TIME_WAKEUP_REPEAT_MS_180;
    LINM_SFRS->CTRL.RSTERR              = 1U;
    LINM_SFRS->CTRL.RSTINT              = 1U;
    
    linm_SetTiming(E_LIN_BAUDRATE_19200);
}

void LINM_RegisterLINM_IRQ(lin_cb_func_t cb)
{
    linm_callback = cb;
    NVIC_EnableIRQ(LINM_IRQn);
}


void LINM_UnRegisterLINM_IRQ(void)
{
    linm_callback = NULL;
    NVIC_DisableIRQ(LINM_IRQn);
}

int8_t LINM_SendFrame(LIN_Device_Frame_t *frame)
{
    linmFrame = *frame;
    LINM_SFRS->ID        = frame->frame_id;
    if (frame->msg_type == LIN_MSG_TYPE_TX){
        LINM_SFRS->DL.LENGTH = frame->length;
        LINM_SFRS->CTRL.TRANSMIT = 1U;
    }else{
        LINM_SFRS->DL.LENGTH = frame->length;
        LINM_SFRS->CTRL.TRANSMIT = 0U;
    }
    LINM_SFRS->DL.ENHCHK = frame->checksum;
    
    (void)LINM_WriteBuffer(frame->data, (uint32_t)frame->length);
    LINM_SFRS->CTRL.STARTREQ = 1U;
    
    return 0;
}

void LINM_Handler(void)
{
    uint8_t status   = (uint8_t)LINM_STATUS_REG;
    if( ((status & (uint8_t)E_LIN_STATUS_ERROR) == 0U) &&  ((status & (uint8_t)E_LIN_STATUS_WAKEUP) == 0U) ){
        if ( (status & (uint8_t)E_LIN_STATUS_COMPLETE) != 0U ){
            if (linmFrame.msg_type == (uint8_t)LIN_MSG_TYPE_RX){
                linmFrame.length   = (uint8_t)LINM_SFRS->DL.LENGTH;
                (void)LINM_ReadBuffer(linmFrame.data, LIN_BUFF_SIZE);
                if (linm_callback != NULL){
                    linm_callback(&linmFrame);
                }
            }
        } 
    }
    LINM_SFRS->CTRL.RSTERR              = 1U;    /*reset error*/
    LINM_SFRS->CTRL.RSTINT              = 1U;    /*reset interrupt*/
}


/* Please make sure here and don't delete these functions!!!!*/
/* LIN slave wake up interrupt ISR */
void WULINS_Handler(void)
{
    WICA_SFRS->CTRL.LINIRQCLR   = 1U;
}
/* LIN master wake up interrupt ISR */
void WULINM_Handler(void)
{
    WICA_SFRS->CTRL.LINMIRQCLR  = 1U;
}
