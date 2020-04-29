/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file hwcfg.c
 */

#include <stddef.h>
#include <stdint.h>
#include "realplum.h"
#include "hwcfg.h"


uint8_t HWCFG_GetRCHFClockCalibValue(void)
{
    uint8_t trim;
    if (HWCFG_SFRS->CLK_RC_HF != 0xFFU){
        trim = (uint8_t)HWCFG_SFRS->CLK_RC_HF;
    }else{
        trim = 0x3EU;
    }
    return trim;
}

  
uint8_t HWCFG_GetRCLFClockCalibValue(void)
{
    return HWCFG_SFRS->CLK_RC_LF;
}


uint16_t HWCFG_GetVBG(void)
{
    uint16_t volt;
    
    if (HWCFG_SFRS->BANDGAP_VOLT != 0xFFU){
        volt =  HWCFG_SFRS->BANDGAP_VOLT;
    }else{
        volt = E_HWCFG_ADC_VBG;
    }
    return volt;
}


/**
 * @brief Get the version of calibration data.
 *
 * @return The version for this calibration data block.
 */
uint32_t HWCFG_GetCalibrationVersion(void)
{
    uint32_t version;
    version = HWCFG_SFRS->CHIP.ATE_TEST_REV[2];
    version = (version << 8U) + HWCFG_SFRS->CHIP.ATE_TEST_REV[1];
    version = (version << 8U) + HWCFG_SFRS->CHIP.ATE_TEST_REV[0];
    return version;
  
}

uint16_t HWCFG_GetV2ITrimValue(CurrValue_t value)
{
    uint16_t trimValue;
    if (HWCFG_SFRS->V2I_30mA_25C != 0xFFU){
        trimValue = HWCFG_SFRS->V2I_30mA_25C;
    }else{
        trimValue =  0x20U;
    }
    return trimValue;
}


uint16_t HWCFG_GetLED0TrimValue(CurrValue_t value,TemperatureValue_t temp)
{
    uint16_t trimValue;
    if (temp == TEMPERATURE_25C){
        if ( HWCFG_SFRS->LED0_TRIM_30mA_25C <= 0x1FFU){
            trimValue =  HWCFG_SFRS->LED0_TRIM_30mA_25C;
        }else{
            trimValue =  300U; /* 100uA*300 = 30mA */
        }
    }else{
        if ( HWCFG_SFRS->LED0_TRIM_30mA_125C <= 0x1FFU){
            trimValue =  HWCFG_SFRS->LED0_TRIM_30mA_125C;
        }else{
            trimValue =  300U; /* 100uA*300 = 30mA */
        }
    }
    
    return trimValue;
}

uint16_t HWCFG_GetLED1TrimValue(CurrValue_t value,TemperatureValue_t temp)
{
    uint16_t trimValue;
    if (temp == TEMPERATURE_25C){
        if ( HWCFG_SFRS->LED1_TRIM_30mA_25C <= 0x1FFU){
            trimValue = HWCFG_SFRS->LED1_TRIM_30mA_25C;
        }else{
            trimValue = 300U; /* 100uA*300 = 30mA */
        }
    }else{
        if ( HWCFG_SFRS->LED1_TRIM_30mA_125C <= 0x1FFU){
            trimValue = HWCFG_SFRS->LED1_TRIM_30mA_125C;
        }else{
            trimValue = 300U; /* 100uA*300 = 30mA */
        }
    }
    
    return trimValue;
}

uint16_t HWCFG_GetLED2TrimValue(CurrValue_t value,TemperatureValue_t temp)
{
    uint16_t trimValue;
    if (temp == TEMPERATURE_25C){
        if ( HWCFG_SFRS->LED2_TRIM_30mA_25C <= 0x1FFU){
            trimValue = HWCFG_SFRS->LED2_TRIM_30mA_25C;
        }else{
            trimValue = 300U; /* 100uA*300 = 30mA */
        }
    }else{
        if ( HWCFG_SFRS->LED2_TRIM_30mA_125C <= 0x1FFU){
            trimValue = HWCFG_SFRS->LED2_TRIM_30mA_125C;
        }else{
            trimValue = 300U; /* 100uA*300 = 30mA */
        }
    }
    
    return trimValue;
}


uint16_t HWCFG_GetOffLED0TrimValue(OffCurrValue_t value,TemperatureValue_t temp)
{
    uint16_t trimValue;
    
    if (temp == TEMPERATURE_25C){
        if ( HWCFG_SFRS->LED0_OFF_TRIM_2mA_25C.trim <= 0x1FU){
            trimValue = HWCFG_SFRS->LED0_OFF_TRIM_2mA_25C.trim;
        }else{
            trimValue = 20U; /* 100uA*20 = 2mA */
        } 
      
    }else{
        if ( HWCFG_SFRS->LED0_OFF_TRIM_2mA_125C.trim <= 0x1FU){
            trimValue = HWCFG_SFRS->LED0_OFF_TRIM_2mA_125C.trim;
        }else{
            trimValue = 20U; /* 100uA*20 = 2mA */
        } 
    }
    
    return trimValue;
}

uint16_t HWCFG_GetOffLED1TrimValue(OffCurrValue_t value,TemperatureValue_t temp)
{
    uint16_t trimValue;
    if (temp == TEMPERATURE_25C){
        if ( HWCFG_SFRS->LED1_OFF_TRIM_2mA_25C.trim <= 0x1FU){
            trimValue = HWCFG_SFRS->LED1_OFF_TRIM_2mA_25C.trim;
        }else{
            trimValue = 20U; /* 100uA*20 = 2mA */
        } 
    }else{
        if ( HWCFG_SFRS->LED1_OFF_TRIM_2mA_125C.trim <= 0x1FU){
            trimValue = HWCFG_SFRS->LED1_OFF_TRIM_2mA_125C.trim;
        }else{
            trimValue = 20U; /* 100uA*20 = 2mA */
        } 
    }
    return trimValue;
}

uint16_t HWCFG_GetOffLED2TrimValue(OffCurrValue_t value,TemperatureValue_t temp)
{
    uint16_t trimValue;
    if (temp == TEMPERATURE_25C){
        if ( HWCFG_SFRS->LED2_OFF_TRIM_2mA_25C.trim <= 0x1FU){
            trimValue = HWCFG_SFRS->LED2_OFF_TRIM_2mA_25C.trim;
        }else{
            trimValue = 20U; /* 100uA*20 = 2mA */
        } 
    }else{
        if ( HWCFG_SFRS->LED2_OFF_TRIM_2mA_125C.trim <= 0x1FU){
            trimValue = HWCFG_SFRS->LED2_OFF_TRIM_2mA_125C.trim;
        }else{
            trimValue = 20U; /* 100uA*20 = 2mA */
        }
    }
    return trimValue;
}





























