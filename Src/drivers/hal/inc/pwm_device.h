/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pwm_device.h
  */

#ifndef PWM_DEVICE_H
#define PWM_DEVICE_H

#include <stdint.h>
#include <stdbool.h>
#include "realplum.h"
#include "pmua_sfr.h"
#include "hwcfg.h"

typedef void (*PwmIsrCallback_t)(uint8_t status);

typedef enum{
  PWM_PRESCALER_DIVIDE_1 = 0U,
  PWM_PRESCALER_DIVIDE_2,
  PWM_PRESCALER_DIVIDE_4,
  PWM_PRESCALER_DIVIDE_8,
  PWM_PRESCALER_DIVIDE_16,
  PWM_PRESCALER_DIVIDE_64,
  PWM_PRESCALER_DIVIDE_256,
  PWM_PRESCALER_DIVIDE_1024,
}PwmPrescaler_t;

typedef enum{
  PWM_CHANNEL_0 = 0U,
  PWM_CHANNEL_1,
  PWM_CHANNEL_2
}PwmChannel_t;

typedef enum{
  PWM_TRIGGER_POS_EDGE = 0U,
  PWM_TRIGGER_NEG_EDGE,
}PWMTriggerEdge_t;

typedef struct{
  PwmPrescaler_t divide;
  uint16_t       period;
  uint8_t        invertEn;
  uint8_t        enable;
}PwmConfig_t;

void PWM_Init(PwmChannel_t channel, PwmPrescaler_t divide, uint16_t period, uint8_t invertEn);
void PWM_SetMatchValue(PwmChannel_t channel, uint16_t matchRisingValue, uint16_t matchFaillValue);
void PWM_StartAndUpdate(void);
void PWM_SetRGBValue(uint16_t red, uint16_t green, uint16_t blue);
void PWM_RegisterPWMMatchIRQ(PwmChannel_t channel, PWMTriggerEdge_t edge, PwmIsrCallback_t callback);
void PWM_UnRegisterPWMMatchIRQ(PwmChannel_t channel);
void PWM_RegisterPWMReloadIRQ(PwmChannel_t channel, PwmIsrCallback_t callback);
void PWM_UnRegisterPWMReloadIRQ(PwmChannel_t channel);
void PWM_Enable(PwmChannel_t channel);
void PWM_EnableAllChannels(void);
void PWM_Disable(PwmChannel_t channel);
void PWM_DisableAllChannels(void);
void PWM_SetPrescaler(PwmChannel_t channel, PwmPrescaler_t divide);
void PWM_SetPeriod(PwmChannel_t channel, uint16_t period);
void PWM_SetInvert(PwmChannel_t channel, uint8_t invertEn);
  
int8_t PWM_SetMaxChannelCurrent(PwmChannel_t channel, CurrValue_t current,OffCurrValue_t offCurrent);
int8_t PWM_SetCompensationCurrent(PwmChannel_t channel,CurrValue_t maxCurrent, int8_t current);
void PWM_TurnOffChannelCurrent(void);

/*  channel : 0-2  */
uint16_t PWM_GetCurrentPWM(uint8_t channel);

int8_t PWM_UpdateFInished(void);
int8_t PWM_AllChannelsAreOff(void);

#endif /* __PWM_DEVICE_H__  */