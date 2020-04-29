/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file timer2_sfr_access.h
 */

#ifndef __TIMER2_SFR_ACCESS_H__
#define __TIMER2_SFR_ACCESS_H__

/**
 * @brief Start timer 2
 */
static __INLINE void f_Timer2_Start(void)
{
    TIMER2_SFRS->ENA = 1;
}

/**
 * @brief Stop timer 2
 */
static __INLINE void f_Timer2_Stop(void)
{
    TIMER2_SFRS->ENA = 0;
}

/**
 * @brief Load timing setting for timer 2
 */
static __INLINE void f_Timer2_LoadCounter(uint32_t cnt)
{
    TIMER2_SFRS->COUNT = cnt;
}
#endif /* __TIMER2_SFR_ACCESS_H__ */