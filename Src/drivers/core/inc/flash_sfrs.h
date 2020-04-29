/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file flash_sfrs.h
 */

#ifndef FLASH_SFRS_H__
#define FLASH_SFRS_H__

#include <stddef.h>
#include <stdint.h>
#include "realplum.h"

typedef enum{
    E_FLASH_CODE_UNPROTECT = 0x00000000U,
    E_FLASH_CODE_PROTECT = 0xF2E11047U,
}CodeProtectionMode_t;
/**
* @brief Flash memory erase sector function.
*
* @param address inside the sector to be erased.
*/
static __INLINE void f_FLASH_EraseSector(__IO uint32_t address)
{
    /* load address */
    FLS_WR_ADDR->ADDR = address;
    /* do erasing... */
    FLS_SECTOR_ERASE_UNLOCK->KEY = FLS_SECTOR_ERASE_UNLOCK_KEY;
    FLS_SECTOR_ERASE_START->KEY  = FLS_SECTOR_ERASE_START_KEY;
/*
     Wait for the erase to be done by writing into the Flash Controller
     The write access will be held until the flash erase operation is done
     Any write or read to the flash controller will do the trick...
*/
    FLS_CTRL_REG             = 0x00000000U;
}

/**
* @brief Flash memory write word function.
*
* @param address and data (byte) to be programmed into flash.
* @param data and data (byte) to be programmed into flash.
*/
static __INLINE  void f_FLASH_WriteWord(__IO uint32_t address, uint32_t data)
{
    /* load address */
    FLS_CTRL_OP_UNLOCK->KEY = FLS_CTRL_OP_UNLOCK_KEY;
    FLS_CTRL_OP->SIZE = 3U;
    FLS_WR_ADDR->ADDR = address;
    FLS_WR_DATA->DATA = data;
    FLS_WR_UNLOCK->KEY = FLS_WR_UNLOCK_KEY;
    FLS_WR_START->KEY  = FLS_WR_START_KEY;
 /*
     Wait for the write to be done by writing into the Flash Controller
     The write access will be held until the flash operation is done
     Any write or read to the flash controller will do the trick...
*/
    FLS_CTRL_REG             = 0x00000000U;
}

/**
* @brief Flash memory write byte check function.
*
* @param mode The enumeration of enable or disable code protection
*/
static __INLINE  void f_FLASH_CodeProtection(CodeProtectionMode_t mode)
{
    FLS_CODE_PROT->KEY = (uint32_t)mode; 
}

#endif /* __FLASH_SFRS_H__ */