/**
 * @copyright 2016 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file clock_sfr_access.h
 */

#ifndef __CLOCK_SFR_ACCESS_H__
#define __CLOCK_SFR_ACCESS_H__

#include <stdint.h>
#include "vulcan.h"

/**
 * @brief The starting address of PLL trim register.
 */
#define CLOCK_PLL_TRIM_SFRS           ((__IO CLOCK_PLL_TRIM_SFRS_t *) (0x500180D6))

#define	E_CLOCK_SOURCE_RC_32K		0x00
#define	E_CLOCK_SOURCE_XT_32K		0x01
#define	E_CLOCK_SOURCE_RC_11M		0x02

#define	E_CLOCK_DIV_1                   0
#define	E_CLOCK_DIV_2                   1
#define	E_CLOCK_DIV_3                   2
#define	E_CLOCK_DIV_4                   3
#define	E_CLOCK_DIV_6                   4
#define	E_CLOCK_DIV_8                   5
#define	E_CLOCK_DIV_12                  6
#define	E_CLOCK_DIV_24                  7

/**
 * @brief Select clock source.
 *
 * param src The enumeration of clock source.
 */
static void f_CLOCK_SelectSource(uint8_t src)
{
    switch (src) {
    case E_CLOCK_SOURCE_RC_32K:
        break;
    case E_CLOCK_SOURCE_XT_LF:
        break;
    case E_CLOCK_SOURCE_RC_HF:
        break;
    default:
        break;
    }
}

/**
 * @brief Get clock source.
 *
 * return The enumeration of clock source.
 */
static uint8_t f_CLOCK_GetClockSource(void)
{
    return CLOCK_SFRS->PMUCLK.CKSEL;
}

/**
 * @brief Enable crystal clock source.
 */
static void f_CLOCK_EnableXTClock(void)
{
    CLOCK_SFRS->PMUCLK.XOEN = 1;
    CLOCK_SFRS->PMUCLK.PLLEN = 1;
}

/**
 * @brief Disable crystal clock source.
 */
static void f_CLOCK_DisableXTClock(void)
{
    CLOCK_SFRS->PMUCLK.PLLEN = 0;
    CLOCK_SFRS->PMUCLK.XOEN = 0;
}

/**
 * @brief Disable PLL.
 */
static void f_CLOCK_DisablePLL(void)
{
    CLOCK_SFRS->PMUCLK.PLLEN = 0;
}

/**
 * @brief Check if crystal clock source enabled.
 *
 * return The status of crystal clock source.
 */
static uint8_t f_CLOCK_IsXTClockEnabled(void)
{
    return CLOCK_SFRS->PMUCLK.XOEN;
}

/**
 * @brief Enable RC clock source.
 */
static void f_CLOCK_EnableRCClock(void)
{
    CLOCK_SFRS->PMUCLK.RCEN = 1;
}

/**
 * @brief Disable RC clock source.
 */
static void f_CLOCK_DisableRCClock(void)
{
    CLOCK_SFRS->PMUCLK.RCEN = 0;
}

/**
 * @brief Check if RC clock source enabled.
 *
 * return The status of RC clock source.
 */
static uint8_t f_CLOCK_IsRCClockEnabled(void)
{
    return CLOCK_SFRS->PMUCLK.RCEN;
}

/**
 * @brief Trim RC clock.
 *
 * @param val The trim setting for RC clock.
 */
static void f_CLOCK_RCTrim(uint8_t val)
{
    SYSCTRLA_SFRS->HF_OSC_TRIM.CCAL = val;
}

/**
 * @brief Set clock divider.
 *
 * @param val The setting for clock frequency divider.
 */
static void f_CLOCK_Divider(uint8_t val)
{
    CLOCK_SFRS->PMUCLK.CKD = val;
}

/**
 * @brief Trim RC 32KHz clock.
 *
 * @param val The trim setting for RC10KHz clock.
 */
static void f_CLOCK_RC10KTrim(uint8_t val)
{
    SYSCTRLA_SFRS->LF_OSC_TRIM.RC = val;
}

/**
* @brief Trim 32K XT clock.
 *
 * @param val The trim setting for XT 32K.
 */
static void f_CLOCK_XT32KTrim(uint8_t val)
{
    SYSCTRLA_SFRS->LF_OSC_TRIM.XO = val;
}

#endif /* __CLOCK_SFRS_H__ */

