/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pmu_device.h
 */

#ifndef PMU_DEVICE_H__
#define PMU_DEVICE_H__

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "realplum.h"

typedef enum{
    POLARITY_NATIVE = 0,
    POLARITY_FLIP,
}BattMonitorPolarity_t;


typedef enum{
  BOR_1V5_1310mV = 0,
  BOR_1V5_1340mV,
  BOR_1V5_1370mV,
  BOR_1V5_1400mV,
  
  BOR_1V5_1440mV,
  BOR_1V5_1480mV,
  BOR_1V5_1510mV,
  BOR_1V5_1560mV,
  
  BOR_1V5_1600mV,
  BOR_1V5_1640mV,
  BOR_1V5_1690mV,
  BOR_1V5_1740mV,
  
  BOR_1V5_1800mV,
  BOR_1V5_1920mV,
  BOR_1V5_2060mV,
  BOR_1V5_2210mV,
}Bor1V5Thres_t;


typedef enum{
  BOR_3V3_2160mV = 0,
  BOR_3V3_2220mV,
  BOR_3V3_2290mV,
  BOR_3V3_2360mV,
  
  BOR_3V3_2430mV,
  BOR_3V3_2510mV,
  BOR_3V3_2590mV,
  BOR_3V3_2680mV,
  
  BOR_3V3_2780mV,
  BOR_3V3_2890mV,
  BOR_3V3_3000mV,
  BOR_3V3_3120mV,
}Bor3V3Thres_t;

/**
 * @brief The enumeration of brownout mode.
 */
typedef enum {
    PMU_BROWNOUT_RESET = 0U,
    PMU_BROWNOUT_INTERRUPT,
    PMU_BROWNOUT_NONE,
} PMU_BROWNOUT_MODE_t;

/**
 * @brief The enumeration of wake up timer mode.
 */
typedef enum {
    WAKEUP_TIMEER_DISABLE = 0U,
    WAKEUP_TIMEER_ENABLE,
} PMU_WAKEUP_TIMEER_MODE_t;

/**
 * @brief The enumeration of wake up timer mode.
 */
typedef enum {
    WAKEUP_TIMEER_INTERVAL_100us = 0U,
    WAKEUP_TIMEER_INTERVAL_200us,
    WAKEUP_TIMEER_INTERVAL_400us,
    WAKEUP_TIMEER_INTERVAL_800us,
    WAKEUP_TIMEER_INTERVAL_16ms,
    WAKEUP_TIMEER_INTERVAL_32ms,
    WAKEUP_TIMEER_INTERVAL_64ms,
    WAKEUP_TIMEER_INTERVAL_128ms,
    WAKEUP_TIMEER_INTERVAL_256ms,
    WAKEUP_TIMEER_INTERVAL_512ms,
    WAKEUP_TIMEER_INTERVAL_1024ms,
    WAKEUP_TIMEER_INTERVAL_2048ms,
    WAKEUP_TIMEER_INTERVAL_4096ms,
    WAKEUP_TIMEER_INTERVAL_8192ms,
    WAKEUP_TIMEER_INTERVAL_16384ms,
    WAKEUP_TIMEER_INTERVAL_32768ms
} PMU_WAKEUP_TIMEER_Interval_t;
void PMU_BORInit(Bor1V5Thres_t lowThreshold, Bor3V3Thres_t highThreshold);
void PMU_WakeTimerInit(PMU_WAKEUP_TIMEER_MODE_t mode, PMU_WAKEUP_TIMEER_Interval_t interval);
void PMU_EnterToDeepSleepMode(void);




#endif /* __PMU_DEVICE_H__ */
