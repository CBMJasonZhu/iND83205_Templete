/**
 * @copyright 2018 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file ioctrla_sfr.h
 */

#ifndef IOCTRLA_SFR_H__
#define IOCTRLA_SFR_H__

#define NEW_IO_CFG   1


#if NEW_IO_CFG == 1

/**
 * @brief A structure to represent Special Function Registers for IOCTRLA.
 */

#define IOCTRLA_GPIO1_REG                      (*(__IO uint32_t *)(0x50011000))
#define IOCTRLA_GPIO2_REG                      (*(__IO uint32_t *)(0x50011004))
#define IOCTRLA_GPIO3_REG                      (*(__IO uint32_t *)(0x50011008))
#define IOCTRLA_GPIO4_REG                      (*(__IO uint32_t *)(0x5001100C))
#define IOCTRLA_LIN_REG                        (*(__IO uint32_t *)(0x50011010))
#define IOCTRLA_LED_REG                        (*(__IO uint32_t *)(0x50011014))
#define IOCTRLA_ANALOGTESTMUXOVERRIDE_REG      (*(__IO uint32_t *)(0x50011018))

typedef struct {
        struct {
           uint32_t HWMODE  :  2; /*!< hardware mode */
           uint32_t LINM_SEL:  1; /*!< LINM Connection Select. */
           uint32_t PUENA   :  1; /*!< pullup enable (active-low) */
           uint32_t PDENA   :  1; /*!< pulldown enable */
           uint32_t RDENA   :  1; /*!< read enable */
           uint32_t PWM_SEL :  2; /*!< PWM output selection for PWM hardware mode */
           uint32_t         : 16; /*    (reserved) */
           uint32_t MUXSEL  :  7;
           uint32_t         :  1; /*    (reserved) */
        }GPIO1;
    
        struct {
           uint32_t HWMODE  :  2; /*!< hardware mode */
           uint32_t         :  1; /*    (reserved) */
           uint32_t PUENA   :  1; /*!< pullup enable (active-low) */
           uint32_t PDENA   :  1; /*!< pulldown enable */
           uint32_t RDENA   :  1; /*!< read enable */
           uint32_t PWM_SEL :  2; /*!< PWM output selection for PWM hardware mode */
           uint32_t         : 16; /*    (reserved) */
           uint32_t         :  7;
           uint32_t         :  1; /*    (reserved) */
        }GPIO2;
    
        struct {
           uint32_t HWMODE  :  2; /*!< hardware mode */
           uint32_t LINS_SEL:  1; /*!< LINS Connection Select. */
           uint32_t PUENA   :  1; /*!< pullup enable (active-low) */
           uint32_t PDENA   :  1; /*!< pulldown enable */
           uint32_t RDENA   :  1; /*!< read enable */
           uint32_t PWM_SEL :  2; /*!< PWM output selection for PWM hardware mode */
           uint32_t         : 16; /*    (reserved) */
           uint32_t         :  7;
           uint32_t         :  1; /*    (reserved) */
        }GPIO3;
    
        struct {
           uint32_t HWMODE  :  2; /*!< hardware mode */
           uint32_t         :  1; /*!< reserved */
           uint32_t PUENA   :  1; /*!< pullup enable (active-low) */
           uint32_t PDENA   :  1; /*!< pulldown enable */
           uint32_t RDENA   :  1; /*!< read enable */
           uint32_t PWM_SEL :  2; /*!< PWM output selection for PWM hardware mode */
           uint32_t         : 16; /*    (reserved) */
           uint32_t         :  7;
           uint32_t         :  1; /*    (reserved) */
        }GPIO4;
    
      struct {
         uint32_t LINS_HWMODE    :  1; /*!< LIN Slave hardware mode */
         uint32_t                :  2; /*    (reserved) */
         uint32_t LINS_PU30K_ENA :  1; /*!< LIN 30K pullup enable */
         uint32_t                :  1; /*    (reserved) */
         uint32_t LINS_TXENA     :  1; /*!< LIN transmit enable */
         uint32_t LINS_RXENA     :  1; /*!< LIN receive  enable */
         uint32_t                :  1; /*    (reserved) */
         uint32_t LINM_HWMODE    :  1; /*!< LIN Master hardware mode */
         uint32_t                :  3; /*    (reserved) */
         uint32_t LINM_PU1K_ENA  :  1; /*!< LIN 1K  pullup enable */
         uint32_t LINM_TXENA     :  1; /*!< LIN transmit enable */
         uint32_t LINM_RXENA     :  1; /*!< LIN receive  enable */
         uint32_t                :  1; /*    (reserved) */
         uint32_t SWON           :  1; /*!< LIN Dual Mode Switch On */
         uint32_t DS_SWON        :  1; /*!< LIN Downstream Switch On */
         uint32_t                :  6; /*    (reserved) */
         uint32_t SWON_LOCK      :  1; /*!< SWON Lock Bit */
         uint32_t                :  6; /*    (reserved) */
         uint32_t PMODE          :  1; /*!< LIN Power Mode */
      }LIN;

      struct {
         uint32_t HWMODE    :  3; /*!< LED hardware mode */
         uint32_t           :  1; /*    (reserved) */
         uint32_t DATA      :  3; /*!< LED Data Out */
         uint32_t           :  1; /*    (reserved) */
         uint32_t SENSE_ENA :  1; /*!< LED Forward Voltage Sense Enable */
         uint32_t           : 23; /*    (reserved) */
      }LED;

      struct {
         uint32_t ADCCONSEL  :  1; /*!< Hardware/Firmware Select */
         uint32_t GPIOCONSEL :  1; /*!< Hardware/Firmware Select */
         uint32_t ADCSELSEL  :  1; /*!< Hardware/Firmware Select */
         uint32_t            :  5; /*    (reserved) */
         uint32_t ADCCONREG  :  6; /*!< Firmware Debug Value */
         uint32_t            :  2; /*    (reserved) */
         uint32_t GPIOCONREG :  7; /*!< Firmware Debug Value */
         uint32_t            :  1; /*    (reserved) */
         uint32_t ADCSELREG  :  4; /*!< Firmware Debug Value */
         uint32_t            :  4; /*    (reserved) */
      }ANALOGTESTMUXOVERRIDE;
} IOCTRLA_SFRS_t;

