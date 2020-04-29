/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file lin_sfrs.h
 */

#define LIN_SFRS LINS_SFRS

#ifndef __LIN_SFRS_H__
#define __LIN_SFRS_H__

#define E_LIN_STATUS_COMPLETE       (1 << 0)
#define E_LIN_STATUS_WAKEUP         (1 << 1)
#define E_LIN_STATUS_ERROR          (1 << 2)
#define E_LIN_STATUS_INT_REQ        (1 << 3)
#define E_LIN_STATUS_DATA_REQ       (1 << 4)
#define E_LIN_STATUS_ABORT          (1 << 5)
#define E_LIN_STATUS_IDLE_TIMEOUT   (1 << 6)
#define E_LIN_STATUS_ACTIVE         (1 << 7)

#define E_LIN_ERROR_BIT             (1 << 0)
#define E_LIN_ERROR_CHECKSUM        (1 << 1)
#define E_LIN_ERROR_TIMEOUT         (1 << 2)
#define E_LIN_ERROR_PARITY          (1 << 3)

#define E_LIN_TIME_INACTIVE_SEC_4       0
#define E_LIN_TIME_INACTIVE_SEC_6       1
#define E_LIN_TIME_INACTIVE_SEC_8       2
#define E_LIN_TIME_INACTIVE_SEC_10      3
#define E_LIN_TIME_INACTIVE_MAX         E_LIN_TIME_INACTIVE_SEC_10

#define E_LIN_TIME_WAKEUP_REPEAT_MS_180 0
#define E_LIN_TIME_WAKEUP_REPEAT_MS_200 1
#define E_LIN_TIME_WAKEUP_REPEAT_MS_220 2
#define E_LIN_TIME_WAKEUP_REPEAT_MS_240 3
#define E_LIN_TIME_WAKEUP_REPEAT_MAX    E_LIN_TIME_WAKEUP_REPEAT_MS_240

#define E_LIN_BAUDRATE_1000     0
#define E_LIN_BAUDRATE_9600     1
#define E_LIN_BAUDRATE_19200    2
#define E_LIN_BAUDRATE_MAX      E_LIN_BAUDRATE_19200

#define E_LIN_MODE_SLAVE        0
#define E_LIN_MODE_MASTER       1
#define E_LIN_MODE_MAX          E_LIN_MODE_MASTER

#define E_LIN_CHKSUM_CLASSIC    0
#define E_LIN_CHKSUM_ENHANCED   1

#define E_LIN_POLARITY_HIGH     0
#define E_LIN_POLARITY_LOW      1

#define E_LIN_DATA_BUFFER_LENGTH    8

/**
 * @brief Write LIN data buffer.
 *
 * @param index The index to the data buffer, supported values are 0...7.
 * @param data The data to be written to the buffer.
 * @return None.
 */
static __INLINE void f_LIN_WriteDataBuffer(uint8_t index, uint8_t data)
{
    LIN_SFRS->DATA.ARRAY[index].DATABUF = data;
}

/**
 * @brief Start a transmission.
 *
 * This function starts a transmission. The caller needs to make sure the data
 * buffer, data length and identifier are already loaded.
 * @return None.
 */
static __INLINE void f_LIN_StartTX(void)
{
    /* Not supported, slave only mode */
}

/**
 * @brief Send wake up signal.
 *
 * This function wake up LIN core from sleep mode and send a wake up signal.
a transmission. The caller needs to make sure the data
 * buffer, data length and identifier are already loaded.
 * @return None.
 */
static __INLINE void f_LIN_Wakeup(void)
{
    LIN_SFRS->CTRL.WAKEUPREQ = 1;
}

static __INLINE void f_LIN_ResetError(void)
{
    LIN_SFRS->CTRL.RSTERR = 1;
}

static __INLINE void f_LIN_ResetInterrupt(void)
{
    LIN_SFRS->CTRL.RSTINT = 1;
}

static __INLINE void f_LIN_SendAck(void)
{
    LIN_SFRS->CTRL.DATAACK = 1;
}

static __INLINE void f_LIN_SetTXFrame(void)
{
    LIN_SFRS->CTRL.TRANSMIT = 1;
}

static __INLINE void f_LIN_SetRXFrame(void)
{
    LIN_SFRS->CTRL.TRANSMIT = 0;
}

