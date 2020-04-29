/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pwm0_sfrs.h
 */
 
#ifndef __PWM_SFRS_H__
#define __PWM_SFRS_H__

#include <stdint.h>
#include <stdbool.h>
#include "iceblue.h"

#define E_PWM0_PRESCALE_DIV1            0
#define E_PWM0_PRESCALE_DIV2            1
#define E_PWM0_PRESCALE_DIV4            2
#define E_PWM0_PRESCALE_DIV8            3
#define E_PWM0_PRESCALE_DIV16           4
#define E_PWM0_PRESCALE_DIV64           5
#define E_PWM0_PRESCALE_DIV256          6
#define E_PWM0_PRESCALE_DIV1024         7

/**
 * @brief Set period for PWM
 *
 * @param period The PWM period in clock counts.
 */
static __INLINE void f_PWM0_SetPeriod(uint32_t period)
{
    PWM_SFRS->PULSE.PERIOD = period;
}

/**
 * @brief Set width for PWM
 *
 * @param width The PWM width in clock counts.
 */
static __INLINE void f_PWM0_SetWidth(uint32_t width)
{
    PWM_SFRS->PULSE.PWIDTH = width;
}

/**
 * @brief Set clock pre-scaler for PWM
 *
 * @param div The PWM clock divider setting.
 */
static __INLINE void f_PWM0_SetPreScaler(uint8_t div)
{
    PWM_SFRS->PRESCALESEL = div;
}

/**
 * @brief Enable on inverter for PWM
 */
static __INLINE void f_PWM0_InvertOn(void)
{
    PWM_SFRS->CTRL.INVERT = 1;
}

/**
 * @brief Disable on inverter for PWM
 */
static __INLINE void f_PWM0_InvertOff(void)
{
    PWM_SFRS->CTRL.INVERT = 0;
}

/**
 * @brief Enable PWM
 */
static __INLINE void f_PWM0_Enable(void)
{
    PWM_SFRS->CTRL.ENAREQ = 1;
}

/**
 * @brief Disable PWM
 */
static __INLINE void f_PWM0_Disable(void)
{
    PWM_SFRS->CTRL.ENAREQ = 0;
}

/**
 * @brief Update PWM with new period and width.
 */
static __INLINE void f_PWM0_Update(void)
{
    PWM_SFRS->CTRL.UPDATE = 1;
}

/**
 * @brief Check if PWM update in progress.
 */
static __INLINE uint8_t f_PWM0_IsUpdate(void)
{
    return PWM_SFRS->CTRL.UPDATE;
}

#endif /* __PWM_SFRS_H__ */
