/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file gpio_sfrs_access.h
 */

#ifndef __GPIO_SFRS_ACCESS_H__
#define __GPIO_SFRS_ACCESS_H__

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Enable/disable pull up on GPIO
 *
 * @param pos The pin position.
 * @param bit The enable bit, true for enable, false for disable.
 */
void GPIO_EnablePullUp(uint8_t pos, bool bit);

/**
 * @brief Enable/disable pull down on GPIO
 *
 * @param pos The pin position.
 * @param bit The enable bit, true for enable, false for disable.
 */
void GPIO_EnablePullDown(uint8_t pos, bool bit);

/**
 * @brief Enable/disable output on GPIO
 *
 * @param pos The pin position.
 * @param bit The enable bit, true for enable, false for disable.
 */
void GPIO_EnableOutput(uint8_t pos, bool bit);

/**
 * @brief Enable/disable Input on GPIO
 *
 * @param pos The pin position.
 * @param bit The enable bit, true for enable, false for disable.
 */
void GPIO_EnableInput(uint8_t pos, bool bit);

/**
 * @brief Enable/disable interrupt on GPIO
 *
 * @param pos The pin position.
 * @param r The rising edge enable bit, true for enable, false for disable.
 * @param f The falling edge enable bit, true for enable, false for disable.
 */
void GPIO_EnableInterrupt(uint8_t pos, bool r, bool f);

/**
 * @brief Read pin state on GPIO
 *
 * @param pos The pin position.
 * @return The pin state.
 */
uint8_t GPIO_Read(uint8_t pos);

/**
 * @brief Set pin state on GPIO
 *
 * @param pos The pin position.
 * @param bit The pin state.
 */
void GPIO_Set(uint8_t pos, uint8_t bit);

#endif /* __GPIO_SFRS_ACCESS_H__ */
