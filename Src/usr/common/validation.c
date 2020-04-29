#include <validation.h>

__ramfunc void  nvr_write(uint32_t address, uint32_t data);
__ramfunc void  nvr_erase(uint32_t address);

void validation_main(void)
{
//    uint8_t result = 1;
    
//    (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_RED,   CURR_6MA,OFF_CURR_2MA);
//    (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_GREEN, CURR_6MA,OFF_CURR_2MA);
//    (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_BLUE,  CURR_6MA,OFF_CURR_2MA);
//    
//    PWM_Init(PHY_CHANNEL_RED,   PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX, FALSE);
//    PWM_Init(PHY_CHANNEL_GREEN, PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX, FALSE);
//    PWM_Init(PHY_CHANNEL_BLUE,  PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX, FALSE);
//    PWM_Enable(PHY_CHANNEL_RED);
//    PWM_Enable(PHY_CHANNEL_GREEN);
//    PWM_Enable(PHY_CHANNEL_BLUE);
//
//    (void)CLM_SetPWMs(65535U, 65535U, 65535U,0U);
//    while(1){
//        (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_RED,   CURR_6MA,OFF_CURR_2MA);
//        (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_GREEN, CURR_6MA,OFF_CURR_2MA);
//        (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_BLUE,  CURR_6MA,OFF_CURR_2MA);
//
//        (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_RED,   CURR_30MA,OFF_CURR_2MA);
//        (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_GREEN, CURR_30MA,OFF_CURR_2MA);
//        (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_BLUE,  CURR_30MA,OFF_CURR_2MA);
//
//    }

//    uint16_t trim = 0x3EU;
//    uint8_t result = 1U;
//    SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
//    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY   = 0x0EU;
//    SYSCTRLA_SFRS->PMU_TRIM.TRIM = 0x04;
//    IOCTRLA_ANALOGTESTMUXOVERRIDE_REG = 0x01080006U;
//    while(result == 1U){
//    }

}


__ramfunc void  nvr_write(uint32_t address, uint32_t data)
{
    // load address
    FLS_WR_ADDR->ADDR = address;
    FLS_WR_DATA->DATA = data;
    FLS_CTRL_OP_UNLOCK->KEY = FLS_CTRL_OP_UNLOCK_KEY;
    FLS_CTRL_OP->SIZE       = 3;   //Word Write
    FLS_WR_UNLOCK->KEY      = FLS_WR_UNLOCK_KEY;
    FLS_WR_START->KEY       = FLS_ERASE_NVR_KEY;
    // Wait for the write to be done by writing into the Flash Controller
    // The write access will be held until the flash operation is done
    // Any write or read to the flash controller will do the trick...
    FLS_CTRL->CTRL          = 0x00000000;  
  
}

__ramfunc void  nvr_erase(uint32_t address)
{
      // load address
    FLS_WR_ADDR->ADDR = address;
    // do erasing...
    FLS_SECTOR_ERASE_UNLOCK->KEY = FLS_SECTOR_ERASE_UNLOCK_KEY;
    FLS_WR_START->KEY            = FLS_ERASE_NVR_KEY;
    FLS_SECTOR_ERASE_START->KEY  = FLS_SECTOR_ERASE_START_KEY;
     // Wait for the erase to be done by writing into the Flash Controller
    // The write access will be held until the flash erase operation is done
    // Any write or read to the flash controller will do the trick...
    FLS_CTRL->CTRL               = 0x00000000;
}