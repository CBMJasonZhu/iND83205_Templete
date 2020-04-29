/**
 * @copyright 2015 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file lin_stack.c
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <linstack.h>

extern LIN_Device_Frame_t lins_cmd_table[3];

l_irqmask l_sys_irq_disable(void)
{
    return 0;
}

void l_sys_irq_restore(l_irqmask previous)
{
   
}

l_bool l_sys_init(void)
{
    return (l_bool)0U;
}

void l_ifc_init(l_ifc_handle ifc)
{
}

l_bool l_ifc_connect(l_ifc_handle ifc)
{
    return false;
}

l_bool l_ifc_disconnect(l_ifc_handle ifc)
{
    return false;
}

l_bool l_flg_tst(l_flag_handle flag)
{
    return (l_bool)0U;
}

void l_flg_clr(l_flag_handle flag)
{
/*    LIN_Device_Frame_t *frame = lins_cmd_table + flag;
    frame->flag = 0;
*/
}

l_u16 l_ifc_read_status(l_ifc_handle ifc)
{
    return 0U;
}

void l_ifc_goto_sleep(l_ifc_handle ifc)
{
}

void l_ifc_wake_up(l_ifc_handle ifc)
{
}

void l_sch_set(l_ifc_handle ifc, l_schedule_handle schedule, l_u8 entry)
{
  
}

l_u8 l_sch_tick(l_ifc_handle ifc)
{
    return 0U;
}

l_bool l_bool_rd(l_signal_handle signal)
{
    LIN_Device_Frame_t *frame = lins_cmd_table + signal;
    return (l_bool)frame->data[0];
}

l_u8 l_u8_rd(l_signal_handle signal)
{
    LIN_Device_Frame_t *frame = lins_cmd_table + signal;
    return frame->data[0];
}

l_u16 l_u16_rd(l_signal_handle signal)
{
    LIN_Device_Frame_t *frame = lins_cmd_table + signal;
    return (l_u16)((l_u16)frame->data[0] | ((l_u16)frame->data[1] << 8));
}

void l_bool_wr(l_signal_handle signal, l_bool v)
{
    LIN_Device_Frame_t *frame = lins_cmd_table + signal;
    frame->data[0] = (l_u8)v;
}

void l_u8_wr(l_signal_handle signal, l_u8 v)
{
    LIN_Device_Frame_t *frame = lins_cmd_table + signal;
    frame->data[0] = v;
}

void l_u16_wr(l_signal_handle signal, l_u16 v)
{
    LIN_Device_Frame_t *frame = lins_cmd_table + signal;
    frame->data[0] = (l_u8)(v & 0xFFU);
    frame->data[1] = (l_u8)(v >> 8);
}

void l_bytes_rd(l_signal_handle signal, l_u8 start, l_u8 count, l_u8 *const data)
{
    LIN_Device_Frame_t *frame = lins_cmd_table + signal;
    for (l_u8 i = 0U; i < count; i++){
        data[i] = *(frame->data + start + i);
    }
    /*(void)memcpy((void *)data, frame->data + start, count);*/
}

void l_bytes_wr(l_signal_handle signal, l_u8 start, l_u8 count, l_u8 const *data)
{
    LIN_Device_Frame_t *frame = lins_cmd_table + signal;
    
    for (l_u8 i = 0U; i < count; i++){
       *(frame->data + start + i) =  data[i];
    }
    /*(void)memcpy((frame->data + start), data, count);*/
}

void l_ifc_rx(l_ifc_handle ifc)
{
}

void l_ifc_tx(l_ifc_handle ifc)
{
}

void l_ifc_aux(l_ifc_handle ifc)
{
}

l_u16 l_ifc_ioctl(l_ifc_handle ifc, l_ioctl_op op, void *pv)
{
    return 0U;
}





l_bool ld_is_ready(l_ifc_handle ifc)
{
    return (l_bool)0U;
}

void ld_check_response(l_ifc_handle ifc, l_u8 *RSID, l_u8 *error_code)
{

}

void ld_assign_frame_id_range(l_ifc_handle ifc, l_u8 NAD, l_u8 start_index, const l_u8* const PIDs)
{

}

void ld_assign_frame_id(l_ifc_handle ifc, l_u8 NAD, l_u16 supplier_id, l_u16 message_id, l_u8 PID)
{
}

void ld_read_by_id(l_ifc_handle ifc, l_u8 NAD, l_u16 supplier_id, l_u16 function_id, l_u8 id, l_u8 const *data)
{
}

void ld_conditional_change_NAD(l_ifc_handle ifc, l_u8 NAD, l_u8 id, l_u8 byte, l_u8 mask, l_u8 invert, l_u8 new_NAD)
{
}

l_u8 ld_read_configuration(l_ifc_handle ifc, l_u8  *const data, l_u8 *const length)
{
    l_u8 i, *d, *l;
    LIN_Device_Frame_t *frame;

    if (sizeof(lins_cmd_table)/sizeof(LIN_Device_Frame_t) > ((l_u32)(*length) - 1U)){
        return LD_LENGTH_TOO_SHORT;
    }
    d = (l_u8 *)data;
    l = (l_u8 *)length;
/*    d[0] = lin->State->nad;*/
    for (i = 0U; i < sizeof(lins_cmd_table)/sizeof(LIN_Device_Frame_t); i++) {
        frame = lins_cmd_table + i;
        d[i + 1U] = frame->frame_id;
    }
    *l = (l_u8)(sizeof(lins_cmd_table)/sizeof(LIN_Device_Frame_t) + 1U);
    return LD_READ_OK;
}

l_u8 ld_set_configuration(l_ifc_handle ifc, l_u8 const *data, l_u8 const length)
{
    l_u8 i;
    LIN_Device_Frame_t *frame;

    if (sizeof(lins_cmd_table)/sizeof(LIN_Device_Frame_t)!= ((l_u32)length - 1U)){
        return LD_LENGTH_NOT_CORRECT;
    }

    for (i = 0U; i < length - 1U; i++) {
        frame = lins_cmd_table + i;
        frame->frame_id = data[i+1U];
    }
    return LD_SET_OK;
}

void ld_put_raw(l_ifc_handle ifc, l_u8 const *data)
{
}

void ld_get_raw(l_ifc_handle ifc, l_u8 const *data)
{
}

l_u8 ld_raw_tx_status(l_ifc_handle ifc)
{
    return 0;
}

l_u8 ld_raw_rx_status(l_ifc_handle ifc)
{
    return 0;
}

void ld_send_message(l_ifc_handle ifc, l_u16 length, l_u8 NAD, l_u8 const *data)
{

}

void ld_receive_message(l_ifc_handle ifc, l_u16 const *length, l_u8 const *NAD, l_u8 const *data)
{
}

l_u8 ld_tx_status(l_ifc_handle ifc)
{
    return 0U;
}

l_u8 ld_rx_status(l_ifc_handle ifc)
{
    return 0U;
}

void ld_init(l_ifc_handle ifc)
{
}
 
