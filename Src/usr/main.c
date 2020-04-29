#include <systemInit.h>
#include <pwm_device.h>
#include <linSlaveTask.h>

//用户flash 扇区0 不保存用户数据
uint8_t User_Param[USER_PARAM_SIZE];
void main(void)
{
    uint32_t sectorAddr;
    SYS_Init();
    //上电初始化
    sectorAddr = USER_PARAM_ADDRESS_START;//计算扇区地址
    for (uint8_t i = 0U; i < USER_PARAM_SIZE; i++){
        User_Param[i] = ((uint8_t *)sectorAddr)[i];
    }
    rgbs.current_color = 0U;
    rgbs.current_level = 0U;
    rgbs.rgb_sta = RGBS_OFF;
    for(;;){
      TM_RunTasks();
    }
}
