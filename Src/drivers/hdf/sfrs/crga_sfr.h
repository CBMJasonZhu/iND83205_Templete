/**
 * @copyright 2017 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file crga_sfr.h
 */

#ifndef CRGA_SFR_H__
#define CRGA_SFR_H__

#include <stdint.h>
#include <stdbool.h>

#define CRGA_LFCLKCTRL_REG          (*(__IO uint32_t *)(0x50000000))
#define CRGA_SYSCLKCTRL_REG         (*(__IO uint32_t *)(0x50000004))
#define CRGA_RESETCTRL_REG          (*(__IO uint32_t *)(0x50000008))
#define CRGA_BORACTION_REG          (*(__IO uint32_t *)(0x5000000C))
#define CRGA_BORCONFIG_REG          (*(__IO uint32_t *)(0x50000010))
#define CRGA_WDTACTION_REG          (*(__IO uint32_t *)(0x50000014))
#define CRGA_LFCLKKILL_REG          (*(__IO uint32_t *)(0x50000018))
#define CRGA_CPCLKCTRL_REG          (*(__IO uint32_t *)(0x5000001C))
#define CRGA_OVTEMPACTION_REG       (*(__IO uint32_t *)(0x50000020))
#define CRGA_OVTEMPCONFIG_REG       (*(__IO uint32_t *)(0x50000024))

/**
 * @brief A structure to represent Special Function Registers for LFCLKCTRL.
 */
typedef struct{
    uint32_t LFRCSTS:  1; /*Slow oscillator status. Will be high when the 10KHz oscillator is selected*/
    uint32_t        : 31; /*    (reserved) */
}LFCLKCTRL_t;


/**
 * @brief A structure to represent Special Function Registers for SYSCLKCTRL.
DIVSYSCLK:
    Clock div select. Select the divider ratio on the system clock when using fast oscillator
    0x0: No Division. Full Clock speed.
    0x1: Div by 2.
    0x2: Div by 4.
    0x3: Div by 8.
SYSCLKSEL:
    Clock select. Used to switch between the fast and slow system clocks
    0x0: Slow clock (10 KHz)
    0x1: Fast clock (16 MHz)
HFRCSTS:
    Fast oscillator status. Will be high when the 16MHz oscillator is enabled
HFRCENA:
    Fast oscillator enable. Setting this bit when the 16Hz oscillator is not running 
    will cause the oscillator to start (the PMU may have already started it). Even though 
    the fast oscillator is running, its output is only used when selected via the clock mux 
    - see CLKSEL. This bit is cleared automatically on entering SLEEP mode
*/

typedef struct {
    uint32_t HFRCENA   :  1; /*!< Fast oscillator enable */
    uint32_t HFRCSTS   :  1; /*!< Fast oscillator status */
    uint32_t           :  6;  /*    (reserved) */
    uint32_t SYSCLKSEL :  1; /*!< Clock select */
    uint32_t           :  7;  /*    (reserved) */
    uint32_t DIVSYSCLK :  2; /*!< Clock div select */
    uint32_t           : 14;  /*    (reserved) */
}SYSCLKCTRL_t;

/**
 * @brief A structure to represent Special Function Registers for RESETCTRL.
*/
typedef struct {
    uint32_t PORFLAG       :  1; /*!< Power on reset flag */
    uint32_t              :  1; /*    (reserved) */
    uint32_t BOR3V3FLAG    :  1; /*!< BOR 3v3 flag */
    uint32_t              :  1; /*    (reserved) */
    uint32_t BOR1V5FLAG    :  1; /*!< BOR 1v5 flag */
    uint32_t WDTFLAG       :  1; /*!< Watchdog bark flag */
    uint32_t OVTEMPFLAG    :  1; /*!< Ovet Temp Violation flag */
    uint32_t              :  1; /*    (reserved) */
    uint32_t PORFLAGCLR    :  1; /*!< POR flag clear */
    uint32_t              :  1; /*    (reserved) */
    uint32_t BOR3V3FLAGCLR :  1; /*!< BOR 3v3 clear */
    uint32_t              :  1; /*    (reserved) */
    uint32_t BOR1V5FLAGCLR :  1; /*!< BOR 1v5 clear */
    uint32_t WDTFLAGCLR    :  1; /*!< WDT flag clear */
    uint32_t OVTEMPFLAGCLR :  1; /*!< OVTEMP flag clear */
    uint32_t              :  1; /*    (reserved) */
    uint32_t HARDRSTREQ    :  1; /*!< Hard reset request */
    uint32_t              :  7; /*    (reserved) */
    uint32_t SOFTRSTREQ    :  1; /*!< Soft reset request */
    uint32_t              :  7; /*    (reserved) */
} RESETCTRL_t;


