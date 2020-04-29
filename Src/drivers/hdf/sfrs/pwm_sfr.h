/**
 * @copyright 2017 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file pwm_sfr.h
 */

#ifndef PWM_SFR_H__
#define PWM_SFR_H__

#define PWM_CHANNEL_NUM    3U


#define PWM_BASE0_REG                  (*(__IO uint32_t *)(0x50010600))
#define PWM_BASE1_REG                  (*(__IO uint32_t *)(0x50010604))
#define PWM_BASE2_REG                  (*(__IO uint32_t *)(0x50010608))
#define PWM_CTRL_REG                   (*(__IO uint32_t *)(0x5001060C))
#define PWM_PULSE0_REG                 (*(__IO uint32_t *)(0x50010610))
#define PWM_PULSE1_REG                 (*(__IO uint32_t *)(0x50010614))
#define PWM_PULSE2_REG                 (*(__IO uint32_t *)(0x50010618))
#define PWM_INTCTRL_REG                (*(__IO uint32_t *)(0x5001061C))
#define PWM_INTSTATUS_REG              (*(__IO uint32_t *)(0x50010620))
#define PWM_INTUPDATED_REG             (*(__IO uint32_t *)(0x50010624))
/**
 * @brief A structure to represent Special Function Registers for PWM BASE.

PRESCALESEL0:
    Prescaler select. Defines the ratio between the system clock and the clock used for the waveform generator.
    0x0: Divide by 1
    0x1: Divide by 2
    0x2: Divide by 4
    0x3: Divide by 8
    0x4: Divide by 16
    0x5: Divide by 64
    0x6: Divide by 256
    0x7: Divide by 1024
PERIOD0:
    Period. Specifies the period of the output waveform in terms of a number of prescaler output cycles.
 */

typedef struct {
       uint32_t                 : 8; /*!< Reserved */
       uint32_t PRESCALESEL     : 3;
       uint32_t                 : 5; /*!< Reserved */
       uint32_t PERIOD          :16;
} PWM_BASE_t;


/**
 * @brief A structure to represent Special Function Registers for PWM CTRL.
*/

typedef struct {
       uint32_t ENAREQ  :  4; /*!< Enable request. Set to enable the waveform generator. */
       uint32_t         :  4; /*    (reserved) */
       uint32_t ENASTS  :  4; /*!< Enable status. Status of enable in the waveform generator. */
       uint32_t         :  4; /*    (reserved) */
       uint32_t INVERT  :  4; /*Invert. Set to invert the output waveform.*/
       uint32_t         :  4; /*    (reserved) */
       uint32_t UPDATE  :  4; /*Update. Set to trigger consumption of new PULSE parameters (invert,prescale_sel,period,pulse start & stop). The flag is automatically cleared by the hardware when the settings are consumed, so reading a high value indicates that an update is still pending.*/
       uint32_t         :  4; /*    (reserved) */
} PWM_CTRL_t;

/**
 * @brief A structure to represent Special Function Registers for PWM PULSE.
*/

typedef struct {
       uint32_t PFALL : 16; /*!< Pulse Rise. Specifies the pulse rise of the output waveform in terms of a number of prescaler output cycles. However, if INVERT is set it determines the pulse fall timing. */
       uint32_t PRISE : 16; /*!< Pulse Fall. Specifies the pulse fall of the output waveform in terms of a number of prescaler output cycles. However, if INVERT is set it determines the pulse rise timing. */
} PWM_PULSE_t;


/**
 * @brief A structure to represent Special Function Registers for PWM CTRL.
*/

typedef struct {
       uint32_t ENABLE :  8; /*!< Interrupt enable. bit[3:0] : posedge interrupt enable; bit[7:4] : negedge interrupt enable. */
       uint32_t        :  8; /*    (reserved) */
       uint32_t CLEAR  :  8; /*!< Interrupt clear. bit[3:0]: posedge interrupt clear; bit[7:4] : negedge interrupt clear. */
       uint32_t        :  8; /*    (reserved) */
} PWM_INTCTRL_t;

/**
 * @brief A structure to represent Special Function Registers for PWM INTSTATUS.
*/
typedef struct {
       uint32_t STATUS : 8; /*!< Interrupt status. bit[3:0] : posedge interrupt status; bit[7:4] : negedge interrupt status. */
       uint32_t        : 8; /*    (reserved) */
       uint32_t IRQ    : 8; /*!< Interrupt status. bit[3:0]: posedge interrupt status; bit[7:4] : negedge interrupt status. */
       uint32_t        : 8; /*    (reserved) */
} PWM_INTSTATUS_t;


/**
 * @brief A structure to represent Special Function Registers for PWM INTUPDATED.
*/
typedef struct {
       uint32_t ENABLE :  4; /*!< Interrupt enable */
       uint32_t        :  4; /*    (reserved) */
       uint32_t CLEAR  :  4; /*!< Interrupt clear */
       uint32_t        :  4; /*    (reserved) */
       uint32_t STATUS :  4; /*!< Interrupt status */
       uint32_t        :  4; /*    (reserved) */
       uint32_t IRQ    :  4; /*!< Interrupt active */
       uint32_t        :  4; /*    (reserved) */
} PWM_INTUPDATED_t;
/**
 * @brief A structure to represent Special Function Registers for PWM.
 */
typedef struct {
  PWM_BASE_t            BASE[3];
  PWM_CTRL_t            CTRL;
  PWM_PULSE_t           PULSE[3];
  PWM_INTCTRL_t         INTCTRL;
  PWM_INTSTATUS_t       INTSTATUS;
  PWM_INTUPDATED_t      INTUPDATED;
} PWM_SFRS_t;

/**
 * @brief The starting address of PWM SFRS.
 */
#define PWM_SFRS ((__IO PWM_SFRS_t *)0x50010600)

#endif /* end of __PWM_SFR_H__ section */

