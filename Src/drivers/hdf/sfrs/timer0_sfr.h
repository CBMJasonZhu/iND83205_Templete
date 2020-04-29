/**
 * @copyright 2017 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file timer0_sfr.h
 */

#ifndef TIMER0_SFR_H__
#define TIMER0_SFR_H__
#include <stdint.h>
/**
 * @brief A structure to represent Special Function Registers for TIMER0.
  COUNT:
    Count. Initial counter value. The timer will count from this value to 0xFFFFFFFF
    and roll over to 0x00000000. At this point it will generate an interrupt if enabled.
    The interrupt routine is responsible for reloading the value if needed as this timer 
    does not auto-reload the original content.
  ENA:
     Enable. This bit starts/stops the timer: < br> 1 = Timer Running < br> 0 = Timer Inactive
 */
typedef struct {
   uint32_t COUNT;      /*<! Count */
   struct{
       uint32_t ENA     :  1;
       uint32_t RSVD    : 31; /*    (reserved) */
   }CFG;
} TIMER0_SFRS_t;

/**
 * @brief The starting address of TIMER0 SFRS.
 */
#define TIMER0_SFRS ((__IO TIMER0_SFRS_t *)0x50020000)

#endif /* end of __TIMER0_SFR_H__ section */

