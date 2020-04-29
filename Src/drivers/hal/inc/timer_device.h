/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file timer_device.h
 */

#ifndef TIMER_DEVICE_H__
#define TIMER_DEVICE_H__

#include <stdint.h>
#include "timer0_sfr.h"
#include "timer1_sfr.h"
#include "timer2_sfr.h"

#define HW_GP_TIMER_NUM  3U
typedef enum{
  TIMER0 = 0U,
  TIMER1,
  TIMER2, 
}HwGpTimerId_t;

typedef enum GlobalTimerMode_t{
    HW_TIMER_ONE_SHOT_MODE = 0U,
    HW_TIMER_PERIODIC_MODE,
} HwGpTimeMode_t;

typedef void (*TimeoutCallbackFunc_t)(void);

typedef struct{
    /* Timer parameters */
    uint32_t timeReloadCnt;
    uint32_t periodCount;
    HwGpTimeMode_t mode;
    TimeoutCallbackFunc_t handler;
} HwGpTimer_t;


void TIMER_Init(HwGpTimerId_t timerId, HwGpTimeMode_t mode, uint32_t timeoutTicks,TimeoutCallbackFunc_t callback);
void TIMER_Disable(HwGpTimerId_t timerId);


#endif /* __TIMER_DEVICE_H__ */