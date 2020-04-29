/**
 * @copyright 2016 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file clock_device.h
 */

#ifndef CLOCK_DEVICE_H__
#define CLOCK_DEVICE_H__

#include <stdint.h>
#include <realplum.h>
#include <hwcfg.h>

typedef enum{
  CLOCK_RC_10KHz = 0U,
  CLOCK_RC_16MHz
}ClockSource_t;



#define CLOCK_DIV_1  (0U)
#define CLOCK_DIV_2  (1U)
#define CLOCK_DIV_4  (2U)
#define CLOCK_DIV_8  (3U)

void Clock_SystemMainClockInit(ClockSource_t clock, uint8_t divider);
uint32_t Clock_GetSystemClock(void);

#endif /* __CLOCK_DEVICE_H__ */
