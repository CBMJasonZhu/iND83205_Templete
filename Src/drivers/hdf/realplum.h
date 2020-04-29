#ifndef ICEBLUE_H__
#define ICEBLUE_H__


#include <stdint.h>

/* declare an enumerated type that can be used to refer to interrupts by name, e.g. in calls to the NVIC configuration
  functions - the definitions here must match the vector table positions, offset so that IRQ0 (the 1st ASIC IRQ) has
 the value 0...
*/
typedef enum IRQn
{
  /* Cortex-M0 core exceptions...*/
  NMI_IRQn          = -14,       
  Reset_IRQn        = -15,
  HardFault_IRQn    = -13,
  SVC_IRQn          = -5, 
  PendSV_IRQn       = -2, 
  SysTick_IRQn      = -1, 
  /* ASIC-specific IRQs... (should match the vector defined in dig_realplum_top.sv)*/
  WULIN_IRQn        = 0, 
  WUGPIO_IRQn       = 1, 
  WUTIMER_IRQn      = 2, 
  BOR_IRQn          = 3, 
  WatchdogA_IRQn    = 4, 
  UV_IRQn           = 5, 
  OV_IRQn           = 6, 
  LIN_IRQn          = 7, 
  ADC_IRQn          = 8, 
  PWM_IRQn          = 9, 
  LINM_IRQn         = 10, 
  GPIO_IRQn         = 11, 
  WULINM_IRQn       = 12, 
  OVTEMP_IRQn       = 13, 
  reserved14_IRQn   = 14, 
  Lullaby_IRQn      = 15,
  /* Clough peripheral IRQs...*/
  Timer0_IRQn       = 16, 
  Timer1_IRQn       = 17, 
  Timer2_IRQn       = 18, 
  Watchdog_IRQn     = 19, 
  BTE_IRQn          = 20, 
  SDIO_IRQn         = 21
} IRQn_Type;
/*
and define a tell-tale macro that will prevent the clough.h header from attempting to re-define this with the
 default (non-ASIC-specific) version...

#define __IRQn_Type
*/

#include <realplum_sfr.h>
#include "verne.h"

#endif

