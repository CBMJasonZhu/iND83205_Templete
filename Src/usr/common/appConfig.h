#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__

#include <clock_device.h>
#include <pwm_device.h>
#include <string.h>

#define TRUE                    (1U)
#define FALSE                   (0U)


/* ************************hardware config ********************************/
#define MAIN_CPU_CLOCK          4U       /*MHz*/

#if MAIN_CPU_CLOCK == 16U
    #define SYS_MAIN_CLOCK_DIV    CLOCK_DIV_1
#elif MAIN_CPU_CLOCK == 8U
    #define SYS_MAIN_CLOCK_DIV    CLOCK_DIV_2
#elif MAIN_CPU_CLOCK == 4U
    #define SYS_MAIN_CLOCK_DIV    CLOCK_DIV_4
#elif MAIN_CPU_CLOCK == 2U
    #define SYS_MAIN_CLOCK_DIV    CLOCK_DIV_8
#endif


#define PHY_CHANNEL_RED    PWM_CHANNEL_0
#define PHY_CHANNEL_GREEN  PWM_CHANNEL_2
#define PHY_CHANNEL_BLUE   PWM_CHANNEL_1

#define PWM_VALUE_MAX_POS               (15U)
#define PWM_VALUE_MAX                   ((1UL << PWM_VALUE_MAX_POS) -1UL)


#define LIN_MASTER_EN           (0U)
#define ROOM_TEMPERATURE        (24)  /*1C*/
/* ************************System config ********************************/

#define WATCH_DOG_EN                            (0U)
#define SAFETY_MONITOR_EN                       (0U)

#define ENABLE_LIN_BUS_IDLE_TIMEOUT_SLEEP       (0U) /* 1: mcu sleep if lin bus idle timeout */

#define USER_PARAM_SIZE 8
//static uint8_t flashBuff[FLASH_SECTOR_SIZE];
extern uint8_t User_Param[USER_PARAM_SIZE];


enum
{
  RGBS_OFF = 0x00,
  RGBS_IDLE = 0x01,
  RGBS_LEVELX = 0x02,
  RGBS_COLORX = 0x03,
};
enum
{
  LEVEL_LINEAR = 0x00,
  LEVEL_SINUS = 0x01,
  LEVEL_LOG1 = 0x02,
  LEVEL_LOG2 = 0x03,
};

typedef struct{
    uint8_t target_color;
    uint8_t current_color;
    uint16_t target_level;
    uint16_t current_level;
    uint8_t target_time;
    uint8_t last_target_time;
    uint8_t target_type;
    uint8_t last_target_type;
    uint8_t update_flag;
    uint8_t rgb_sta;
    uint8_t transition_start;
}RGB_Slave;
extern RGB_Slave rgbs;

#define SAVED_NAD     User_Param[1]
#define SAVED_PID     User_Param[2]

/* ************************debug config ********************************/

#define CODE_DEBUG_EN    1

#if CODE_DEBUG_EN == 1
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #define DEBUG_OUT printf

#else

  #define DEBUG_OUT(...)

#endif



#endif