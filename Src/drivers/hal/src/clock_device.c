/**
 * @copyright 2016 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file clock_device.c
 */

#include "clock_device.h"

#define SYSTEM_CLOCK_HIGH_FREQ  (16000000U)
#define SYSTEM_CLOCK_LOW_FREQ   (10000U)
static uint32_t systemClock = SYSTEM_CLOCK_LOW_FREQ;

void Clock_SystemMainClockInit(ClockSource_t clock, uint8_t divider)
{
    if (clock == CLOCK_RC_16MHz){
        CRGA_SFRS->SYSCLKCTRL.HFRCENA = 1U;
        while(CRGA_SFRS->SYSCLKCTRL.HFRCSTS == 0U){}
        SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0EU;
        SYSCTRLA_SFRS->TRIM_HF_RC = HWCFG_GetRCHFClockCalibValue();
        SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0U;
        systemClock = SYSTEM_CLOCK_HIGH_FREQ;
    }else{
        CRGA_SFRS->LFCLKKILL.KILLLFRC = 0U;
        SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0EU;
        SYSCTRLA_SFRS->TRIM_LF_RC = HWCFG_GetRCLFClockCalibValue();
        SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0U;
        systemClock = SYSTEM_CLOCK_LOW_FREQ;
    }
    CRGA_SFRS->SYSCLKCTRL.SYSCLKSEL = (uint32_t)clock;
    CRGA_SFRS->SYSCLKCTRL.DIVSYSCLK = (uint32_t)divider;
    /* SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_LOCK = 1; Lock Trim until reset*/
    systemClock = systemClock >> (uint8_t)divider;

}

uint32_t Clock_GetSystemClock(void)
{
    return systemClock;
}
