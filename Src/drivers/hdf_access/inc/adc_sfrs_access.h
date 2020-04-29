/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file adc_sfrs_access.h
 * This file is created for Bon.
 */
 
#ifndef __ADC_SFRS_H__
#define __ADC_SFRS_H__

#include "iceblue.h"

/**
 * @brief Enumerations of ADC channels
 */
#define E_ADC_CNTRL_CHSEQ_CH0    0x00
#define E_ADC_CNTRL_CHSEQ_CH1    0x01
#define E_ADC_CNTRL_CHSEQ_CH2    0x02
#define E_ADC_CNTRL_CHSEQ_CH3    0x04
#define E_ADC_CNTRL_CHSEQ_CH4    0x08
#define E_ADC_CNTRL_CHSEQ_CH5    0x10

#define E_ADC_CNTRL_CHSEQ_CH12   0x21
#define E_ADC_CNTRL_CHSEQ_CH13   0x29
#define E_ADC_CNTRL_CHSEQ_CH23   0x22
#define E_ADC_CNTRL_CHSEQ_CH21   0x2A
#define E_ADC_CNTRL_CHSEQ_CH31   0x24
#define E_ADC_CNTRL_CHSEQ_CH32   0x2C

#define E_ADC_CNTRL_CHSEQ_CH123  0x31
#define E_ADC_CNTRL_CHSEQ_CH132  0x39
#define E_ADC_CNTRL_CHSEQ_CH231  0x32
#define E_ADC_CNTRL_CHSEQ_CH213  0x3A
#define E_ADC_CNTRL_CHSEQ_CH312  0x34
#define E_ADC_CNTRL_CHSEQ_CH321  0x3C

#define E_ADC_CNTRL_CHSEQ_CH123  0x31
#define E_ADC_CNTRL_CHSEQ_CH132  0x39
#define E_ADC_CNTRL_CHSEQ_CH231  0x32
#define E_ADC_CNTRL_CHSEQ_CH213  0x3A
#define E_ADC_CNTRL_CHSEQ_CH312  0x34
#define E_ADC_CNTRL_CHSEQ_CH321  0x3C

/**
 * @brief Enumerations of ADC sample cycles
 */
#define E_ADC_CONF_SAMPCYC_1    0
#define E_ADC_CONF_SAMPCYC_2    1
#define E_ADC_CONF_SAMPCYC_4    2
#define E_ADC_CONF_SAMPCYC_8    3
#define E_ADC_CONF_SAMPCYC_16   4
#define E_ADC_CONF_SAMPCYC_32   5
#define E_ADC_CONF_SAMPCYC_48   6
#define E_ADC_CONF_SAMPCYC_64   7

/**
 * @brief Enumerations of ADC startup delay
 */
#define E_ADC_CNTRL_STUPDLY_1us         0
#define E_ADC_CNTRL_STUPDLY_8us         1
#define E_ADC_CNTRL_STUPDLY_12us        2
#define E_ADC_CNTRL_STUPDLY_16us        3

/**
 * @brief Select ADC channel.
 *
 * @param channel The enumeration of ADC channel.
 */
static __INLINE void f_ADC_SelectChannel(uint8_t channel)
{
    ADC_SFRS->CNTRL.CHSEQ = channel;
}

/**
 * @brief Select ADC startup delay.
 *
 * @param delay The enumeration of ADC startup delay.
 */
static __INLINE void f_ADC_SelectStartupDelay(uint8_t delay)
{
    ADC_SFRS->CNTRL.STUPDLY = delay;
}

/**
 * @brief Set ADC sample cycle.
 *
 * @param cycle The enumeration for sample cycle.
 */
static __INLINE void f_ADC_SampleCycle(uint8_t cycle)
{
    ADC_SFRS->CONF.SAMPCYC = cycle;
}

/**
 * @brief Read ADC result for Channel 1.
 *
 * @return The result of last ADC conversion.
 */
static __INLINE uint16_t f_ADC_Read_Data1(void)
{
    return ADC_SFRS->DATA1;
}