#else
typedef union {
    struct {
       uint32_t HWMODE  :  2; /*!< hardware mode */
       uint32_t         :  1; /*    (reserved) */
       uint32_t PUENA   :  1; /*!< pullup enable (active-low) */
       uint32_t PDENA   :  1; /*!< pulldown enable */
       uint32_t RDENA   :  1; /*!< read enable */
       uint32_t PWM_SEL :  2; /*!< PWM output selection for PWM hardware mode */
       uint32_t         : 16; /*    (reserved) */
       uint32_t MUXSEL  :  7;
       uint32_t         :  1; /*    (reserved) */
    };
    uint32_t WORD;
}GPIO_REG_t;


/**
 * @brief A structure to represent Special Function Registers for IOCTRLA.
 */
typedef struct {
    GPIO_REG_t GPIO[4];
   union {
      struct {
         uint32_t LINS_HWMODE    :  1; /*!< LIN Slave hardware mode */
         uint32_t                :  1; /*    (reserved) */
         uint32_t LINS_ENA       :  1; /*!< LIN Slave enable */
         uint32_t LINS_PU30K_ENA :  1; /*!< LIN 30K pullup enable */
         uint32_t LINS_PU1K_ENA  :  1; /*!< LIN 1K  pullup enable */
         uint32_t LINS_TXENA     :  1; /*!< LIN transmit enable */
         uint32_t LINS_RXENA     :  1; /*!< LIN receive  enable */
         uint32_t                :  1; /*    (reserved) */
         uint32_t LINM_HWMODE    :  1; /*!< LIN Master hardware mode */
         uint32_t                :  1; /*    (reserved) */
         uint32_t LINM_ENA       :  1; /*!< LIN Master enable */
         uint32_t LINM_PU30K_ENA :  1; /*!< LIN 30K pullup enable */
         uint32_t LINM_PU1K_ENA  :  1; /*!< LIN 1K  pullup enable */
         uint32_t LINM_TXENA     :  1; /*!< LIN transmit enable */
         uint32_t LINM_RXENA     :  1; /*!< LIN receive  enable */
         uint32_t                :  1; /*    (reserved) */
         uint32_t SWON           :  1; /*!< LIN Dual Mode Switch On */
         uint32_t DS_SWON        :  1; /*!< LIN Downstream Switch On */
         uint32_t                :  6; /*    (reserved) */
         uint32_t SWON_LOCK      :  1; /*!< SWON Lock Bit */
         uint32_t                :  7; /*    (reserved) */
      };
      uint32_t WORD;
   } LIN;

   union {
      struct {
         uint32_t HWMODE :  3; /*!< LED hardware mode */
         uint32_t       :  1; /*    (reserved) */
         uint32_t DATA   :  3; /*!< LED Data Out */
         uint32_t       : 25; /*    (reserved) */
      };
      uint32_t WORD;
   } LED;

   union {
      struct {
         uint32_t ADCCONSEL            :  1; /*!< Hardware/Firmware Select */
         uint32_t GPIOCONSEL           :  1; /*!< Hardware/Firmware Select */
         uint32_t ADCSELSEL            :  1; /*!< Hardware/Firmware Select */
         uint32_t                      :  5; /*    (reserved) */
         uint32_t ADCCONREG            :  8; /*!< Firmware Debug Value */
         uint32_t GPIOCONREG           :  6; /*!< Firmware Debug Value */
         uint32_t                      :  2; /*    (reserved) */
         uint32_t ADCSELREG            :  4; /*!< Firmware Debug Value */
         uint32_t TRMBGENAREG          :  1; /*!< Firmware Debug Value */
         uint32_t TRMBGV1212CONNECTREG :  1; /*!< Firmware Debug Value */
         uint32_t TRMBGVBGCONNECTREG   :  1; /*!< Firmware Debug Value */
         uint32_t                      :  1; /*    (reserved) */
      };
      uint32_t WORD;
   } ANALOGTESTMUXOVERRIDE;
} IOCTRLA_SFRS_t;


#endif
/**
 * @brief The starting address of IOCTRLA SFRS.
 */
#define IOCTRLA_SFRS ((__IO IOCTRLA_SFRS_t *)0x50011000)

#endif /* end of __IOCTRLA_SFR_H__ section */

