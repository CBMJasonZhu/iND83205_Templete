/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file systick_sfrs.h
 */
 
#ifndef SYSTICK_SFRS_H__
#define SYSTICK_SFRS_H__

#include <stdint.h>
#include <core_cm0.h>
/**
 * @brief Enable SysTick timer.
 */
static __INLINE void f_SYSTICK_Enable(void)
{
    SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);
}

/**
 * @brief Disable SysTick timer.
 */
static __INLINE void f_SYSTICK_Disable(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief Check if SysTick timer enable.
 *
 * @return 0 if disable.
 */
static __INLINE uint8_t f_SYSTICK_IsEnabled(void)
{
    
    return (uint8_t)(SysTick->CTRL & SysTick_CTRL_ENABLE_Msk);
}

/**
 * @brief Enable SysTick timer interrupt.
 */
static __INLINE void f_SYSTICK_IntEnable(void)
{
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief Disable SysTick timer interrupt.
 */
static __INLINE void f_SYSTICK_IntDisable(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief Set Reload value of SysTick timer.
 *
 * @param ticks The tick number to be set.
 */
static __INLINE void f_SYSTICK_SetLoad(uint32_t ticks)
{
    SysTick->LOAD = ticks;
} 

/**
 * @brief Set counter value of SysTick timer.
 *
 * @param ticks The tick number to be set.
 */
static __INLINE void f_SYSTICK_SetCount(uint32_t ticks)
{
    SysTick->VAL = ticks;
} 

/**
 * @brief Get counter value of SysTick timer.
 *
 * @return The current counter of Systick timer.
 */
static __INLINE uint32_t f_SYSTICK_GetCount(void)
{
    return SysTick->VAL;
} 

/**
 * @brief Get load value of SysTick timer.
 *
 * @return The load setting of Systick timer.
 */
static __INLINE uint32_t f_SYSTICK_GetLoad(void)
{
    return SysTick->LOAD;
} 
#endif /* __SYSTICK_SFRS_H__ */