/**
 * @brief A structure to represent Special Function Registers for BORACTION.
  VDD1V8:
      BOR 1v8 action. Defines the consequences of brown-out condition on the 1v5 supply being detected by the hardware.
      0x2: No action
      0x1: IRQ generated
      0x0: Hard reset generated 
  VDD2V6:
      BOR 2v6 action(Obsolete). Defines the consequences of brown-out condition on the 2v6 supply being detected by the hardware.
      0x2: No action
      0x1: IRQ generated
      0x0: Hard reset generated
  VDD3V3:
      BOR 3v3 action. Defines the consequences of brown-out condition on the 3v3 supply being detected by the hardware.
      0x2: No action
      0x1: IRQ generated
      0x0: Hard reset generated
 */

typedef struct {
    uint32_t VDD3V3 :  2; /*!< BOR 3v3 action */
    uint32_t       :  2; /*    (reserved) */
    uint32_t VDD1V5 :  2; /*!< BOR 1v5 action */
    uint32_t       : 26; /*    (reserved) */
} BORACTION_t;

/**
 * @brief A structure to represent Special Function Registers for BORACTION.
  BOR3V3THRESH:
	BOR 3v3 threshold. Select the BOR threshold voltage level for the 3v3 regulator
        0x0: 2.16V
        0x1: 2.22V
        0x2: 2.29V
        0x3: 2.36V
        0x4: 2.43V
        0x5: 2.51V
        0x6: 2.59V
        0x7: 2.68V
        0x8: 2.78V
        0x9: 2.89V
        0xa: 3.00V
        0xb: 3.12V
        0xc: 3.25V
        0xd: 3.55V
        0xe: 3.91V
        0xf: 4.35V
   BOR2V6THRESH:
	BOR 2v6 threshold(Obsolete). Select the BOR threshold voltage level for the 2v6 regulator
        0x0: 1.74V
        0x1: 1.78V
        0x2: 1.83V
        0x3: 1.87V
        0x4: 1.92V
        0x5: 1.97V
        0x6: 2.02V
        0x7: 2.07V
        0x8: 2.13V
        0x9: 2.19V
        0xa: 2.26V
        0xb: 2.32V
        0xc: 2.40V
        0xd: 2.56V
        0xe: 2.74V
        0xf: 2.95V  
    BOR1V8THRESH:
        BOR 1v5 threshold. Select the BOR threshold voltage level for the 1v8 regulator
        0x0: 1.31V
        0x1: 1.34V
        0x2: 1.37V
        0x3: 1.40V
        0x4: 1.44V
        0x5: 1.48V
        0x6: 1.51V
        0x7: 1.56V
        0x8: 1.60V
        0x9: 1.64V
        0xa: 1.69V
        0xb: 1.74V
        0xc: 1.80V
        0xd: 1.92V
        0xe: 2.06V
        0xf: 2.21V 
      BORBIASOVERRIDEENA:
          BOR bias override bit. Independently controllable test bit which can be used to override the BOR bias enable signal. Set BOR_REF_SEL=1 to enable the use of this test bit.
      BORBIASOVERRIDESEL:
          BOR bias override select. Independently controllable test bit which can be used to override the BOR bias enable signal. Set BOR_REF_SEL=1 to enable the use of the BOR_REF_ENA register test bit.
          0x0: Functional Mode
          0x1: Override Mode- Use BOR_BIAS_OVERRIDE_ENA register to control bias.
*/
typedef struct {
    uint32_t BOR3V3THRESH       :  4; /*!< BOR 3v3 threshold */
    uint32_t                   : 12; /*    (reserved) */
    uint32_t BOR1V5THRESH       :  4; /*!< BOR 1v5 threshold */
    uint32_t                   :  4; /*    (reserved) */
    uint32_t BORBIASOVERRIDEENA :  1; /*!< BOR bias override bit */
    uint32_t                   :  3; /*    (reserved) */
    uint32_t BORBIASOVERRIDESEL :  1; /*!< BOR bias override select */
    uint32_t                   :  3; /*    (reserved) */
} BORCONFIG_t;

/**
 * @brief A structure to represent Special Function Registers for WDTACTION.
  WDTACTION:
    Watchdog action. Defines the consequences of watchdog bark being detected by the hardware.
    0x0: IRQ generated
    0x1: Hard reset generated
*/
typedef struct {
    uint32_t WDTACTION:  1;
    uint32_t          : 31; /*    (reserved) */
}WDTACTION_t;
  

