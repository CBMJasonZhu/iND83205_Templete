/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file lin_stack.h
  */

#ifndef LIN_STACK_H__
#define LIN_STACK_H__

#include <stdint.h>
#include <linslaveTask.h>
#include <lin_device.h>

#define LIN_PCI_SF               (0x00U << 4U)
#define LIN_PCI_FF               (0x01U << 4U)
#define LIN_PCI_CF               (0x02U << 4U)

#define LIN_NAD_WILDCARD                0x7FU
#define LIN_SUPPLIER_ID_WILDCARD        0x7FFFU
#define LIN_FUNCTION_ID_WILDCARD        0x7FFFU



/**
 * @brief The enumerations of frame identifiers.
 */
#define LIN_FRAME_ID_MASTER_REQ         0x3CU
#define LIN_FRAME_ID_SLAVE_RES          0x3DU
#define LIN_FRAME_ID_FUNCTIONAL         0x7EU
#define LIN_FRAME_ID_UNASSIGN           0x00U
#define LIN_FRAME_ID_DONOTCARE          0xFFU

/**
 * @brief The enumerations of NAD.
 */
#define LIN_NAD_SNPD_ON                 0x20U
#define LIN_NAD_SNPD_CFG                0x21U
#define LIN_NAD_SNPD_OFF                0x22U

/**
 * @brief The enumerations of service IDs.
 */
#define LIN_SID_ASSIGN_NAD              0xB0U
#define LIN_SID_ASSIGN_FRAME_ID         0xB1U
#define LIN_SID_READ_BY_ID              0xB2U
#define LIN_SID_CONDITIONAL_CHANGE_NAD  0xB3U
#define LIN_SID_DATA_DUMP               0xB4U
#define LIN_SID_SNPD_ASSIGN_NAD         0xB0U
#define LIN_SID_SAVE_CONFIGURATION      0xB6U
#define LIN_SID_ASSIGN_FRAME_ID_RANGE   0xB7U

#define LIN_SID_READ_DATABY_ID          0x22U

#define LIN_CHECKSUM_CLASSIC            0U    /*!< classic checksum does not include ID Byte.  */ 
#define LIN_CHECKSUM_ENHANCED           1U    /*!< "enhanced" checksum includes ID Byte.  */ 

/**
 * @brief The offset of reponse service IDs.
 */
#define LIN_RSID_OFFSET                 0x40U

/**
 * @brief The error code used by the APIs.
 */
#define LD_SET_OK                       0U
#define LD_READ_OK                      0U
#define LD_LENGTH_NOT_CORRECT           1U
#define LD_DATA_ERROR                   2U
#define LD_LENGTH_TOO_SHORT             3U

/**
 * @brief The definitions and enumerations used in the API function in LIN stack.
 */
typedef uint8_t         l_bool; 
typedef uint8_t         l_ioctl_op; 
typedef uint8_t         l_irqmask; 
typedef uint8_t         l_u8; 
typedef uint16_t        l_u16; 
typedef uint32_t        l_u32; 

typedef uint8_t         l_signal_handle; 
typedef uint8_t         l_flag_handle; 
typedef uint8_t         l_ifc_handle; 
typedef uint8_t         l_schedule_handle;








































/**
* @brief A structure to represent LIN status information.
*/
typedef struct {
    l_u8 error_in_resp:1;
    l_u8 success_in_transfer:1;
    l_u8 overrun:1;
    l_u8 goto_sleep:1;
    l_u8 bus_activity:1;
    l_u8 event_trig_frame_collision:1;
    l_u8 save_config:1;
    l_u8 :1;
    l_u8 pid;
} lin_status_t;

/**
* @brief A structure to represent LIN product identification.
*/
typedef struct { 
    l_u8  supplier_id_lsb;
    l_u8  supplier_id_msb;
    l_u8  function_id_lsb;
    l_u8  function_id_msb;
    l_u8  variant_id;
} lin_product_id_t; 


/**
 * @brief A structure to represent LIN single request frame.
 */
typedef struct { 
    l_u8  nad;          /*!< Node address.  */ 
    struct {
        l_u8   len:4;   /*!< message length */
        l_u8   type:4;  /*!< type is 0x00 for single frame */
    } pci;              /*!< protocol control information.  */ 
    l_u8  sid;          /*!< service id.  */ 
    l_u8  data[5];      /*!< Number of bytes in the data field.  */
} lin_pdu_sf_req_t; 

/**
* @brief A structure to represent LIN for single respone frame.
*/
typedef struct { 
    l_u8  nad;          /*!< Node address.  */ 
    struct {
        l_u8   len:4;   /*!< message length */
        l_u8   type:4;  /*!< type is 0x00 for single frame */
    } pci;              /*!< protocol control information.  */ 
    l_u8  rsid;         /*!< service id.  */ 
    l_u8  data[5];      /*!< Number of bytes in the data field.  */ 
} lin_pdu_sf_res_t; 

/**
* @brief A structure to represent LIN first request frame for multi frame pdu.
*/
typedef struct { 
    l_u8  nad;          /*!< Node address.  */ 
    struct {
        l_u8   len:4;   /*!< message length */
        l_u8   type:4;  /*!< type is 0x00 for single frame */
    } pci;              /*!< protocol control information.  */ 
    l_u8  len;          /*!< message length, least significant bits.  */ 
    l_u8  sid;          /*!< service id.  */ 
    l_u8  data[4];      /*!< Number of bytes in the data field.  */ 
} lin_pdu_ff_req_t; 

