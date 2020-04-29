/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file gpio_device.c
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "gpio_device.h"
#include "gpio_sfr.h"
#include <isrfuncs.h>

#ifdef GPIO_SFRS
static gpio_cb_func_t gpioCallback[4]= {NULL, NULL,NULL, NULL};


void GPIO_Handler( void )
{
    if(GPIO_SFRS->GPAP03.GPAACTDET0 == 1U){/* GPIO1 */
        GPIO_SFRS->GPAP03.GPACLR0 = 1U; 
        if (gpioCallback[0] !=NULL){
            gpioCallback[0]();
        }
    }
    if(GPIO_SFRS->GPAP03.GPAACTDET1 == 1U){/* GPIO2 */
        GPIO_SFRS->GPAP03.GPACLR1 = 1U; 
        if (gpioCallback[1] !=NULL){
            gpioCallback[1]();
        }
    }
    if(GPIO_SFRS->GPAP74.GPAACTDET4 == 1U){/* GPIO3 */
        GPIO_SFRS->GPAP74.GPACLR4 = 1U;
        if (gpioCallback[2] !=NULL){
            gpioCallback[2]();
        }
    }
    if(GPIO_SFRS->GPAP74.GPAACTDET5 == 1U){/* GPIO4 */
        GPIO_SFRS->GPAP74.GPACLR5 = 1U;
        if (gpioCallback[3] !=NULL){
            gpioCallback[3]();
        }
    }
}


void GPIO_Init(GpioPort_t port, GpioMuxMode_t muxMode, GpioDir_t stateMode,GpioPullMode_t pullMode,GpioPWMchannel_t channel)
{
    if (port == GPIO_PORT_1){
        IOCTRLA_SFRS->GPIO1.HWMODE = (uint32_t)muxMode;
    }else if (port == GPIO_PORT_2){
        IOCTRLA_SFRS->GPIO2.HWMODE = (uint32_t)muxMode;
    }else if (port == GPIO_PORT_3){
        IOCTRLA_SFRS->GPIO3.HWMODE = (uint32_t)muxMode;
    }else{
        IOCTRLA_SFRS->GPIO4.HWMODE = (uint32_t)muxMode;
    }
  
  if (muxMode == GPIO_MUX_GPIO){
      if (port == GPIO_PORT_1){
          GPIO_SFRS->GPAP03.GPADIR0 = (uint32_t)stateMode;
          IOCTRLA_SFRS->GPIO1.RDENA = 1U; /*read enable */
          if (pullMode == GPIO_PULL_MODE_PULLUP){
              IOCTRLA_SFRS->GPIO1.PUENA = 0U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO1.PDENA = 0U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }else if (pullMode == GPIO_PULL_MODE_PULLDOWN){
              IOCTRLA_SFRS->GPIO1.PUENA = 1U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO1.PDENA = 1U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }else{
              IOCTRLA_SFRS->GPIO1.PUENA = 1U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO1.PDENA = 0U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }
      }else if (port == GPIO_PORT_2){
          GPIO_SFRS->GPAP03.GPADIR1 = (uint32_t)stateMode;
          IOCTRLA_SFRS->GPIO2.RDENA = 1U; /*read enable */
          if (pullMode == GPIO_PULL_MODE_PULLUP){
              IOCTRLA_SFRS->GPIO2.PUENA = 0U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO2.PDENA = 0U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }else if (pullMode == GPIO_PULL_MODE_PULLDOWN){
              IOCTRLA_SFRS->GPIO2.PUENA = 1U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO2.PDENA = 1U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }else{
              IOCTRLA_SFRS->GPIO2.PUENA = 1U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO2.PDENA = 0U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }
      }else if (port == GPIO_PORT_3){
          GPIO_SFRS->GPAP74.GPADIR4 = (uint32_t)stateMode;
          IOCTRLA_SFRS->GPIO3.RDENA = 1U; /*read enable */
          if (pullMode == GPIO_PULL_MODE_PULLUP){
              IOCTRLA_SFRS->GPIO3.PUENA = 0U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO3.PDENA = 0U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }else if (pullMode == GPIO_PULL_MODE_PULLDOWN){
              IOCTRLA_SFRS->GPIO3.PUENA = 1U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO3.PDENA = 1U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }else{
              IOCTRLA_SFRS->GPIO3.PUENA = 1U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO3.PDENA = 0U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }
      }else{
          GPIO_SFRS->GPAP74.GPADIR5 = (uint32_t)stateMode;
          IOCTRLA_SFRS->GPIO4.RDENA = 1U; /*read enable */
          if (pullMode == GPIO_PULL_MODE_PULLUP){
              IOCTRLA_SFRS->GPIO4.PUENA = 0U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO4.PDENA = 0U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }else if (pullMode == GPIO_PULL_MODE_PULLDOWN){
              IOCTRLA_SFRS->GPIO4.PUENA = 1U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO4.PDENA = 1U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down*/
          }else{
              IOCTRLA_SFRS->GPIO4.PUENA = 1U; /* 0x0: Enable 100K Ohm Pull Up    0x1: Disable 100K Ohm Pull Up */
              IOCTRLA_SFRS->GPIO4.PDENA = 0U; /* 0x0: Disable 100K Ohm Pull Down 0x1: Enable 100K Ohm Pull Down */
          }
      }
  }else if (muxMode == GPIO_MUX_PWM){
      if (port == GPIO_PORT_1){
          IOCTRLA_SFRS->GPIO1.PWM_SEL = (uint32_t)channel;
      }else if (port == GPIO_PORT_2){
          IOCTRLA_SFRS->GPIO2.PWM_SEL = (uint32_t)channel;
      }else if (port == GPIO_PORT_3){
          IOCTRLA_SFRS->GPIO3.PWM_SEL = (uint32_t)channel;
      }else{
          IOCTRLA_SFRS->GPIO4.PWM_SEL = (uint32_t)channel;
      }
  }else{ /* GPIO_MUX_LIN */

  }
}


