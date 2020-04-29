/**
 * @copyright 2018 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file linm_sfr.h
 */

#ifndef LINM_SFR_H__
#define LINM_SFR_H__

#define LINM_DATABYTE1_REG     (*(__IO uint32_t *)(0x50010800))
#define LINM_DATABYTE2_REG     (*(__IO uint32_t *)(0x50010804))
#define LINM_DATABYTE3_REG     (*(__IO uint32_t *)(0x50010808))
#define LINM_DATABYTE4_REG     (*(__IO uint32_t *)(0x5001080C))
#define LINM_DATABYTE5_REG     (*(__IO uint32_t *)(0x50010810))
#define LINM_DATABYTE6_REG     (*(__IO uint32_t *)(0x50010814))
#define LINM_DATABYTE7_REG     (*(__IO uint32_t *)(0x50010818))
#define LINM_DATABYTE8_REG     (*(__IO uint32_t *)(0x5001081C))
#define LINM_CTRL_REG          (*(__IO uint32_t *)(0x50010820))
#define LINM_STATUS_REG        (*(__IO uint32_t *)(0x50010824))
#define LINM_ERROR_REG         (*(__IO uint32_t *)(0x50010828))
#define LINM_DL_REG            (*(__IO uint32_t *)(0x5001082C))
#define LINM_BTDIV07_REG       (*(__IO uint32_t *)(0x50010830))
#define LINM_BITTIME_REG       (*(__IO uint32_t *)(0x50010834))
#define LINM_ID_REG            (*(__IO uint32_t *)(0x50010838))
#define LINM_BUSTIME_REG       (*(__IO uint32_t *)(0x5001083C))

typedef struct {
   uint32_t DATA:  8;
   uint32_t         : 24; /*    (reserved) */
}LINM_DataBuff_t;

/**
 * @brief A structure to represent Special Function Registers for LINM.
 */
typedef struct {
    LINM_DataBuff_t  DATABUFF[8];
    struct {
        uint32_t STARTREQ  :  1; /*!< Start Request */
        uint32_t WAKEUPREQ :  1; /*!< WakeUp Request */
        uint32_t RSTERR    :  1; /*!< Reset Error */
        uint32_t RSTINT    :  1; /*!< Reset interrupt */
        uint32_t DATAACK   :  1; /*!< Data Acknowledgement */
        uint32_t TRANSMIT  :  1; /*!< Transmit Operation */
        uint32_t SLEEP     :  1; /*!< Sleep Request */
        uint32_t STOP      :  1; /*!< Stop Register */
        uint32_t          : 24; /*    (reserved) */
    } CTRL;

    struct {
         uint32_t COMPLETE       :  1;
         uint32_t WAKEUP         :  1;
         uint32_t ERROR          :  1; /*!< Lin Error */
         uint32_t INTR           :  1; /*!< Interupt Request */
         uint32_t DATAREQ        :  1; /*!< Data Request */
         uint32_t ABORTED        :  1;
         uint32_t BUSIDLETIMEOUT :  1; /*!< BUS Idle Timeout */
         uint32_t ACTIVE         :  1; /*!< Lin Bus Active */
         uint32_t               : 24; /*    (reserved) */
    }STATUS;

   struct {
         uint32_t BITMON  :  1; /*!< Bit Error */
         uint32_t CHK     :  1; /*!< Checksum Error */
         uint32_t TIMEOUT :  1; /*!< Timeout Error */
         uint32_t PARITY  :  1; /*!< Parity Error */
         uint32_t        : 28; /*    (reserved) */
   } ERROR;

   struct {
         uint32_t LENGTH :  4; /*!< Data Length */
         uint32_t       :  3; /*    (reserved) */
         uint32_t ENHCHK :  1; /*!< Enhancement Check */
         uint32_t       : 24; /*    (reserved) */
   } DL;

   uint32_t BTDIV07:  8;
   uint32_t        : 24; /*    (reserved) */

   struct {
         uint32_t BTDIV8 :  1; /*!< Bt Div Most Significant bit */
         uint32_t BTMULT :  5; /*!< Bt Div Most Significant bit */
         uint32_t PRESCL :  2; /*!< Prescaler Register */
         uint32_t       : 24; /*    (reserved) */
   } BITTIME;

   uint32_t ID:  6;
   uint32_t   : 26; /*    (reserved) */

   struct {
         uint32_t WUPREPEAT   :  2; /*!< wakeup repeat time */
         uint32_t BUSINACTIVE :  2; /*!< Bus Inactivity Time */
         uint32_t            : 28; /*    (reserved) */
   } BUSTIME;

} LINM_SFRS_t;

/**
 * @brief The starting address of LINM SFRS.
 */
#define LINM_SFRS ((__IO LINM_SFRS_t *)0x50010800)

#endif /* end of __LINM_SFR_H__ section */

