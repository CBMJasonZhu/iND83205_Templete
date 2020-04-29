/**
 * @copyright 2018 indie Semiconductor
 * 
 * This file is proprietary to indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of indie Semiconductor.
 * 
 * @file lins_sfr.h
 */

#ifndef LINS_SFR_H__
#define LINS_SFR_H__


#define E_LIN_STATUS_COMPLETE       (1U << 0)
#define E_LIN_STATUS_WAKEUP         (1U << 1)
#define E_LIN_STATUS_ERROR          (1U << 2)
#define E_LIN_STATUS_INT_REQ        (1U << 3)
#define E_LIN_STATUS_DATA_REQ       (1U << 4)
#define E_LIN_STATUS_ABORT          (1U << 5)
#define E_LIN_STATUS_IDLE_TIMEOUT   (1U << 6)
#define E_LIN_STATUS_ACTIVE         (1U << 7)

#define E_LIN_ERROR_BIT             (1U << 0)
#define E_LIN_ERROR_CHECKSUM        (1U << 1)
#define E_LIN_ERROR_TIMEOUT         (1U << 2)
#define E_LIN_ERROR_PARITY          (1U << 3)


#define E_LIN_TIME_INACTIVE_SEC_4       (0U)
#define E_LIN_TIME_INACTIVE_SEC_6       (1U)
#define E_LIN_TIME_INACTIVE_SEC_8       (2U)
#define E_LIN_TIME_INACTIVE_SEC_10      (3U)
#define E_LIN_TIME_INACTIVE_MAX         (3U) /*E_LIN_TIME_INACTIVE_SEC_10*/

#define E_LIN_TIME_WAKEUP_REPEAT_MS_180 (0U)
#define E_LIN_TIME_WAKEUP_REPEAT_MS_200 (1U)
#define E_LIN_TIME_WAKEUP_REPEAT_MS_220 (2U)
#define E_LIN_TIME_WAKEUP_REPEAT_MS_240 (3U)
#define E_LIN_TIME_WAKEUP_REPEAT_MAX    (3U)/*E_LIN_TIME_WAKEUP_REPEAT_MS_240*/


typedef enum{
    E_LIN_BAUDRATE_9600 = 0U,
    E_LIN_BAUDRATE_19200= 1U,
}LIN_BaudRate_t;
#define E_LIN_BAUDRATE_MAX   (1U)/*E_LIN_BAUDRATE_19200*/

#define E_LIN_MODE_SLAVE        (0U)
#define E_LIN_MODE_MASTER       (1U)
#define E_LIN_MODE_MAX          (1U)/*E_LIN_MODE_MASTER*/

#define E_LIN_CHKSUM_CLASSIC    (0U)
#define E_LIN_CHKSUM_ENHANCED   (1U)

#define E_LIN_POLARITY_HIGH     (0U)
#define E_LIN_POLARITY_LOW      (1U)

#define E_LIN_DATA_BUFFER_LENGTH    (8U)


typedef struct {
   uint32_t DATA:  8;
   uint32_t     : 24; /*    (reserved) */
}LINS_DataBuff_t;



#define LINS_DATABYTE1_REG     (*(__IO uint32_t *)(0x50010700))
#define LINS_DATABYTE2_REG     (*(__IO uint32_t *)(0x50010704))
#define LINS_DATABYTE3_REG     (*(__IO uint32_t *)(0x50010708))
#define LINS_DATABYTE4_REG     (*(__IO uint32_t *)(0x5001070C))
#define LINS_DATABYTE5_REG     (*(__IO uint32_t *)(0x50010710))
#define LINS_DATABYTE6_REG     (*(__IO uint32_t *)(0x50010714))
#define LINS_DATABYTE7_REG     (*(__IO uint32_t *)(0x50010718))
#define LINS_DATABYTE8_REG     (*(__IO uint32_t *)(0x5001071C))
#define LINS_CTRL_REG          (*(__IO uint32_t *)(0x50010720))
#define LINS_STATUS_REG        (*(__IO uint32_t *)(0x50010724))
#define LINS_ERROR_REG         (*(__IO uint32_t *)(0x50010728))
#define LINS_DL_REG            (*(__IO uint32_t *)(0x5001072C))
#define LINS_BTDIV07_REG       (*(__IO uint32_t *)(0x50010730))
#define LINS_BITTIME_REG       (*(__IO uint32_t *)(0x50010734))
#define LINS_ID_REG            (*(__IO uint32_t *)(0x50010738))
#define LINS_BUSTIME_REG       (*(__IO uint32_t *)(0x5001073C))
/**
 * @brief A structure to represent Special Function Registers for LINS.
 */
typedef struct {
   LINS_DataBuff_t  DATABUFF[8];
   struct {
         uint32_t          :  1; /*    (reserved) */
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
   } STATUS;

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
         uint32_t       :  5; /*    (reserved) */
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

} LINS_SFRS_t;

/**
 * @brief The starting address of LINS SFRS.
 */
#define LINS_SFRS ((__IO LINS_SFRS_t *)0x50010700)

#endif /* end of __LINS_SFR_H__ section */

