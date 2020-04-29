/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file adc_device.h
 */

#ifndef ADC_DEVICE_H__
#define ADC_DEVICE_H__

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "realplum.h"
#include "adc_sfr.h"

typedef enum {
  ADC_MEASURE_ITEM_NONE = 0U,
  ADC_MEASURE_ITEM_VBG,
  ADC_MEASURE_ITEM_VTEMP,
  ADC_MEASURE_ITEM_GPIO1,
  ADC_MEASURE_ITEM_GPIO2,
  ADC_MEASURE_ITEM_GPIO3,
  ADC_MEASURE_ITEM_GPIO4,
  ADC_MEASURE_ITEM_VDD1V5,
  ADC_MEASURE_ITEM_VBAT,
  ADC_MEASURE_ITEM_VLED0_VBAT,
  ADC_MEASURE_ITEM_VLED1_VBAT,
  ADC_MEASURE_ITEM_VLED2_VBAT,
}AdcMeasureItem_t;



typedef enum {
  ADC_SAMPLING_CYCLE_1 = 0U,
  ADC_SAMPLING_CYCLE_2,
  ADC_SAMPLING_CYCLE_4,
  ADC_SAMPLING_CYCLE_8,
  ADC_SAMPLING_CYCLE_16,
  ADC_SAMPLING_CYCLE_32,
  ADC_SAMPLING_CYCLE_48,
  ADC_SAMPLING_CYCLE_64,
}AdcSamplingCycle_t;

typedef enum {
  ADC_MODE_DIFF = 0U,
  ADC_MODE_SINGLE_END,
}AdcMode_t;

#define CONF_MODE_POS                   (0U)
#define CONF_AUTOEN_POS                 (1U)
#define CONF_ATTEN_POS                  (2U)
#define CONF_SAMPLECYC_POS              (3U)
#define CONF_SWSYNCIN_POS               (6U)
#define CONF_TRMLEDBUFFER4ADC_POS       (16U)


typedef enum {
  ADC_STARTUP_DELAY_1US = 0U,
  ADC_STARTUP_DELAY_8US,
  ADC_STARTUP_DELAY_12US,
  ADC_STARTUP_DELAY_16US,
}AdcStartupDelay_t;


typedef enum {
  ADC_CH4_SEL_GPIO1 = 0U,
  ADC_CH4_SEL_GPIO2,
  ADC_CH4_SEL_GPIO3,
  ADC_CH4_SEL_GPIO4,
}AdcCh4Select_t;


typedef enum {
  ADC_CH3_SEL_TEMP_SENSOR = 0U,
  ADC_CH3_SEL_VBAT,
}AdcCh3Select_t;

typedef enum {
  SYNC_EDGE_RISING = 0U,
  SYNC_EDGE_FALLING,
}AdcCh2SyncEdge_t;

typedef enum {
  ADC_CH2_SEL_LED0 = 0U,
  ADC_CH2_SEL_LED1,
  ADC_CH2_SEL_LED2,
}AdcCh2Select_t;


typedef enum {
  ADC_CH0 = 0x00U,
  ADC_CH1 = 0x01U,
  ADC_CH2 = 0x02U,
  ADC_CH3 = 0x04U,
  ADC_CH4 = 0x08U,
  ADC_CH5 = 0x10U,
  ADC_CH1_THEN_CH2 = 0x21U,
  ADC_CH1_THEN_CH3 = 0x29U,
  ADC_CH2_THEN_CH3 = 0x22U,
  ADC_CH2_THEN_CH1 = 0x2AU,
  ADC_CH3_THEN_CH1 = 0x24U,
  ADC_CH3_THEN_CH2 = 0x2CU,
  ADC_CH1_THEN_CH2_THEN_CH3 = 0x31U,
  ADC_CH1_THEN_CH3_THEN_CH2 = 0x39U,
  ADC_CH2_THEN_CH3_THEN_CH1 = 0x32U,
  ADC_CH2_THEN_CH1_THEN_CH3 = 0x3AU,
  ADC_CH3_THEN_CH1_THEN_CH2 = 0x34U,
  ADC_CH3_THEN_CH2_THEN_CH1 = 0x3CU,
}AdcChannelSequence_t;

#define CNTRL_CONVERT_POS       (0U)
#define CNTRL_CONT_POS          (1U)
#define CNTRL_SYNCENA_POS       (2U)
#define CNTRL_STUPDLY_POS       (4U)
#define CNTRL_IRQENA_POS        (6U)
#define CNTRL_IRQCLR_POS        (7U)
#define CNTRL_CHSEQ_POS         (8U)
#define CNTRL_CH2SEL_POS        (16U)
#define CNTRL_CH4SEL_POS        (20U)


typedef enum {
  ADC_WAIT_TIME_250NS = 0x00U,
  ADC_WAIT_TIME_500NS,
  ADC_WAIT_TIME_750NS,
  ADC_WAIT_TIME_1000NS,
  ADC_WAIT_TIME_1250NS,
  ADC_WAIT_TIME_1500NS,
  ADC_WAIT_TIME_1750NS,
  ADC_WAIT_TIME_2000NS,
  ADC_WAIT_TIME_2250NS,
  ADC_WAIT_TIME_2500NS,
  ADC_WAIT_TIME_2750NS,
  ADC_WAIT_TIME_3000NS,
  ADC_WAIT_TIME_3250NS,
}AdcWaittingTime_t;

#define TSET_TCURR_POS       (0U)
#define TSET_TCHNL_POS       (4U)
#define TSET_TGUARD_POS      (8U)

typedef void (*adc_cb_func_t)(AdcMeasureItem_t item, uint16_t *const result);

void ADC_GeneralInit(void);
void ADC_Init(AdcMeasureItem_t item, uint8_t syncEnable);
void ADC_UnInit(AdcMeasureItem_t item);
void ADC_RegisterIRQ(adc_cb_func_t callback);
void ADC_UnregisterIRQ(void);
void ADC_Start(void);


#endif /* __ADC_DEVICE_H__ */