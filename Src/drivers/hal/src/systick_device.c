/**
 * @copyright 2016 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file systick_device.c
 */

#include <stddef.h>
#include "errno.h"
#include "realplum.h"
#include "systick_sfrs.h"
#include "systick_device.h"
#include "clock_device.h"
#include <isrfuncs.h>

static sysTick_cb_func_t sysTickCallback = NULL;

void SysTick_Handler(void)
{
    if (sysTickCallback != NULL){
        sysTickCallback();
    }
}


void SysTick_Init(uint32_t ticks, sysTick_cb_func_t callback)
{
    SysTick->VAL = 0U;
    SysTick->LOAD = ticks;
    sysTickCallback = callback;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);
    NVIC_EnableIRQ(SysTick_IRQn);
}


uint32_t SysTick_GetCount(void)
{
    uint32_t count = SysTick->VAL;
    return count;
}

uint32_t SysTick_GetLoad(void)
{
    uint32_t count = SysTick->LOAD;
    return count;
}


