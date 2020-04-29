/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file pwm_device.c
 */


#include <stdint.h>
#include <string.h>
#include "realplum.h"
#include "errno.h"
#include "pwm_device.h"
#include "appConfig.h"
#include <isrfuncs.h>
#include <adc_device.h>
#if defined PWM_SFRS


static PwmIsrCallback_t  pwmMatchCallback[PWM_CHANNEL_NUM] = {NULL,NULL,NULL};
static PwmIsrCallback_t  pwmReloadCallback[PWM_CHANNEL_NUM] = {NULL,NULL,NULL};

static uint16_t currPWM[3];

void PWM_Handler(void)
{
    uint32_t pwm_status = PWM_SFRS->INTSTATUS.STATUS;
    for (uint32_t i = 0U; i < PWM_CHANNEL_NUM; i++){
        if (((pwm_status &(1UL << i))!= 0U) || ((pwm_status &(1UL << (i+4U)))!= 0U) ){
            if (pwmMatchCallback[i] != NULL){
                pwmMatchCallback[i](pwm_status);
            }
            PWM_SFRS->INTCTRL.CLEAR =(1UL << i) | (1UL << (i+4U));
        }
        if ((PWM_SFRS->INTUPDATED.STATUS & (1UL << i))!= 0U ){
            if (pwmReloadCallback[i] != NULL){
                pwmReloadCallback[i](PWM_SFRS->INTUPDATED.STATUS);
            }
            PWM_SFRS->INTUPDATED.CLEAR = (1UL << i);
        }
    }
}

/**
 * @brief The instance of PMW device
 */
void PWM_Init(PwmChannel_t channel, PwmPrescaler_t divide, uint16_t period, uint8_t invertEn)
{
    IOCTRLA_SFRS->LED.HWMODE |= (1UL << channel);
    PWM_SFRS->BASE[channel].PRESCALESEL = (uint32_t)divide;
    PWM_SFRS->BASE[channel].PERIOD = period;
    PWM_SFRS->CTRL.INVERT |= (uint32_t)invertEn << channel;
    
    /* set duty cycle to 0*/
    PWM_SFRS->PULSE[channel].PFALL = 0U;
    PWM_SFRS->PULSE[channel].PRISE = 0U;
    
}


/* matchRisingValue <= matchFaillValue
 matchRisingValue <= matchFaillValue
*/
/**
 * @brief The instance of PMW device
 */
void PWM_SetMatchValue(PwmChannel_t channel, uint16_t matchRisingValue, uint16_t matchFaillValue)
{
    PWM_SFRS->PULSE[channel].PRISE = matchRisingValue;
    PWM_SFRS->PULSE[channel].PFALL = matchFaillValue;
}

/**
 * @brief The instance of PMW device
 */
void PWM_StartAndUpdate(void)
{
    PWM_SFRS->CTRL.ENAREQ = (1UL << PWM_CHANNEL_0) | (1UL << PWM_CHANNEL_1) | (1UL << PWM_CHANNEL_2);
    PWM_SFRS->CTRL.UPDATE = (1UL << PWM_CHANNEL_0) | (1UL << PWM_CHANNEL_1) | (1UL << PWM_CHANNEL_2);
}



/**
 * @brief The instance of PWM_SetRGBValue
 */
void PWM_SetRGBValue(uint16_t red, uint16_t green, uint16_t blue)
{
    uint16_t r,g,b;
    r= red   >> (16U-PWM_VALUE_MAX_POS);
    g= green >> (16U-PWM_VALUE_MAX_POS);
    b= blue  >> (16U-PWM_VALUE_MAX_POS);
    
    currPWM[PHY_CHANNEL_RED] = r;
    currPWM[PHY_CHANNEL_GREEN] = g;
    currPWM[PHY_CHANNEL_BLUE] = b;
    /*
    if (r == 0U){
        PWM_SetMatchValue(PHY_CHANNEL_RED,   1U, r);
    }else{
        PWM_SetMatchValue(PHY_CHANNEL_RED,   0U, r);
    }
    PWM_SetMatchValue(PHY_CHANNEL_GREEN, ((uint16_t)PWM_VALUE_MAX - g)>>1U, (uint16_t)(((uint32_t)PWM_VALUE_MAX + (uint32_t)g) >> 1U) );
    PWM_SetMatchValue(PHY_CHANNEL_BLUE,  ((uint16_t)PWM_VALUE_MAX - b),     (uint16_t)PWM_VALUE_MAX);
    */
    PWM_SetMatchValue(PHY_CHANNEL_RED,  ((uint16_t)PWM_VALUE_MAX - r),     (uint16_t)PWM_VALUE_MAX);
    PWM_SetMatchValue(PHY_CHANNEL_GREEN,((uint16_t)PWM_VALUE_MAX - g),     (uint16_t)PWM_VALUE_MAX);
    PWM_SetMatchValue(PHY_CHANNEL_BLUE, ((uint16_t)PWM_VALUE_MAX - b),     (uint16_t)PWM_VALUE_MAX);
    
    PWM_StartAndUpdate();
}

