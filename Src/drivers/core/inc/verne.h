#ifndef FlashChipVerneBased_H
#define FlashChipVerneBased_H

/********************************************************************************************************************/
/* Number of IRQ priority level bits - must be defined before loading core_cm0.h*/
/********************************************************************************************************************/

/* #define __NVIC_PRIO_BITS 2 */

/********************************************************************************************************************/
/*Includes*/
/********************************************************************************************************************/
#include "core_cm0.h"
#include <stdint.h>

/*
Target address for write/erase operation. 
In byte writes, this is the read address of the flash to be written to. 
In erase modes, it is a read address inside the sector to be erased. 
This register must be written in the correct sequence or the operation will fail.
*/
typedef struct{
      uint32_t ADDR : 16;
      uint32_t RSVD : 16;
}FLS_ADDR_t;


/*
Content to be written into the targeted address. 
This register must be written in the correct sequence or the operation will fail.
*/
typedef struct{
  uint32_t DATA;
}FLS_WRDT_t;


/*
Control register to unlock write. A value of 0x55555555 must be written to this 
address at the correct point in the write sequence or the operation will fail.
*/
typedef struct{
  uint32_t KEY;
}FLS_UNLBWR_t;


/*
Control register to start a write. A value of 0xAAAAAAAA must be written to this
address at the correct point in the write sequence or the operation will fail.
*/
typedef struct{
  uint32_t KEY;
}FLS_BWRSTRT_t;


/*
Control register to unlock a sector erase. A value of 0x66666666 must be written
to this address at the correct point in the sector erase sequence or the operation will fail.
*/
typedef struct{
  uint32_t KEY;
}FLS_UNLSER_t;


/*
Control register to commit a sector erase. A value of 0x99999999 must be written
to this address at the correct point in the sector erase sequence or the operation will fail.
*/
typedef struct{
  uint32_t KEY;
}FLS_SERSTRT_t;


/*
Number of wait states used in the reading process. Each read from flash memory 
will take number of cycles equal to 1+RWC to complete.
*/
typedef struct{
      uint32_t CTRL : 2;
      uint32_t RSVD : 30;
}FLS_CTRL_t;



/*
Code Protection / SerialWire Lockout Control 
Code protection control register. 
Write a value of 0xF2E11047 to disable the SerialWire interface. 
Write 0x00000000 to enable it. 
This allows the user program to disable the SerialWire interface to prevent 
unauthorized debug access to the part. 
NOTE1: This register does not lock the Flash Memory against read/write/erase by 
      the applications program. Instead what it does is to disable all communications 
      with the debug interface, therefore preventing any external attack. 
      The application code is still able to modify the flash content. 
NOTE2: Upon Power-On Reset or Normal Reset the system disables the communication 
      for a small time interval (8192 clock cycles). If the application needs to be 
      protected it is mandatory to set this register with the appropriate code in the 
      beginning of the initialization process and before the internal hardware enable 
      the debug communication.
*/
typedef struct{
  uint32_t KEY;
}FLS_CP_t;



/*
Flash Control Operation Register Unlock value. 0xACDC_1972 needs to be written 
in this register to unlock the Control Operation Register access. When this register
is read, it returns the state of the lock: 
      0: The Control Operation Register is locked. The Control Operation Register (FLASH_CTRL_OP) cannot be written. 
      1: The Control Operation Register is unlocked. The Control Operation Register (FLASH_CTRL_OP) can be written. 
Note: After each write to the FLASH_CTRL_OP register, the state of the lock is cleared and the pattern needs to be written again to allow a new configuration of the register.
*/
typedef struct{
  uint32_t KEY;
  uint32_t UNLOCK_CTRL_OP;
}FLS_UNLOCK_CTRL_OP_t;



/*
SIZE :SIZE of the write operation. Refer to data sheet for more information of the use of this field.
CHIP: CHIP bit. This bit is only used during the Erase operation. It allows the system to erase more than one sector. 
      0: The Erase operation will only erase the sector selected by the FLASH_ADDR register value. 
      1: The Erase operation will erase the full main array of the flash.
*/
typedef struct{
      uint32_t CHIP : 1;
      uint32_t SIZE : 2;
      uint32_t RSVD : 29;
}FLS_CTRL_OP_t;


/*

OSC_TRIM:
    Oscillator Trim Value. This register will be automatically populated with the
    value stored in the NVR sector 1 (@0001_0000).
SDIO_TIMING_CFG:
    SDIO interface timing configuration. This register will be automatically 
    populated with the value stored in the NVR sector 1 (@0001_0000). When set, 
    the SDIO/INT signals are captured on the falling edge of CLK. When cleared, 
    these data are captured on the rising edge of CLK
SLEEPDEEP_CFG:
    Deep Sleep VDD_IO configuration. This register will be automatically populated 
    with the value stored in the NVR sector 1 (@0001_0000). When set, the system will
    NOT be reset if VDD_IO is going away during Deep Sleep mode. Otherwise (0), the 
    system is reset if VDD_IO is removed.
*/
typedef struct{
      uint32_t OSC_TRIM        :16;
      uint32_t SDIO_TIMING_CFG :1;
      uint32_t SLEEPDEEP_CFG   :1;
      uint32_t RSVD            :14;
}FLS_TRIM_t;


#define FLS_WR_UNLOCK_KEY           0x55555555U
#define FLS_WR_START_KEY            0xAAAAAAAAU
#define FLS_SECTOR_ERASE_UNLOCK_KEY 0x66666666U
#define FLS_SECTOR_ERASE_START_KEY  0x99999999U
#define FLS_CTRL_OP_UNLOCK_KEY      0xACDC1972U


