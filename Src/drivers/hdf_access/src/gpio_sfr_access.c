/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file gpio_sfrs.c
 */

#include <stdbool.h>
#include "iceblue.h"
#include "gpio_sfr_access.h"

void GPIO_EnablePullUp(uint8_t pos, bool bit)
{
    switch(pos) {
    case 0:
        if (bit)
            IOCTRLA_SFRS->GPIO1.PUENA = 0;
        else
            IOCTRLA_SFRS->GPIO1.PUENA = 1;
        break;
    case 1:
        if (bit)
            IOCTRLA_SFRS->GPIO2.PUENA = 0;
        else
            IOCTRLA_SFRS->GPIO2.PUENA = 1;
        break;
    default:
        break;
    }
}

void GPIO_EnablePullDown(uint8_t pos, bool bit)
{
    switch(pos) {
    case 0:
        if (bit)
            IOCTRLA_SFRS->GPIO1.PDENA = 1;
        else
            IOCTRLA_SFRS->GPIO1.PDENA = 0;
        break;
    case 1:
        if (bit)
            IOCTRLA_SFRS->GPIO2.PDENA = 1;
        else
            IOCTRLA_SFRS->GPIO2.PDENA = 0;
        break;
    default:
        break;
    }
}

void GPIO_EnableOutput(uint8_t pos, bool bit)
{
    switch(pos) {
    case 0:
        if (bit) {
            GPIO_SFRS->GPAP03.GPADIR0 = 1;
        } else {
            GPIO_SFRS->GPAP03.GPADIR0 = 0;
        }
        break;
    case 1:
        if (bit) {
            GPIO_SFRS->GPAP03.GPADIR1 = 1;
        } else {
            GPIO_SFRS->GPAP03.GPADIR1 = 0;
        }
        break;
    default:
        break;
    }
}

void GPIO_EnableInput(uint8_t pos, bool bit)
{
    switch(pos) {
    case 0:
        if (bit) {
            IOCTRLA_SFRS->GPIO1.RDENA = 1;
            GPIO_SFRS->GPAP03.GPADIR0 = 0;
        } else {
            IOCTRLA_SFRS->GPIO1.RDENA = 0;
            GPIO_SFRS->GPAP03.GPADIR0 = 1;
        }
        break;
    case 1:
        if (bit) {
            IOCTRLA_SFRS->GPIO2.RDENA = 1;
            GPIO_SFRS->GPAP03.GPADIR1 = 0;
        } else {
            IOCTRLA_SFRS->GPIO2.RDENA = 0;
            GPIO_SFRS->GPAP03.GPADIR1 = 1;
        }
        break;
    default:
        break;
    }
}

void GPIO_EnableInterrupt(uint8_t pos, bool r, bool f)
{
    switch(pos) {
    case 0:
        if (r | f) {
            GPIO_SFRS->GPAP03.GPAIE0 = 1;
            GPIO_SFRS->GPAP03.GPARE0 = r;
            GPIO_SFRS->GPAP03.GPAFE0 = f;
        } else {
            GPIO_SFRS->GPAP03.GPAIE0 = 0;
        }
        break;
    case 1:
        if (r | f) {
            GPIO_SFRS->GPAP03.GPAIE1 = 1;
            GPIO_SFRS->GPAP03.GPARE1 = r;
            GPIO_SFRS->GPAP03.GPAFE1 = f;
        } else {
            GPIO_SFRS->GPAP03.GPAIE1 = 0;
        }
        break;
    default:
        break;
    }
}

uint8_t GPIO_Read(uint8_t pos)
{
    return (uint8_t )(((GPIO_SFRS->DATA[(1 << pos)]) & (1 << pos)) >> pos);
}

void GPIO_Set(uint8_t pos, uint8_t bit)
{
    GPIO_SFRS->DATA[(1 << pos)] = bit << pos;
}

