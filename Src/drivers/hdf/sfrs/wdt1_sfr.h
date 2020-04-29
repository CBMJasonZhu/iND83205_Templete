/**
 * @copyright 2017 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file wdt1_sfr.h
 */

#ifndef WDT1_SFR_H__
#define WDT1_SFR_H__
#include <stdint.h>
/**
 * @brief A structure to represent Special Function Registers for WDT1.
  CFG:
    ENA:
      WDT Enable. This bit can be asserted but it cannot be de-asserted. It means 
      that once the WDT is enabled it cannot be turned off until a Reset or Power-On Reset occurs.
    RSTEN:
      Reset enable. If enabled a WDT time-out will force the microcontroller to reset. 
      This bit can be asserted but it cannot be de-asserted.
    RSTFLAG:
      Reset flag. This flag is set by the system at the initialization if the initialization
      was caused by a reset triggered by the WDT. The bit can be cleared by the application.
    PRESET: Preset. Defines the watchdog timeout period. 
      It means that the WDT internal counter will count from 0 to the prescaler value
      at the system clock speed and trigger if not cleared. For instance, a system running
      from a 30MHz Crystal with WDTPRES[1I0] = 10 will trigger the WDT after approximately 
      0.14 seconds if not cleared properly and in time by the application.
      0x0: 2^13 / System Clock
      0x1: 2^19 / System Clock
      0x2: 2^22 / System Clock
      0x3: 2^32 / System Clock
  KEY:
    Key. To clear the WDT counting the following words must be written in this order and without any other instruction between then: 
      0x3C570001 
      0x007F4AD6
  
 */

#define WDT1_CFG_REG                   (*(__IO uint32_t *)(0x50020018))
#define WDT1_KEY_REG                   (*(__IO uint32_t *)(0x5002001C))

typedef struct {
   struct {
         uint32_t ENA     :  1; /*!< Enable */
         uint32_t RSTEN   :  1; /*!< Reset enable */
         uint32_t RSTFLAG :  1; /*!< Reset flag */
         uint32_t PRESET  :  2;
         uint32_t RSVD    :  27;/*    (reserved) */
   } CFG;
   uint32_t KEY;                /*<! Key */
} WDT1_SFRS_t;

/**
 * @brief The starting address of WDT1 SFRS.
 */
#define WDT1_SFRS ((__IO WDT1_SFRS_t *)0x50020018)

#endif /* end of __WDT1_SFR_H__ section */