void GPIO_RegisterIRQ(GpioPort_t port, GpioISREdge_t risingEdge, gpio_cb_func_t callback)
{
    if (port == GPIO_PORT_1){
        GPIO_SFRS->GPAP03.GPARE0 = (uint32_t)risingEdge;
        GPIO_SFRS->GPAP03.GPAFE0 = (uint32_t)(!(bool)risingEdge);
        GPIO_SFRS->GPAP03.GPAIE0 = 1U;
    }else if (port == GPIO_PORT_2){
        GPIO_SFRS->GPAP03.GPARE1 = (uint32_t)risingEdge;
        GPIO_SFRS->GPAP03.GPAFE1 = (uint32_t)(!(bool)risingEdge);
        GPIO_SFRS->GPAP03.GPAIE1 = 1U;
    }else if (port == GPIO_PORT_3){
        GPIO_SFRS->GPAP74.GPARE4 = (uint32_t)risingEdge;
        GPIO_SFRS->GPAP74.GPAFE4 = (uint32_t)(!(bool)risingEdge);
        GPIO_SFRS->GPAP74.GPAIE4 = 1U;
    }else{
        GPIO_SFRS->GPAP74.GPARE5 = (uint32_t)risingEdge;
        GPIO_SFRS->GPAP74.GPAFE5 = (uint32_t)(!(bool)risingEdge);
        GPIO_SFRS->GPAP74.GPAIE5 = 1U;
    }
    gpioCallback[port] = callback;
    NVIC_EnableIRQ(GPIO_IRQn);
}

void GPIO_UnRegisterIRQ(GpioPort_t port)
{
    if (port == GPIO_PORT_1){
        GPIO_SFRS->GPAP03.GPAIE0 = 0U;
    }else if (port == GPIO_PORT_2){
        GPIO_SFRS->GPAP03.GPAIE1 = 0U;
    }else if (port == GPIO_PORT_3){
        GPIO_SFRS->GPAP74.GPAIE4 = 0U;
    }else{
        GPIO_SFRS->GPAP74.GPAIE5 = 0U;
    }
    gpioCallback[port] = NULL;
}

void GPIO_Set(GpioPort_t port, GpioSetState_t state)
{
    if (port == GPIO_PORT_1){
        GPIO_SFRS->GPADATA[0x01] = ((uint8_t)state << 0x00U);/* GPIO1 */
    }else if (port == GPIO_PORT_2){
        GPIO_SFRS->GPADATA[0x02] = ((uint8_t)state << 0x01U);/* GPIO2 */
    }else if (port == GPIO_PORT_3){
        GPIO_SFRS->GPADATA[0x10] = ((uint8_t)state << 0x04U);/* GPIO3 */
    }else{
        GPIO_SFRS->GPADATA[0x20] = ((uint8_t)state << 0x05U);/* GPIO4 */
    }
}

uint8_t GPIO_Read(GpioPort_t port)
{
    uint8_t data;
    if (port == GPIO_PORT_1){
        data = GPIO_SFRS->GPADATA[0x01];/* GPIO1 */
    }else if (port == GPIO_PORT_2){
        data = GPIO_SFRS->GPADATA[0x02];/* GPIO2 */
    }else if (port == GPIO_PORT_3){
        data = GPIO_SFRS->GPADATA[0x10];/* GPIO3 */
    }else{
        data = GPIO_SFRS->GPADATA[0x20];/* GPIO4 */
    }
    return (uint8_t)(data != 0U);
}

/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
/* GPIO wake up interrupt ISR */
void WUGPIO_Handler(void)
{
    WICA_SFRS->CTRL.GPIOIRQCLR  = 1U;
}
/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
/* wakeup timer wake up interrupt ISR */
void WUTIMER_Handler(void)
{
    WICA_SFRS->CTRL.WU_TIMERIRQCLR = 1U;
}

#endif