/**
* @brief A structure to represent LIN first respone frame for multi frame pdu.
*/
typedef struct { 
    l_u8  nad;          /*!< Node address.  */ 
    struct {
        l_u8   len:4;   /*!< message length */
        l_u8   type:4;  /*!< type is 0x00 for single frame */
    } pci;              /*!< protocol control information.  */ 
    l_u8  len;          /*!< message length, least significant bits.  */ 
    l_u8  rsid;         /*!< service id.  */ 
    l_u8  data[4];      /*!< Number of bytes in the data field.  */ 
} lin_pdu_ff_res_t; 

/**
* @brief A structure to represent LIN consecutive frame for multi frame pdu.
*/
typedef struct { 
    l_u8  nad;          /*!< Node address.  */ 
    struct {
        l_u8   len:4;   /*!< message length */
        l_u8   type:4;  /*!< type is 0x00 for single frame */
    } pci;              /*!< protocol control information.  */ 
    l_u8  data[6];      /*!< Number of bytes in the data field.  */ 
} lin_pdu_cf_t; 

extern lin_status_t lin_status;

/**
 * @brief The function prototypes required by LIN specification.
 */
l_irqmask l_sys_irq_disable(void); 
void l_sys_irq_restore(l_irqmask previous); 
l_bool l_sys_init(void); 
void l_ifc_init(l_ifc_handle ifc); 
l_bool l_ifc_connect(l_ifc_handle ifc); 
l_bool l_ifc_disconnect(l_ifc_handle ifc); 
l_bool l_flg_tst(l_flag_handle flag); 
void l_flg_clr(l_flag_handle flag); 
l_u16 l_ifc_read_status(l_ifc_handle ifc); 
void l_ifc_goto_sleep(l_ifc_handle ifc); 
void l_ifc_wake_up(l_ifc_handle ifc); 
void l_sch_set(l_ifc_handle ifc, l_schedule_handle schedule, l_u8 entry); 
l_u8 l_sch_tick(l_ifc_handle ifc); 
l_bool l_bool_rd(l_signal_handle signal); 
l_u8 l_u8_rd(l_signal_handle signal); 
l_u16 l_u16_rd(l_signal_handle signal); 
void l_bool_wr(l_signal_handle signal, l_bool v); 
void l_u8_wr(l_signal_handle signal, l_u8 v); 
void l_u16_wr(l_signal_handle signal, l_u16 v); 
void l_bytes_rd(l_signal_handle signal, l_u8 start, l_u8 count, l_u8 *const data); 
void l_bytes_wr(l_signal_handle signal, l_u8 start, l_u8 count, l_u8 const *data); 
void l_ifc_rx(l_ifc_handle ifc); 
void l_ifc_tx(l_ifc_handle ifc); 
void l_ifc_aux(l_ifc_handle ifc); 
l_u16 l_ifc_ioctl(l_ifc_handle ifc, l_ioctl_op op, void *pv); 
l_bool ld_is_ready(l_ifc_handle ifc); 
void ld_check_response(l_ifc_handle ifc, l_u8 *RSID, l_u8 *error_code); 
void ld_assign_NAD(l_ifc_handle ifc, l_u8 NAD, l_u16 supplier_id, l_u16 function_id, l_u8 new_NAD);
void ld_assign_frame_id_range (l_ifc_handle ifc, l_u8 NAD, l_u8 start_index, const l_u8* const PIDs);
void ld_assign_frame_id(l_ifc_handle ifc, l_u8 NAD, l_u16 supplier_id, l_u16 message_id, l_u8 PID); 
void ld_read_by_id(l_ifc_handle ifc, l_u8 NAD, l_u16 supplier_id, l_u16 function_id, l_u8 id, l_u8 const *data); 
void ld_conditional_change_NAD(l_ifc_handle ifc, l_u8 NAD, l_u8 id, l_u8 byte, l_u8 mask, l_u8 invert, l_u8 new_NAD); 
l_u8 ld_read_configuration(l_ifc_handle ifc, l_u8  *const data, l_u8 *const length);
l_u8 ld_set_configuration(l_ifc_handle ifc, l_u8 const *data, l_u8 const length);
void ld_put_raw(l_ifc_handle ifc, l_u8 const *data); 
void ld_get_raw(l_ifc_handle ifc, l_u8 const *data); 
l_u8 ld_raw_tx_status(l_ifc_handle ifc); 
l_u8 ld_raw_rx_status(l_ifc_handle ifc); 
void ld_send_message(l_ifc_handle ifc, l_u16 length, l_u8 NAD, l_u8 const *data); 
void ld_receive_message(l_ifc_handle ifc, l_u16 const *length, l_u8 const *NAD, l_u8 const *data); 
l_u8 ld_tx_status(l_ifc_handle ifc); 
l_u8 ld_rx_status(l_ifc_handle ifc); 
void ld_init(l_ifc_handle ifc); 

#endif /* __LIN_STACK_H__  */