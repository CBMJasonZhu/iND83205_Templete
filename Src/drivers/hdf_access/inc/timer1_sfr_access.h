/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file timer1_sfr_access.h
 */

#ifndef __TIMER1_SFR_ACCESS_H__
#define __TIMER1_SFR_ACCESS_H__

/**
 * @brief Start timer 1
 */
static __INLINE void f_Timer1_Start(void)
{
    TIMER1_SFRS->ENA = 1;
}

/**
 * @brief Stop timer 1
 */
static __INLINE void f_Timer1_Stop(void)
{
    TIMER1_SFRS->ENA = 0;
}

/**
 * @brief Load timing setting for timer 1
 */
static __INLINE void f_Timer1_LoadCounter(uint32_t cnt)
{
    TIMER1_SFRS->COUNT = cnt;
}

#endif /* __TIMER1_SFR_ACCESS_H__ */