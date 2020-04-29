#ifndef _LINS_TASK_H
#define _LINS_TASK_H

#include <appConfig.h>
#include <softTimerTask.h>
#include "systick_device.h"
#include <taskManager.h>
#include <lin_device.h>
#include <pmu_device.h>
#include <flash_device.h>

#include <ColorMixingTask.h>
#include <isrfuncs.h>
#include <measureTask.h>

/**
* @brief A structure to represent LIN frame information.
*/

#define NODE_ADDR_BROADCAST      0xFFU
#define FRAME_ID_COLOR_CONTROL   0x20U


enum{
  AUTO_ADDRESSING_IDLE = 0x00,
  AUTO_ADDRESSING_INIT = 0x01,
  AUTO_ADDRESSING_NAD = 0x02,
  AUTO_ADDRESSING_STORE = 0x03,
  AUTO_ADDRESSING_END = 0x04,
};



enum{
  DIGNOSE_CMD_SET_FRAME_ID = 0x01,
};


enum{
  DIGNOSE_CMD_GET_PN_VOLT = 0x00,
  DIGNOSE_CMD_GET_PN_TEMP = 0x01,
  DIGNOSE_CMD_GET_BATT_VOLT = 0x02,
  DIGNOSE_CMD_GET_CHIP_TEMP = 0x03,
  DIGNOSE_CMD_GET_TEST_PN_VOLT = 0x04,
  DIGNOSE_CMD_GET_TEST_PN_TEMP = 0x05,
  
  DIGNOSE_CMD_SET_COLOR_RED_PARAM = 0x10,
  DIGNOSE_CMD_GET_COLOR_RED_PARAM = 0x11,
  DIGNOSE_CMD_SET_COLOR_GREEN_PARAM = 0x12,
  DIGNOSE_CMD_GET_COLOR_GREEN_PARAM = 0x13,
  DIGNOSE_CMD_SET_COLOR_BLUE_PARAM  = 0x14,
  DIGNOSE_CMD_GET_COLOR_BLUE_PARAM  = 0x15,
  DIGNOSE_CMD_SET_COLOR_WHITE_PARAM = 0x16,
  DIGNOSE_CMD_GET_COLOR_WHITE_PARAM = 0x17,
  DIGNOSE_CMD_SET_COLOR_RGB_MIN_INTENSITY = 0x18,
  DIGNOSE_CMD_GET_COLOR_RGB_MIN_INTENSITY = 0x19,
  DIGNOSE_CMD_SET_PNVOLT_RGB = 0x1A,
  DIGNOSE_CMD_GET_PNVOLT_RGB = 0x1B,
  DIGNOSE_CMD_SAVE_COLOR_PARAM = 0x1C,
};



typedef union{
    struct{
        uint8_t nodeAddress;
        uint8_t command;
    };
    struct{
        uint8_t nodeAddress;
        uint8_t command;
        uint16_t x;
        uint16_t y;
        uint8_t  Y;
        uint8_t transitionTime;/* 100ms/count */
    }ColorXY;
    struct{
        uint8_t  nodeAddress;
        uint8_t  command;
        uint16_t hue;
        uint8_t  saturation;
        uint8_t  level;
        uint8_t  transitionTime;/* 100ms/count */
    }ColorHSL;
    struct{
        uint8_t  nodeAddress;
        uint8_t  command;
        uint8_t  red;
        uint8_t  green;
        uint8_t  blue;
        uint8_t  level;
        uint8_t  transitionTime;/* 100ms/count */
    }ColorRGBL;
    struct{
        uint8_t  nodeAddress;
        uint8_t  command;
        uint8_t  red;
        uint8_t  green;
        uint8_t  blue;
        uint8_t  transitionTime;/* 100ms/count */
    }ColorRGB;
    struct{
        uint8_t  nodeAddress;
        uint8_t  command;
        uint8_t  PWM_Red;
        uint8_t  PWM_Green;
        uint8_t  PWM_Blue;
        uint8_t  transitionTime;/* 100ms/count */
    }ColorPWM;
    uint8_t  data[LIN_BUFF_SIZE];       /*!< data buffer. */
}Color_CMD_Frame_t;


typedef struct{
    uint8_t nodeAddress;
    uint8_t command;
    uint8_t service;
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;
    uint8_t data4;
    uint8_t data5;
}DignoseRequest_t;


typedef struct{
    uint8_t nodeAddress;
    uint8_t command;
    uint8_t rservice;
    uint8_t unused1;
    uint8_t unused2;
    uint8_t unused3;
    uint8_t unused4;
    uint8_t unused5;
}GetIDResponse_t;

typedef struct{
    uint8_t nodeAddress;
    uint8_t PCI;
    uint8_t SID;
    uint16_t Supplier_ID;
    uint16_t Function_ID;
    uint8_t New_NAD;
}DignoseAutoAddress_t;


typedef struct{
    uint8_t  nodeAddress;
    uint8_t  command;
    uint16_t pnVolt[3];
}GetLedPNVolt_t;

typedef struct{
    uint8_t  nodeAddress;
    uint8_t  command;
    int8_t   pnTemperature[3];
}GetLedPNTemp_t;

typedef struct{
    uint8_t  nodeAddress;
    uint8_t  command;
    uint16_t volt;
}GetLedBattVolt_t;

typedef struct{
    uint8_t  nodeAddress;
    uint8_t  command;
    int16_t  temperature;
}GetLedChipTemp_t;

typedef union{
    GetLedPNVolt_t   pnVolt;
    GetLedPNTemp_t   pnTemperature;
    GetLedBattVolt_t battVolt;
    uint8_t payload[8];
}DignoseResponse_t;



typedef struct{
    uint8_t  nodeAddress;
    uint8_t  command;
    Coordinate_t color;
    uint16_t typicalIntensity;
}ConfigLedParam_t;

typedef struct{
    uint8_t  nodeAddress;
    uint8_t  command;
    int16_t pnVolt[3];
}ConfigTypicalLedPNVolt_t;

typedef struct{
    uint8_t  nodeAddress;
    uint8_t  command;
    uint16_t intensity[3];
}ConfigMinLedIntensity_t;






typedef enum{
    COLOR_CMD_XYY  = 0x00,
    COLOR_CMD_HSL  = 0x01,
    COLOR_CMD_RGBL = 0x02,
    COLOR_CMD_RGB  = 0x03,
    COLOR_CMD_PWM  = 0x04
}ColorCommand_t;







void LINS_AutoAddressingSwitchOn(void);
void LINS_AutoAddressingSwitchOff(void);
void LINS_TaskHandler(void);


uint8_t LINS_GetNAD(void);
void LINS_SetNAD(uint8_t nad);



#endif