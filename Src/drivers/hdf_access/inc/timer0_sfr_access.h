/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file timer0_sfr_access.h
 */

#ifndef __TIMER0_SFR_ACCESS_H__
#define __TIMER0_SFR_ACCESS_H__

/**
 * @brief Start timer 0
 */
static __INLINE void f_Timer0_Start(void)
{
    TIMER0_SFRS->ENA = 1;
}

/**
 * @brief Stop timer 0
 */
static __INLINE void f_Timer0_Stop(void)
{
    TIMER0_SFRS->ENA = 0;
}

/**
 * @brief Load timing setting for timer 0
 */
static __INLINE void f_Timer0_LoadCounter(uint32_t cnt)
{
    TIMER0_SFRS->COUNT = cnt;
}

#endif /* __TIMER0_SFR_ACCESS_H__ */