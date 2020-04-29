/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file gpio_device.h
 */

#ifndef GPIO_DEVICE_H__
#define GPIO_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "realplum.h"
#include "errno.h"


typedef enum{
  GPIO_PORT_1 = 0U,
  GPIO_PORT_2,
  GPIO_PORT_3,
  GPIO_PORT_4,
}GpioPort_t;


typedef enum{
  GPIO_DIR_INPUT  = 0U,
  GPIO_DIR_OUTPUT,
}GpioDir_t;

typedef enum{
  GPIO_PULL_MODE_PULLUP = 0U,
  GPIO_PULL_MODE_PULLDOWN,
  GPIO_PULL_MODE_NONE,
}GpioPullMode_t;

typedef enum{
  GPIO_LOW = 0U,
  GPIO_HIGH,
}GpioSetState_t;

typedef enum{
  GPIO_MUX_GPIO = 0U,
  GPIO_MUX_PWM,
  GPIO_MUX_LIN,
}GpioMuxMode_t;


typedef enum{
  GPIO_PWM_CH1 = 0U,
  GPIO_PWM_CH2,
  GPIO_PWM_CH3,
  GPIO_PWM_NONE,
}GpioPWMchannel_t;


typedef enum{
  GPIO_FALLING_EDGE = 0U,
  GPIO_RISING_EDGE,
}GpioISREdge_t;


typedef void (*gpio_cb_func_t)(void);


void GPIO_Init(GpioPort_t port, GpioMuxMode_t muxMode, GpioDir_t stateMode,GpioPullMode_t pullMode,GpioPWMchannel_t channel);
void GPIO_RegisterIRQ(GpioPort_t port, GpioISREdge_t risingEdge, gpio_cb_func_t callback);
void GPIO_UnRegisterIRQ(GpioPort_t port);
void GPIO_Set(GpioPort_t port, GpioSetState_t state);
uint8_t GPIO_Read(GpioPort_t port);

#endif /* __GPIO_DEVICE_H__ */