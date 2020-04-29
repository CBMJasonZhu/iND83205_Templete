/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file flash_device.c
 */

#include <stdint.h>
#include <string.h>
#include "flash_device.h"
#include "flash_sfrs.h"


static uint8_t flashBuff[FLASH_SECTOR_SIZE];

/* address + size can't overlap section address */
int8_t Flash_devCopyToFlash(uint8_t *const buff, uint32_t address, uint16_t size)
{
    int8_t result;
    uint32_t temp;
    uint32_t offsetAddr;
    uint32_t sectorAddr;
    
    //保证在用户参数区
    if ( (address < USER_PARAM_ADDRESS_START) || (address > SYS_PHY_PARAMS_ADDRESS_START) || ((address + size) >= FLASH_FINAL_ADDRESS) || (size == 0U) || (((address%FLASH_SECTOR_SIZE) + size) > FLASH_SECTOR_SIZE) ){
        result = -1;
    }else{
        result = 0;
        sectorAddr = (address/FLASH_SECTOR_SIZE)*FLASH_SECTOR_SIZE;//计算扇区地址
        offsetAddr = address%FLASH_SECTOR_SIZE;
        
        for (uint32_t i = 0U; i < FLASH_SECTOR_SIZE; i++){
            flashBuff[i] = ((uint8_t *)sectorAddr)[i];
        }
        
        for (uint16_t i = 0U; i < size; i++){
            flashBuff[offsetAddr+i] = buff[i];
        }
        /* Erase sector data */
        f_FLASH_EraseSector(sectorAddr);
        /* Write data to flash */  
        //32bit长度数据
        for (uint32_t i = 0U; i < (FLASH_SECTOR_SIZE >> 2U); i++){
            temp = (uint32_t)flashBuff[i*4U + 0U] + ((uint32_t)flashBuff[i*4U + 1U] << 8U)  + ((uint32_t)flashBuff[i*4U + 2U] << 16U)  + ((uint32_t)flashBuff[i*4U + 3U] << 24U) ;
            f_FLASH_WriteWord(sectorAddr + (i << 2U), temp);
        }
    }
    return result;
}


