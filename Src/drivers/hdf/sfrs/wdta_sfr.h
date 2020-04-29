/**
 * @copyright 2017 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file wdta_sfr.h
 */

#ifndef WDTA_SFR_H__
#define WDTA_SFR_H__

#define WDT_STOP_KEY    0x6da475c3
#define WDT_CLEAR_KEY   0x3c574ad6

/**
 * @brief A structure to represent Special Function Registers for WDTA.
CTRL:
  TIMEOUTSEL:
      Timeout select. Defines the watchdog timeout period (the time between a clear operation and the next timeout).
      0x0: 2^8 * 100us ~= 26 ms
      0x1: 2^10 * 100us ~= 102 ms
      0x2: 2^13 * 100us ~= 1 s
      0x3: 2^17 * 100us ~= 13 s
  RUNNING:
      Running status. A flag that indicates when the watchdog timer is enabled.
      0x0: Watchdog timer is stopped and cleared
      0x1: Watchdog timer is running

STOP:
      Stop. Write the *stop* code (0x6da475c3) to this register to reset the timer and disable
      the watchdog (e.g. during debug). If any other value is written to this register 
      the watchdog will be enabled.
CLEAR:
      Clear. Write the value 0x3c574ad6 (as a single word access) to reset the watchdog timer. 
      Periodically performing this action is the expected method of preventing the watchdog 
      from timing out (and resetting the MCU).   

CNTVAL:
      Counter value. The instantaneous value of watchdog timeout counter
 */

#define WDTA_CTRL_REG                  (*(__IO uint32_t *)(0x50010300))
#define WDTA_STOP_REG                  (*(__IO uint32_t *)(0x50010304))
#define WDTA_CLEAR_REG                 (*(__IO uint32_t *)(0x50010308))
#define WDTA_CNTVAL_REG                (*(__IO uint32_t *)(0x5001030C))

typedef struct {

   struct {
         uint32_t            :  1; /*    (reserved) */
         uint32_t RUNNING    :  1; /*!< Running status */
         uint32_t            :  6; /*    (reserved) */
         uint32_t TIMEOUTSEL :  2; /*!< Timeout select */
         uint32_t            : 22; /*    (reserved) */
   } CTRL;
   uint32_t STOP; /*<! Stop */
   uint32_t CLEAR; /*<! Clear */
   uint32_t CNTVAL;

} WDTA_SFRS_t;

/**
 * @brief The starting address of WDTA SFRS.
 */
#define WDTA_SFRS ((__IO WDTA_SFRS_t *)0x50010300)

#endif /* end of __WDTA_SFR_H__ section */

