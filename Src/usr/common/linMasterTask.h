#ifndef _LINM_TASK_H
#define _LINM_TASK_H

#include <appConfig.h>
#include <softTimerTask.h>
#include "systick_device.h"
#include <taskManager.h>
#include <lin_device.h>
#include <linStack.h>

#include <ColorMixingTask.h>

typedef union{
    uint8_t command;
    struct{
        uint8_t command;
        uint8_t transitionTime;/* 100ms/count */
        uint16_t x;
        uint16_t y;
        uint16_t Y;
    }ColorXY;
    struct{
        uint8_t  command;
        uint8_t transitionTime;
        uint16_t hue;
        uint16_t saturation;
        uint16_t level;
    }ColorHSL;
    struct{
        uint8_t  command;
        uint8_t transitionTime;
        uint16_t level;
        uint8_t  red;
        uint8_t  green;
        uint8_t  blue;
    }ColorRGB;
    struct{
        uint8_t  command;
        uint8_t transitionTime;
        uint16_t  PWM_Red;
        uint16_t  PWM_Green;
        uint16_t  PWM_Blue;
    }ColorPWM;
    uint8_t  data[LIN_BUFF_SIZE];       /*!< data buffer. */
}Color_CMDM_Frame_t;


typedef enum{
    COLOR_CMDM_XYY = 0x00,
    COLOR_CMDM_HSL = 0x01,
    COLOR_CMDM_RGB = 0x02,
    COLOR_CMDM_PWM = 0x03,
}ColorMCommand_t;









void LINM_TaskHandler(void);














#endif