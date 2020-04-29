/**
 * @copyright 2018 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file adc_sfr.h
 */

#ifndef ADC_SFR_H__
#define ADC_SFR_H__

#define ADC_SFRS_NEW  1

#if ADC_SFRS_NEW == 1

#define ADC_CONF_REG            (*(__IO uint32_t *)(0x50010D00))
#define ADC_CNTRL_REG           (*(__IO uint32_t *)(0x50010D04))
#define ADC_TSET_REG            (*(__IO uint32_t *)(0x50010D08))
#define ADC_DATA1_REG           (*(__IO uint32_t *)(0x50010D0C))
#define ADC_DATA2_REG           (*(__IO uint32_t *)(0x50010D10))
#define ADC_DATA0345_REG        (*(__IO uint32_t *)(0x50010D14))
#define ADC_STATUS_REG          (*(__IO uint32_t *)(0x50010D18))
/**
 * @brief A structure to represent Special Function Registers for ADC.
 */
typedef struct {
  
   struct {
         uint32_t MODE             :  1; /*!< ADC mode select */
         uint32_t AUTOEN           :  1; /*!< Bias Enable mode */
         uint32_t ATTEN            :  1; /*!< ADC input Attenuation setting */
         uint32_t SAMPCYC          :  3; /*!< Sample cycle */
         uint32_t SWSYNCIN         :  1; /*!< Soft Sync Input */
         uint32_t                  :  9; /*    (reserved) */
         uint32_t                  :  1; /*!< LED BUFFER ENABLE */
         uint32_t                  : 15; /*    (reserved) */
   } CONF;

   struct {
         uint32_t CONVERT :  1; /*!< ADC START/STATUS Register */
         uint32_t CONT    :  1; /*!< Continuous Convesion Enable */
         uint32_t SYNCENA :  1; /*!< Sync Enable */
         uint32_t SYNCEDGE:  1; /*!< Sync Edge Select. Select edge sensitivity of the sync signal. 0x0: Triggered by the posedge of the sync signal.0x1: Triggered by the negedge of the sync signal.*/
         uint32_t STUPDLY :  2; /*!< Startup Delay */
         uint32_t IRQENA  :  1; /*!< IRQ Enable */
         uint32_t IRQCLR  :  1; /*!< IRQ Clear */
         uint32_t CHSEQ   :  6; /*!< Channel Sequence */
         uint32_t         :  2; /*    (reserved) */
         uint32_t CH2SEL  :  2; /*!< Channel2 Selection */
         uint32_t         :  1; /*!< reserved */
         uint32_t         :  1; /*    (reserved) */
         uint32_t CH4SEL  :  2; /*!< Channel4 Selection */
         uint32_t         : 10; /*    (reserved) */
   } CNTRL;

   struct {
         uint32_t TCURR  :  4; /*!< TCURR setting */
         uint32_t TCHNL  :  4; /*!< TCHNL setting */
         uint32_t TGUARD :  4; /*!< TGUARD setting */
         uint32_t       : 20; /*    (reserved) */
   } TSET;

   uint32_t DATA1: 12;
   uint32_t      : 20; /*    (reserved) */

   uint32_t DATA2: 12;
   uint32_t      : 20; /*    (reserved) */

   uint32_t DATA0345: 12;
   uint32_t         : 20; /*    (reserved) */

   struct {
         uint32_t CONVDONE :  1; /*!< The set Sequence of Conversions is Done */
         uint32_t FSM      :  6; /*!< current state of the ADC Sequencer */
         uint32_t         : 25; /*    (reserved) */
   } STATUS;

} ADC_SFRS_t;
#else

/**
 * @brief A structure to represent Special Function Registers for ADC.
 */
typedef struct {

   union {
      struct {
         uint32_t MODE             :  1; /*!< ADC mode select */
         uint32_t AUTOEN           :  1; /*!< Bias Enable mode */
         uint32_t ATTEN            :  1; /*!< ADC input Attenuation setting */
         uint32_t SAMPCYC          :  3; /*!< Sample cycle */
         uint32_t SWSYNCIN         :  1; /*!< Soft Sync Input */
         uint32_t                 :  9; /*    (reserved) */
         uint32_t TRMLEDBUFFER4ADC :  1; /*!< LED BUFFER ENABLE */
         uint32_t                 : 15; /*    (reserved) */
      };
      uint32_t WORD;
   } CONF;

   union {
      struct {
         uint32_t CONVERT :  1; /*!< ADC START/STATUS Register */
         uint32_t CONT    :  1; /*!< Continuous Convesion Enable */
         uint32_t SYNCENA :  1; /*!< Sync Enable */
         uint32_t         :  1; /*    (reserved) */
         uint32_t STUPDLY :  2; /*!< Startup Delay */
         uint32_t IRQENA  :  1; /*!< IRQ Enable */
         uint32_t IRQCLR  :  1; /*!< IRQ Clear */
         uint32_t CHSEQ   :  6; /*!< Channel Sequence */
         uint32_t         :  2; /*    (reserved) */
         uint32_t CH2SEL  :  2; /*!< Channel2 Selection */
         uint32_t CH3SEL  :  1; /*!< Channel3 Selection */
         uint32_t         :  1; /*    (reserved) */
         uint32_t CH4SEL  :  2; /*!< Channel4 Selection */
         uint32_t         : 10; /*    (reserved) */
      };
      uint32_t WORD;
   } CNTRL;

   union {
      struct {
         uint32_t TCURR  :  4; /*!< TCURR setting */
         uint32_t TCHNL  :  4; /*!< TCHNL setting */
         uint32_t TGUARD :  4; /*!< TGUARD setting */
         uint32_t       : 20; /*    (reserved) */
      };
      uint32_t WORD;
   } TSET;

   uint32_t DATA1: 12;
   uint32_t      : 20; /*    (reserved) */

   uint32_t DATA2: 12;
   uint32_t      : 20; /*    (reserved) */

   uint32_t DATA0345: 12;
   uint32_t         : 20; /*    (reserved) */

   union {
      struct {
         uint32_t CONVDONE :  1; /*!< The set Sequence of Conversions is Done */
         uint32_t FSM      :  6; /*!< current state of the ADC Sequencer */
         uint32_t         : 25; /*    (reserved) */
      };
      uint32_t WORD;
   } STATUS;

} ADC_SFRS_t;


#endif
/**
 * @brief The starting address of ADC SFRS.
 */
#define ADC_SFRS ((__IO ADC_SFRS_t *)0x50010d00)

#endif /* end of __ADC_SFR_H__ section */