/**
 * @brief Read ADC result for Channel 2.
 *
 * @return The result of last ADC conversion.
 */
static __INLINE uint16_t f_ADC_Read_Data2(void)
{
    return ADC_SFRS->DATA2;
}

/**
 * @brief Read ADC result for Channel 0/3/4/5.
 *
 * @return The result of last ADC conversion.
 */
static __INLINE uint16_t f_ADC_Read_DataX(void)
{
    return ADC_SFRS->DATA0345;
}

/**
 * @brief Start ADC conversion.
 */
static __INLINE void f_ADC_Start(void)
{
    ADC_SFRS->CNTRL.CONVERT = 1;
}

/**
 * @brief Check ADC progress.
 *
 * @return The status of a ADC conversion.
 */
static __INLINE uint8_t f_ADC_ConversionInProgress()
{
    return ADC_SFRS->CNTRL.CONVERT;
}

/**
 * @brief Enable ADC interrupt.
 *
 */
static __INLINE void f_ADC_EnableIRQ(void)
{
    ADC_SFRS->CNTRL.IRQENA = 1;
}

/**
 * @brief Disable ADC interrupt.
 *
 */
static __INLINE void f_ADC_DisableIRQ(void)
{
    ADC_SFRS->CNTRL.IRQENA = 0;
}

/**
 * @brief Clear ADC interrupt.
 *
 */
static __INLINE void f_ADC_ClearIRQ(void)
{
    ADC_SFRS->CNTRL.IRQCLR = 1;
}

/**
 * @brief Is ADC conversion done.
 *
 * @return The status of a ADC conversion.
 */
static __INLINE uint8_t f_ADC_ConversionDone()
{
    return ADC_SFRS->STATUS.CONVDONE;
}

/**
 * @brief Enable ADC Sync mode.
 *
 */
static __INLINE void f_ADC_EnableSync(void)
{
    ADC_SFRS->CNTRL.SYNCENA = 1;
}

/**
 * @brief Disable ADC Sync mode.
 *
 */
static __INLINE void f_ADC_DisableSync(void)
{
    ADC_SFRS->CNTRL.SYNCENA = 0;
}

/**
 * @brief Enable ADC Continuous mode.
 *
 */
static __INLINE void f_ADC_EnableContinuousMode(void)
{
    ADC_SFRS->CNTRL.CONT = 1;
}

/**
 * @brief Disable ADC Continuous mode.
 *
 */
static __INLINE void f_ADC_DisableContinuousMode(void)
{
    ADC_SFRS->CNTRL.CONT = 0;
}

/**
 * @brief Enable ADC Input Attenuation.
 *
 */
static __INLINE void f_ADC_EnableAttenuationMode(void)
{
    ADC_SFRS->CONF.ATTEN = 1;
}

/**
 * @brief Disable ADC Input Attenuation.
 *
 */
static __INLINE void f_ADC_DisableAttenuationMode(void)
{
    ADC_SFRS->CONF.ATTEN = 0;
}

/**
 * @brief Set ADC timing.
 *
 * @param guard The idle time between channel selection in sequential conversion.
 * @param chnl The time for channel settling after channel selection in sequential conversion.
 * @param curr The time delay to start conversion.
 */
static __INLINE void f_ADC_SetADCTimingGuard(uint8_t guard)
{
    ADC_SFRS->TSET.TGUARD = guard;
}

/**
 * @brief Set ADC timing.
 *
 * @param chnl The time for channel settling after channel selection in sequential conversion.
 */
static __INLINE void f_ADC_SetADCTimingChnl(uint8_t chnl)
{
    ADC_SFRS->TSET.TCHNL = chnl;
}

/**
 * @brief Set ADC timing.
 *
 * @param curr The time delay to start conversion.
 */
static __INLINE void f_ADC_SetADCTimingCurr(uint8_t curr)
{
    ADC_SFRS->TSET.TCURR = curr;
}

#endif /* __ADC_SFRS_H__ */