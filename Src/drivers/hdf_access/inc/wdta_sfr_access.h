/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file wdt2_sfr_access.h
 */
 
#ifndef __WDTA_SFR_ACCESS_H__
#define __WDTA_SFR_ACCESS_H__

#include <stdint.h>
#include <stdbool.h>
#include "iceblue.h"

#define WDTA_TIMEOUT_8MS        0
#define WDTA_TIMEOUT_128MS      1
#define WDTA_TIMEOUT_2048MS     2
#define WDTA_TIMEOUT_32768MS    3  

/**
 * @brief Enable watchdog timer.
 */
static __INLINE void f_WDTA_Enable(void)
{
    WDTA_SFRS->STOP = 0;
}

/**
 * @brief Disable watchdog timer.
 */
static __INLINE void f_WDTA_Disable(void)
{
    WDTA_SFRS->STOP = 0x6da475c3;
}

/**
 * @brief Select watchdog timer clock pre-scaler.
 *
 * @param val The setting of pre-scaler.
 */
static __INLINE void f_WDTA_SetPrescaler(uint8_t val)
{
    WDTA_SFRS->CTRL.TIMEOUTSEL = val;
}

/**
 * @brief Clear watchdog timer.
 */
static __INLINE void f_WDTA_Clear(void)
{
    WDTA_SFRS->CLEAR = 0x3c574ad6;
} 

/**
 * @brief Stop watchdog timer.
 */
static __INLINE void f_WDTA_Stop(void)
{
    WDTA_SFRS->STOP = 0x6da475c3;
}

/**
 * @brief Enable watchdog timer reset.
 */
static __INLINE void f_WDTA_ResetEnable(void)
{
    CRGA_SFRS->WDTACTION = 1;
}

/**
 * @brief Disable watchdog timer reset.
 */
static __INLINE void f_WDTA_ResetDisable(void)
{
    CRGA_SFRS->WDTACTION = 0;
}

/**
 * @brief Check if watchdog timer reset occurred.
 *
 * @return The watchdog timer reset flag.
 */
static __INLINE uint8_t f_WDTA_IsReset(void)
{
    return CRGA_SFRS->RESETCTRL.WDTFLAG;
}

#endif /* __WDTA_SFR_ACCESS_H__ */