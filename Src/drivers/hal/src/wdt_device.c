/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file wdt_device.c
 */

#include <string.h>
#include "realplum.h"
#include "wdt_device.h"
#include "errno.h"
#include <isrfuncs.h>
/**
 * @brief The instance of WDT device
 */
static wdt_cb_func_t  wdt1Callback = NULL;
static wdt_cb_func_t  wdtaCallback = NULL;


void Watchdog_Handler(void)
{
    if (wdt1Callback != NULL){
        wdt1Callback();
    }
}

void WDT1_Enable(WDT1Mode_t mode, WDT1Interval_t interval, wdt_cb_func_t callback)
{
    WDT1_SFRS->CFG.PRESET       = (uint32_t)interval;
    if (mode == WDT1_MODE_RESET){
        WDT1_SFRS->CFG.RSTEN        = 1U;
    }else{
        wdt1Callback = callback;
        NVIC_EnableIRQ(Watchdog_IRQn);
    }
    WDT1_SFRS->CFG.ENA          = 1U;
}

#pragma optimize=none
void WDT1_Clear(void)
{
    __disable_irq();
    WDT1_SFRS->KEY       = 0x3C570001U;
    WDT1_SFRS->KEY       = 0x007F4AD6U; 
    __enable_irq();
}


void WatchdogA_Handler(void)
{
    if (wdtaCallback != NULL){
        wdtaCallback();
    }
}

void WDTA_Enable(WDTAMode_t mode, WDTAInterval_t interval, wdt_cb_func_t callback)
{
    WDTA_SFRS->CTRL.TIMEOUTSEL = (uint32_t)interval;
    if (mode == WDTA_MODE_RESET){
        CRGA_SFRS->WDTACTION.WDTACTION = 1U; /*  reset */
    }else{
        wdtaCallback = callback;
        NVIC_EnableIRQ(WatchdogA_IRQn);
        CRGA_SFRS->WDTACTION.WDTACTION = 0U;/*  interrupt */
    }
    WDTA_SFRS->CLEAR = 0x3c574ad6U;
    WDTA_SFRS->STOP = 0U;/* start wdt*/
}

void WDTA_Disable(void)
{
    WDTA_SFRS->STOP = 0xC3U;/* stop wdt*/
}

void WDTA_Clear(void)
{
    WDTA_SFRS->CLEAR = 0x3c574ad6U;
}






























