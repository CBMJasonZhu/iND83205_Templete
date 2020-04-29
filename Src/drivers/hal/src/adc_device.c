/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file adc_device.c
 */

#include "adc_device.h"
#include <isrfuncs.h>
static AdcMeasureItem_t adcMeasureItem = ADC_MEASURE_ITEM_NONE;
static adc_cb_func_t adcCallback = NULL;

void ADC_Handler(void)
{
    uint16_t adcResult[2];
    if (adcMeasureItem < ADC_MEASURE_ITEM_VBAT){
        if (adcCallback !=NULL){
            adcResult[0] = (uint16_t)(ADC_SFRS->DATA0345 & 0x0FFFU);
            adcCallback(adcMeasureItem, adcResult);
        }
    }else if (adcMeasureItem == ADC_MEASURE_ITEM_VBAT){
        if (adcCallback !=NULL){
            adcResult[0] = (uint16_t)(ADC_SFRS->DATA1 & 0x0FFFU); /*Battery volt */
            adcCallback(adcMeasureItem, adcResult);
        }
    }else if (adcMeasureItem <= ADC_MEASURE_ITEM_VLED2_VBAT){
        if (adcCallback !=NULL){
            adcResult[0] = (uint16_t)(ADC_SFRS->DATA2 & 0x0FFFU); /*Led PN */
            adcResult[1] = (uint16_t)(ADC_SFRS->DATA1 & 0x0FFFU); /*Battery volt */
            adcCallback(adcMeasureItem, adcResult);
        }
    }else{
    }
    ADC_SFRS->CNTRL.IRQCLR = 1U;
}

void ADC_GeneralInit(void)
{
    ADC_SFRS->CONF.AUTOEN = 0U; 
    ADC_SFRS->CONF.SAMPCYC  = (uint32_t)ADC_SAMPLING_CYCLE_8;
    ADC_SFRS->CONF.MODE     = (uint32_t)ADC_MODE_SINGLE_END;
    ADC_SFRS->CNTRL.STUPDLY = (uint32_t)ADC_STARTUP_DELAY_1US;
    ADC_SFRS->CNTRL.SYNCENA = 0U; /* disable sync convert mode */
    ADC_SFRS->CNTRL.CONT    = 0U; /* singel convert mode */
    ADC_TSET_REG = (uint32_t)(((uint32_t)ADC_WAIT_TIME_1000NS << TSET_TCURR_POS) | ((uint32_t)ADC_WAIT_TIME_1000NS << TSET_TCHNL_POS) | ((uint32_t)ADC_WAIT_TIME_250NS << TSET_TGUARD_POS));
}


void ADC_Init(AdcMeasureItem_t item, uint8_t syncEnable)
{
    adcMeasureItem = item;
    switch(item){
    case ADC_MEASURE_ITEM_VLED0_VBAT:/* 1/4 x  1/16x*/
      ADC_SFRS->CNTRL.CHSEQ  = (uint32_t)ADC_CH2_THEN_CH1;
      ADC_SFRS->CNTRL.CH2SEL = (uint32_t)ADC_CH2_SEL_LED0;
      ADC_SFRS->CNTRL.SYNCEDGE = (uint32_t)SYNC_EDGE_FALLING;
      ADC_SFRS->CNTRL.SYNCENA = syncEnable;
      break;
      
    case ADC_MEASURE_ITEM_VLED1_VBAT:/* 1/4 x  1/16x*/
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH2_THEN_CH1;
      ADC_SFRS->CNTRL.CH2SEL = (uint32_t)ADC_CH2_SEL_LED1;
      ADC_SFRS->CNTRL.SYNCEDGE = (uint32_t)SYNC_EDGE_FALLING;
      ADC_SFRS->CNTRL.SYNCENA = syncEnable;
      break;
      
    case ADC_MEASURE_ITEM_VLED2_VBAT:/* 1/4x 1/16x*/
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH2_THEN_CH1;
      ADC_SFRS->CNTRL.CH2SEL = (uint32_t)ADC_CH2_SEL_LED2;
      ADC_SFRS->CNTRL.SYNCEDGE = (uint32_t)SYNC_EDGE_FALLING;
      ADC_SFRS->CNTRL.SYNCENA = syncEnable;
      break;
           
    case ADC_MEASURE_ITEM_VBAT:/*1/16 x */
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH1;
      break;
      
    case ADC_MEASURE_ITEM_VTEMP:/* 1x */
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH3;
      break;
      
    case ADC_MEASURE_ITEM_VBG:
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH0;
      break; 
      
    case ADC_MEASURE_ITEM_GPIO1:/* 1x / 1/3x if ADC_SFRS->CONF.ATTEN = 1; */
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH4;
      ADC_SFRS->CNTRL.CH4SEL = (uint32_t)ADC_CH4_SEL_GPIO1;
      ADC_SFRS->CONF.ATTEN = 1U;
      break;
      
    case ADC_MEASURE_ITEM_GPIO2:/* 1x / 1/3x if ADC_SFRS->CONF.ATTEN = 1; */
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH4;
      ADC_SFRS->CNTRL.CH4SEL = (uint32_t)ADC_CH4_SEL_GPIO2;
      ADC_SFRS->CONF.ATTEN = 1U;
      break;
      
    case ADC_MEASURE_ITEM_GPIO3:/* 1x / 1/3x if ADC_SFRS->CONF.ATTEN = 1; */
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH4;
      ADC_SFRS->CNTRL.CH4SEL = (uint32_t)ADC_CH4_SEL_GPIO3;
      ADC_SFRS->CONF.ATTEN = 1U;
      break;
      
    case ADC_MEASURE_ITEM_GPIO4:/* 1x / 1/3x if ADC_SFRS->CONF.ATTEN = 1; */
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH4;
      ADC_SFRS->CNTRL.CH4SEL = (uint32_t)ADC_CH4_SEL_GPIO4;
      ADC_SFRS->CONF.ATTEN = 1U;
      break;
      
    case ADC_MEASURE_ITEM_VDD1V5:/* 1x / 1/3x if ADC_SFRS->CONF.ATTEN = 1; */
      ADC_SFRS->CNTRL.CHSEQ = (uint32_t)ADC_CH5;
      ADC_SFRS->CONF.ATTEN = 1U;
      break;
      
    default:
      break;
    }
}

void ADC_UnInit(AdcMeasureItem_t item)
{
    
  
}

/**
 * @brief Register a callback function to ADC interrupt handler.
 *
 * @param dev Pointer to the ADC device.
 * @param cb The callback function to be registered.
 * @return 0 for success or error code upon a failure.
 */
void ADC_RegisterIRQ(adc_cb_func_t callback)
{
    adcCallback = callback;
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.IRQENA = 1U;
    NVIC_EnableIRQ(ADC_IRQn);
    
}

/**
 * @brief Unregister a callback function to ADC interrupt handler.
 *
 * @param dev Pointer to the ADC device.
 * @param cb The callback function to be unregistered.
 * @return 0 for success or error code upon a failure.
 */
void ADC_UnregisterIRQ(void)
{
    adcCallback = NULL;
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.IRQENA = 0U;
    NVIC_DisableIRQ(ADC_IRQn);
}

void ADC_Start(void)
{
    ADC_SFRS->CNTRL.IRQCLR = 1U;
    ADC_SFRS->CNTRL.CONVERT = 1U;
}