#define FLS_WR_ADDR                 ((__IO FLS_ADDR_t    *)0x50020020)
#define FLS_WR_DATA                 ((__IO FLS_WRDT_t    *)0x50020024)
#define FLS_WR_UNLOCK               ((__IO FLS_UNLBWR_t  *)0x50020028)
#define FLS_WR_START                ((__IO FLS_BWRSTRT_t *)0x5002002C)
#define FLS_SECTOR_ERASE_UNLOCK     ((__IO FLS_UNLSER_t  *)0x50020030)
#define FLS_SECTOR_ERASE_START      ((__IO FLS_SERSTRT_t *)0x50020034)
#define FLS_CTRL                    ((__IO FLS_CTRL_t    *)0x50020040)
#define FLS_CODE_PROT               ((__IO FLS_CP_t      *)0x50020044)
#define FLS_CTRL_OP_UNLOCK   ((__IO FLS_UNLOCK_CTRL_OP_t *)0x50020050)
#define FLS_CTRL_OP                 ((__IO FLS_CTRL_OP_t *)0x50020054)


#define FLS_WR_ADDR_REG                 (*(__IO uint32_t *)(0x50020020))
#define FLS_WR_DATA_REG                 (*(__IO uint32_t *)(0x50020024))
#define FLS_WR_UNLOCK_REG               (*(__IO uint32_t *)(0x50020028))
#define FLS_WR_START_REG                (*(__IO uint32_t *)(0x5002002C))
#define FLS_SECTOR_ERASE_UNLOCK_REG     (*(__IO uint32_t *)(0x50020030))
#define FLS_SECTOR_ERASE_START_REG      (*(__IO uint32_t *)(0x50020034))
#define FLS_CTRL_REG                    (*(__IO uint32_t *)(0x50020040))
#define FLS_CODE_PROT_REG               (*(__IO uint32_t *)(0x50020044))
#define FLS_CTRL_OP_UNLOCK_REG          (*(__IO uint32_t *)(0x50020050))
#define FLS_CTRL_OP_REG                 (*(__IO uint32_t *)(0x50020054))

/*
  end of define :obsoleted in future
*/
/*Definition of the several memory segments defined in Heimdall Slave
Warning: DO NOT CHANGE definitions without careful consideration and 
if at all possible contact Aydeekay LLC!*/

/*Flash Memory, addresses, sizes and blocks information*/


#define FLASH_INITIAL_ADDRESS         0x0L
#define FLASH_FINAL_ADDRESS           0xFFFFU
#define FLASH_SIZE                    ( ( LASH_FINAL_ADDRESS - FLASH_INITIAL_ADDRESS ) + 1 )
#define FLASH_SECTOR_SIZE              0x200U
#define NUMBER_FLASH_SECTORS           (FLASH_SIZE/FLASH_SECTOR_SIZE)

/*First Non-Volatile Register Sector (Used by the Factory - Access Forbidden!!)*/
#define NVR_SECTOR_1_INITIAL_ADDRESS  0x00010000U
#define NVR_SECTOR_1_FINAL_ADDRESS    0x000101FFU
#define NVR_SECTOR_1_SIZE             ( ( NVR_SECTOR_1_FINAL_ADDRESS - NVR_SECTOR_1_INITIAL_ADDRESS ) + 1 )

/*Second Non-Volatile Register Sector */
#define NVR_SECTOR_2_INITIAL_ADDRESS  0x00010200U
#define NVR_SECTOR_2_FINAL_ADDRESS    0x000103FFU
#define NVR_SECTOR_2_SIZE             ( ( NVR_SECTOR_2_FINAL_ADDRESS - NVR_SECTOR_2_INITIAL_ADDRESS ) + 1 )

/*SRAM Memory*/
#define SRAM_INITIAL_ADDRESS          0x20000000U
#define SRAM_FINAL_ADDRESS            0x20003FFFU
#define SRAM_SIZE                     ( ( SRAM_FINAL_ADDRESS - SRAM_INITIAL_ADDRESS ) + 1 )

/************** ARM INTERNAL Registers *****************/
/*WARNING: DO NOT TOUCH!*/
/*******************************************************/

/*Private Peripheral Bus*/
#define PPB_INITIAL_ADDRESS            0xE0000000U
#define PPB_FINAL_ADDRESS              0xE00FFFFFU
#define PPB_SIZE                       ( ( PPB_FINAL_ADDRESS - PPB_INITIAL_ADDRESS ) + 1 ) 

/*Reserved*/
#define RSV_INITIAL_ADDRESS            0xE0100000U
#define RSV_FINAL_ADDRESS              0xEFFFFFFFU
#define RSV_SIZE                       ( ( RSV_FINAL_ADDRESS - RSV_INITIAL_ADDRESS ) + 1 ) 

/*System ROM Tables*/
#define SRT_INITIAL_ADDRESS            0xF0000000U
#define SRT_FINAL_ADDRESS              0xF0001FFEU
#define SRT_SIZE                       ( ( SRT_FINAL_ADDRESS - SRT_INITIAL_ADDRESS ) + 1 ) 

/*From 0xF0001FFE till 0xFFFFFFFF is "default" == not implemented. The same 
applies to non-defined addresses.*/

/*                                              
- 8K   -bootlaoder
- 25K  -app
- 25K  -image 
- 5.5K -user param 
- 0.5K -color lib param 
*/

#define USER_PARAM_ADDRESS_START  (FLASH_FINAL_ADDRESS + 1U - 6U*1024U) // 
#define SYS_PHY_PARAMS_ADDRESS_START   (FLASH_FINAL_ADDRESS + 1U - FLASH_SECTOR_SIZE) /*64K-0.5K*/

#endif
