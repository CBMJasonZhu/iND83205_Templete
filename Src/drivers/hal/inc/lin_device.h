/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file lin_device.h
 */

#ifndef LIN_DEVICE_H__
#define LIN_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include "realplum.h"
#include "lins_sfr.h"
#include "linm_sfr.h"

#define LIN_BUFF_SIZE                  8U

/* Defines for LIN device to handle message direction.  */
#define LIN_MSG_TYPE_RX                0U /*received data from sender for slave; */
#define LIN_MSG_TYPE_TX                1U /*data request from sender for slave; */

#define LIN_CHECKSUM_CLASSIC           0U /*!< classic checksum does not include ID Byte.  */ 
#define LIN_CHECKSUM_ENHANCED          1U /*!< "enhanced" checksum includes ID Byte.  */ 

typedef enum{
  DS_ON = 0U,
  DW_OFF
}DownSwitchState_t;

/**
* @brief A structure to represent LIN frame information.
*/
typedef struct { 
    uint8_t  frame_id;                  /*!< frame ID.  */ 
    uint8_t  msg_type;                  /*!< Type of message, tx or rx.  */ 
    uint8_t  checksum;                  /*!< Type of checksum (enhanced or classic).  */ 
    uint8_t  length;                    /*!< Number of bytes in the data. */ 
    uint8_t  data[LIN_BUFF_SIZE];       /*!< data buffer. */
    uint8_t  responseValid;
    uint8_t  valid;                     /*!< frame is valid or not */
} LIN_Device_Frame_t;


typedef void (*lin_cb_func_t)(LIN_Device_Frame_t const *frame);
typedef void (*lin_active_cb_func_t)(void);

void LINS_Init(void);
void LINS_SwitchDownstreamTo(DownSwitchState_t state);
void LINS_RegisterServices(LIN_Device_Frame_t *lin_cmd_table, uint8_t size);
void LINS_RegisterLINS_IRQ(lin_cb_func_t cb); 
void LINS_RegisterLINS_Active_IRQ(lin_active_cb_func_t cb);
void LINS_UnRegisterLINS_IRQ(void);

void LINM_Init(void);
void LINM_RegisterLINM_IRQ(lin_cb_func_t cb);
void LINM_UnRegisterLINM_IRQ(void);

int8_t LINM_SendFrame(LIN_Device_Frame_t *frame);


#endif /* __LIN_DEVICE_H__ */