int8_t PWM_UpdateFInished(void)
{
    int8_t result = -1;
    uint32_t update = PWM_SFRS->CTRL.UPDATE;
    if (update == 0U){
        result = 0;
    }
    return result;
}

uint16_t PWM_GetCurrentPWM(uint8_t channel)
{
    return currPWM[channel];
}

int8_t PWM_AllChannelsAreOff(void)
{
    int8_t result = -1;
    if ( (currPWM[0] == 0U) && (currPWM[1] == 0U) && (currPWM[2] == 0U) ){
        result = 0;
    }
    return result;
}
/**
 * @brief The instance of PMW device
 */
void PWM_RegisterPWMMatchIRQ(PwmChannel_t channel, PWMTriggerEdge_t edge, PwmIsrCallback_t callback)
{
    pwmMatchCallback[channel] = callback;
    if (edge == PWM_TRIGGER_POS_EDGE){
        PWM_SFRS->INTCTRL.ENABLE |=( 1UL << (uint16_t)channel);
    }else{
        PWM_SFRS->INTCTRL.ENABLE |=( 1UL << ((uint16_t)channel + 4U));
    }
    PWM_SFRS->CTRL.UPDATE |= (1UL << (uint16_t)channel);
    NVIC_EnableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_UnRegisterPWMMatchIRQ(PwmChannel_t channel)
{
    pwmMatchCallback[channel] = NULL;
    PWM_SFRS->INTCTRL.ENABLE &=~( 1UL << (uint16_t)channel);
    PWM_SFRS->INTCTRL.ENABLE &=~( 1UL << ((uint16_t)channel + 4U));
    PWM_SFRS->CTRL.UPDATE |= (uint32_t)(1UL << (uint16_t)channel);
    NVIC_DisableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_RegisterPWMReloadIRQ(PwmChannel_t channel, PwmIsrCallback_t callback)
{
    pwmReloadCallback[channel] = callback;
    PWM_SFRS->INTUPDATED.ENABLE |=( 1UL << (uint16_t)channel );
    PWM_SFRS->CTRL.UPDATE |= (1UL << (uint16_t)channel);
    NVIC_EnableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_UnRegisterPWMReloadIRQ(PwmChannel_t channel)
{
    pwmReloadCallback[channel] = NULL;
    PWM_SFRS->INTUPDATED.ENABLE &=~(1UL << channel);
    PWM_SFRS->CTRL.UPDATE |= (1UL << channel);
    NVIC_DisableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_Enable(PwmChannel_t channel)
{
    PWM_SFRS->CTRL.ENAREQ |= (1UL << channel);
    PWM_SFRS->CTRL.UPDATE |= (1UL << channel);
}
/**
 * @brief The instance of PMW device
 */
void PWM_EnableAllChannels(void)
{
    uint32_t update;
    PWM_SFRS->CTRL.ENAREQ = 0x07U;
    PWM_SFRS->CTRL.UPDATE = 0x07U;
    if ( CRGA_SFRS->RESETCTRL.PORFLAG == 0U){/*  it's not a power on reset    */
        /* update first until taking effect */
        for(;;){
            update = PWM_SFRS->CTRL.UPDATE;
            if (update == 0U){
               break;
            }
        }
    }else{
        CRGA_SFRS->RESETCTRL.PORFLAGCLR = 1U;/*  clear power on reset flag */
    }
}

/**
 * @brief The instance of PMW device
 */
void PWM_Disable(PwmChannel_t channel)
{
    PWM_SFRS->CTRL.ENAREQ &= ~(1UL << channel);
    PWM_SFRS->CTRL.UPDATE |= (1UL << channel);
}

/**
 * @brief The instance of PMW device
 */
void PWM_DisableAllChannels(void)
{
    PWM_SFRS->CTRL.ENAREQ = 0x00U;
    PWM_SFRS->CTRL.UPDATE = 0x07U;
}
/**
 * @brief The instance of PMW device
 */
void PWM_SetPrescaler(PwmChannel_t channel, PwmPrescaler_t divide)
{
    PWM_SFRS->BASE[channel].PRESCALESEL = (uint32_t)divide;
    PWM_SFRS->CTRL.UPDATE |= (1UL << channel);
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetPeriod(PwmChannel_t channel, uint16_t period)
{
    PWM_SFRS->BASE[channel].PERIOD = period;
    PWM_SFRS->CTRL.UPDATE |= (1UL << channel);
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetInvert(PwmChannel_t channel, uint8_t invertEn)
{
    PWM_SFRS->CTRL.INVERT |= (uint32_t)((uint32_t)invertEn << channel);
    PWM_SFRS->CTRL.UPDATE |= (uint32_t)(1UL << channel);
}

/**
 * @brief The instance of PMW device
 */
int8_t PWM_SetMaxChannelCurrent(PwmChannel_t channel, CurrValue_t current,OffCurrValue_t offCurrent)
{
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0EU;
    if (channel == PWM_CHANNEL_0){
        SYSCTRLA_SFRS->LED0.TRIM0     = HWCFG_GetLED0TrimValue(current,TEMPERATURE_25C);
        SYSCTRLA_SFRS->LED0.TRIM0_OFF = HWCFG_GetOffLED0TrimValue(offCurrent,TEMPERATURE_25C);
    }else if (channel == PWM_CHANNEL_1){
        SYSCTRLA_SFRS->LED1.TRIM1     = HWCFG_GetLED1TrimValue(current,TEMPERATURE_25C);
        SYSCTRLA_SFRS->LED1.TRIM1_OFF = HWCFG_GetOffLED1TrimValue(offCurrent,TEMPERATURE_25C);
    }else{
        SYSCTRLA_SFRS->LED1.TRIM2     = HWCFG_GetLED2TrimValue(current,TEMPERATURE_25C);
        SYSCTRLA_SFRS->LED1.TRIM2_OFF = HWCFG_GetOffLED2TrimValue(offCurrent,TEMPERATURE_25C);
    }
    
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0EU;
    SYSCTRLA_SFRS->PMU_TRIM.RESISTOR_TRIM =  HWCFG_GetV2ITrimValue(current);
/*    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_LOCK = 1u;*/
    return 0;
}

void PWM_TurnOffChannelCurrent(void)
{
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0EU;
    SYSCTRLA_SFRS->LED0.TRIM0     = 0;
    SYSCTRLA_SFRS->LED0.TRIM0_OFF = 0;
    SYSCTRLA_SFRS->LED1.TRIM1     = 0;
    SYSCTRLA_SFRS->LED1.TRIM1_OFF = 0;
    SYSCTRLA_SFRS->LED1.TRIM2     = 0;
    SYSCTRLA_SFRS->LED1.TRIM2_OFF = 0;
/*    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_LOCK = 1u;*/

}

/*
 * adjust channel max current as the temperature changes  1: means 0.1mA typically
 *  @param [in] channel: Led cahnnel
 *  @param [in] current: -32~32: -3.2mA ~ 3.2mA typically
 *  @return 0
 */
int8_t PWM_SetCompensationCurrent(PwmChannel_t channel,CurrValue_t maxCurrent, int8_t current)
{
    int16_t trim;
    uint16_t uTrim;
    int8_t result;
    if ( (current >= -32) && (current <= 32) ){
        result = 0;
        SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0EU;
        if (channel == PWM_CHANNEL_0){
            trim = (int16_t)HWCFG_GetLED0TrimValue(maxCurrent,TEMPERATURE_25C);
            trim = trim + current;
            uTrim = (uint16_t)trim;
            SYSCTRLA_SFRS->LED0.TRIM0     = uTrim;
        }else if (channel == PWM_CHANNEL_1){
            trim = (int16_t)HWCFG_GetLED1TrimValue(maxCurrent,TEMPERATURE_25C);
            trim = trim + current;
            uTrim = (uint16_t)trim;
            SYSCTRLA_SFRS->LED1.TRIM1     = uTrim;
        }else{
            trim = (int16_t)HWCFG_GetLED2TrimValue(maxCurrent,TEMPERATURE_25C);
            trim = trim + current;
            uTrim = (uint16_t)trim;
            SYSCTRLA_SFRS->LED1.TRIM2     = uTrim;
        }
    }else{
        result = -1;
    }
    return result;
}



#endif