static __INLINE void f_LIN_Sleep(void)
{
    LIN_SFRS->CTRL.SLEEP = 1;
}

static __INLINE void f_LIN_Stop(void)
{
    LIN_SFRS->CTRL.STOP = 1;
}

/**
 * @brief Read LIN data buffer.
 *
 * @param index The index to the data buffer, supported values are 0...7.
 * @return the data from the buffer.
 */
static __INLINE uint8_t f_LIN_ReadDataBuffer(uint8_t index)
{
    return LIN_SFRS->DATA.ARRAY[index].DATABUF;
}

/**
 * @brief Get LIN status.
 *
 * @return The status byte.
 */
static __INLINE uint8_t f_LIN_GetStatus(void)
{
    return (uint8_t)LIN_SFRS->STATUS.WORD;
}

/**
 * @brief Get LIN error status.
 *
 * @return The error status byte.
 */
static __INLINE uint8_t f_LIN_GetErrorStatus(void)
{
    return (uint8_t)LIN_SFRS->ERROR.WORD;
}

/**
 * @brief Set LIN timing.
 *
 * @param div The setting for bit divider.
 * @param mul The setting for bit multiplier.
 * @param frac The setting for fraction divider.
 * @return None.
 */
static __INLINE void f_LIN_SetTiming(uint16_t div, uint16_t mul, uint8_t frac)
{
    LIN_SFRS->BTDIV07 = div & 0xFF;
    LIN_SFRS->BITTIME.BTDIV8 = div >> 8;
    LIN_SFRS->BITTIME.PRESCL = frac;
    /* mul supported, slave only mode */
}

/**
 * @brief Set LIN ID.
 *
 * @param id The id number of LIN message.
 * @return None.
 */
static __INLINE void f_LIN_SetID(uint8_t id)
{
    LIN_SFRS->ID = id;
}

/**
 * @brief Get LIN ID.
 *
 * @return The ID from register.
 */
static __INLINE uint8_t f_LIN_GetID(void)
{
    return LIN_SFRS->ID;
}

/**
 * @brief Select LIN mode as master or slave.
 *
 * @param mode The mode of LIN module, supported values:
 * E_LIN_MODE_MASTER or E_LIN_MODE_SLAVE,
 * @return None.
 */
static __INLINE void f_LIN_SetMode(uint8_t mode)
{
    /* Not supported, slave only mode */
}

/**
 * @brief Set LIN inactive time.
 *
 * @param time The enumeration of inactive time, defined as:
 *  E_LIN_TIME_INACTIVE_SEC_10 etc.
 * @return None.
 */
static __INLINE void f_LIN_SetInactiveTime(uint8_t time)
{
    LIN_SFRS->BUSTIME.BUSINACTIVE = time;
}

/**
 * @brief Set LIN wakeup repeat time.
 *
 * @param time The enumeration of wakeup repeat time, defined as:
 *  E_LIN_TIME_WAKEUP_REPEAT_MS_180, etc,
 * @return None.
 */
static __INLINE void f_LIN_SetWakeupRepeatTime(uint8_t time)
{
    LIN_SFRS->BUSTIME.WUPREPEAT = time;
}

/**
 * @brief Set LIN data length.
 *
 * @param len The length of LIN data in number of bytes, supported values:
 * 0...8.
 * @return None.
 */
static __INLINE void f_LIN_SetDataLength(uint8_t len)
{
    LIN_SFRS->DL.LENGTH = len;
}

/**
 * @brief Set LIN checksum type.
 *
 * @param type The type of checksum, supported values:
 * E_LIN_CHKSUM_CLASSIC
 * E_LIN_CHKSUM_ENHANCED
 * @return None.
 */
static __INLINE void f_LIN_SetChecksumType(uint8_t type)
{
    LIN_SFRS->DL.ENHCHK = type;
}

/**
 * @brief Set LIN polarity.
 *
 * @param type The type of polarity, supported values:
 * E_LIN_POLARITY_HIGH
 * E_LIN_POLARITY_LOW
 * @return None.
 */
static __INLINE void f_LIN_SetPolarityType(uint8_t type)
{
    /* Not supported, slave only mode */
}

#endif /* __LIN_SFRS_H__ */
