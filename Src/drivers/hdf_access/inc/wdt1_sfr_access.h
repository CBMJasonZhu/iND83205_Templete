/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file WDT1_sfrs.h
 */
 
#ifndef __WDT1_SFR_ACCESS_H__
#define __WDT1_SFR_ACCESS_H__

#define WDT1_PRES_2PWR13     0
#define WDT1_PRES_2PWR19     1
#define WDT1_PRES_2PWR22     2
#define WDT1_PRES_2PWR32     3  

/**
 * @brief Enable watchdog timer.
 */
static __INLINE void f_WDT1_Enable(void)
{
    WDT1_SFRS->CFG.ENA = 1;
}

/**
 * @brief Disable watchdog timer.
 */
static __INLINE void f_WDT1_Disable(void)
{
    WDT1_SFRS->CFG.ENA = 0;
}

/**
 * @brief Enable watchdog timer reset.
 */
static __INLINE void f_WDT1_ResetEnable(void)
{
    WDT1_SFRS->CFG.RSTEN = 1;
}

/**
 * @brief Disable watchdog timer reset.
 */
static __INLINE void f_WDT1_ResetDisable(void)
{
    WDT1_SFRS->CFG.RSTEN = 0;
}

/**
 * @brief Select watchdog timer clock pre-scaler.
 *
 * @param val The setting of pre-scaler.
 */
static __INLINE void f_WDT1_SetPrescaler(uint8_t val)
{
    WDT1_SFRS->CFG.PRESET = val;
}

/**
 * @brief Clear watchdog timer.
 */
static __INLINE void f_WDT1_Clear(void)
{
    WDT1_SFRS->KEY = 0x3C570001;
    WDT1_SFRS->KEY = 0x007F4AD6;   
} 

/**
 * @brief Check if watchdog timer reset occurred.
 *
 * @return The watchdog timer reset flag.
 */
static __INLINE uint8_t f_WDT1_IsReset(void)
{
    return WDT1_SFRS->CFG.RSTFLAG;
} 

#endif /* __WDT1_SFR_ACCESS_H__ */