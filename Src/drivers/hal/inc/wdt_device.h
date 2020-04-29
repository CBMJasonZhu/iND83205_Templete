/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file wdt_device.h
 */

#ifndef WDT_DEVICE_H__
#define WDT_DEVICE_H__

#include <stdint.h>
#include "wdt1_sfr.h"
#include "wdta_sfr.h"
#include "errno.h"


typedef void (*wdt_cb_func_t)(void);

typedef enum{
  WDT1_INTERVAL_E13 = 0U,/*0x0: 2^13 / System Clock  512us @16MHz*/
  WDT1_INTERVAL_E19,    /*0x1: 2^19 / System Clock  32768us @16MHz*/
  WDT1_INTERVAL_E22,    /*0x2: 2^22 / System Clock  262144us @16MHz*/
  WDT1_INTERVAL_E32,    /*0x3: 2^32 / System Clock  268seconds @@16MHz*/
}WDT1Interval_t;

typedef enum{
  WDT1_MODE_RESET = 0U,
  WDT1_MODE_INTERRUPT
}WDT1Mode_t;

void WDT1_Enable(WDT1Mode_t mode, WDT1Interval_t interval, wdt_cb_func_t callback);
void WDT1_Clear(void);


typedef enum{
  WDTA_INTERVAL_26MS = 0U,       /*0x0: 2^8 * 100us ~= 26 ms*/
  WDTA_INTERVAL_102MS,          /*0x1: 2^10 * 100us ~= 102 ms*/
  WDTA_INTERVAL_1000MS,         /*0x2: 2^13 * 100us ~= 1 s*/
  WDTA_INTERVAL_13000MS,        /*0x3: 2^17 * 100us ~= 13 s*/
}WDTAInterval_t;

typedef enum{
  WDTA_MODE_RESET = 0U,
  WDTA_MODE_INTERRUPT
}WDTAMode_t;


void WDTA_Enable(WDTAMode_t mode, WDTAInterval_t interval, wdt_cb_func_t callback);
void WDTA_Disable(void);
void WDTA_Clear(void);




#endif /* __WDT_DEVICE_H__ */