/**
 * @brief A structure to represent Special Function Registers for LFCLKKILL.
*/
typedef struct {
    uint32_t KILLLFRC:  1;/*Kill slow RC oscillator. Setting this bit gates the low frequency RC oscillator input*/
    uint32_t         : 31; /*    (reserved) */
}LFCLKKILL_t;
  
/**
 * @brief A structure to represent Special Function Registers for CPCLKCTRL.
  PMUCPSEL:
    PMU charge pump select. Setting this bit will override the enable signal to the PMU charge pump clock gate.
    0x0: The PMU charge pump clock gate is enabled by two sources by default: 
          1. The PMU state machine 'bring-up/active' states (i.e. hardware driven) 
          2. The High Frequency Oscillator being active *Note* both #1 and #2 soures must be active.
    0x1: The value of the PMU_CP_REG field is what is used to drive the enable signal
         on the PMU charge pump clock gate. *Note* this allows the charge pump clock to 
         be driven even with the slow RC oscillator (10KHz).
  PMUCPREG:
    PMU charge pump override register. If the PMU_CP_SEL bit is high, setting this bit will enable the PMU charge pump clock gate.
  PMUCPDIVSEL:
    PMU charge pump divider select. Selects the divide value for the PMU Charge Pump Clock.
    0x0: No Division. Full Clock speed.
    0x1: Div by 2.
    0x2: Div by 4.
    0x3: Div by 8.
    0x4: Div by 16.
    0x5: Div by 32.
  TXLINCPSEL:
    TXLIN charge pump select. Setting this bit will override the enable signal to the TXLIN charge pump clock gate.
    0x0: The TXLIN charge pump clock gate is enabled by two sources by default: 
        1. The PMU state machine 'bring-up/active' states (i.e. hardware driven) 
        2. The High Frequency Oscillator being active *Note* both #1 and #2 soures must be active.
    0x1: The value of the TXLIN_CP_REG field is what is used to drive the enable 
        signal on the TXLIN charge pump clock gate. *Note* this allows the charge pump clock to be driven 
        even with the slow RC oscillator (10KHz).
  TXLINCPREG:
    TXLIN charge pump override register. If the TXLIN_CP_SEL bit is high, setting this bit will enable the TXLIN charge pump clock gate.
  TXLINCPDIVSEL:
    TxLIN charge pump divider select. Selects the divide value for the TxLIN Charge Pump Clock.
    0x0: No Division. Full Clock speed.
    0x1: Div by 2.
    0x2: Div by 4.
    0x3: Div by 8.
    0x4: Div by 16.
    0x5: Div by 32.
*/
typedef struct {
    uint32_t PMUCPSEL      :  1; /*!< PMU charge pump select */
    uint32_t PMUCPREG      :  1; /*!< PMU charge pump override register */
    uint32_t               :  2; /*    (reserved) */
    uint32_t PMUCPDIVSEL   :  3; /*!< PMU charge pump divider select */
    uint32_t               :  9; /*    (reserved) */
    uint32_t TXLINCPSEL    :  1; /*!< TXLIN charge pump select */
    uint32_t TXLINCPREG    :  1; /*!< TXLIN charge pump override register */
    uint32_t               :  2; /*    (reserved) */
    uint32_t TXLINCPDIVSEL :  3; /*!< TxLIN charge pump divider select */
    uint32_t               :  9; /*    (reserved) */
} CPCLKCTRL_t;


typedef struct {
    uint32_t OVTEMP:  2;
    uint32_t       : 30; /*    (reserved) */
}OVTEMPACTION_t;

typedef struct {
    uint32_t OVTEMPLEVEL        :  4; /*!< Over Temp threshold */
    uint32_t                    : 20; /*    (reserved) */
    uint32_t OVTEMPENA          :  1; /*!< OverTemp Monitor Enable bit */
    uint32_t TEMP_SENSOR_DIS    :  1; /*!< temperature sensor disable bit */
    uint32_t                    :  6; /*    (reserved) */
}OVTEMPCONFIG_t;

typedef struct {
    LFCLKCTRL_t    LFCLKCTRL;
    SYSCLKCTRL_t   SYSCLKCTRL;
    RESETCTRL_t    RESETCTRL;
    BORACTION_t    BORACTION;
    BORCONFIG_t    BORCONFIG;
    WDTACTION_t    WDTACTION;
    LFCLKKILL_t    LFCLKKILL;
    CPCLKCTRL_t    CPCLKCTRL;
    OVTEMPACTION_t OVTEMPACTION;
    OVTEMPCONFIG_t OVTEMPCONFIG;
} CRGA_SFRS_t;

/**
 * @brief The starting address of CRGA SFRS.
 */
#define CRGA_SFRS ((__IO CRGA_SFRS_t *)0x50000000)

#endif /* end of __CRGA_SFR_H__ section */

