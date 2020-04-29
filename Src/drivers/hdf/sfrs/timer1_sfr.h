/**
 * @copyright 2017 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file timer1_sfr.h
 */

#ifndef TIMER1_SFR_H__
#define TIMER1_SFR_H__
#include <stdint.h>
/**
 * @brief A structure to represent Special Function Registers for TIMER1.
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
} TIMER1_SFRS_t;

/**
 * @brief The starting address of TIMER1 SFRS.
 */
#define TIMER1_SFRS ((__IO TIMER1_SFRS_t *)0x50020008)

#endif /* end of __TIMER1_SFR_H__ section */

