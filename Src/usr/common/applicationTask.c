#include <applicationTask.h>
#include <math.h>

static TaskState_t applState = TASK_STATE_INIT;
void ApplTimerExpired(SoftTimer_t *timer);

#define COLOR_MAX  65536U
#define I_RADIO_MAX 255U

static SoftTimer_t ApplTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 100U,
    .handler  = ApplTimerExpired
};



typedef struct{
    uint16_t x;
    uint16_t y;
    uint16_t Y;
}xyY_t;

#define TABLE_SIZE 70U
static xyY_t xyYTable[TABLE_SIZE] = {
  {(uint16_t)(0.175*COLOR_MAX),(uint16_t)(0.05*COLOR_MAX),1023U},
  {(uint16_t)(0.185*COLOR_MAX),(uint16_t)(0.11*COLOR_MAX),1023U},
  {(uint16_t)(0.195*COLOR_MAX),(uint16_t)(0.17*COLOR_MAX),1023U},
  {(uint16_t)(0.205*COLOR_MAX),(uint16_t)(0.23*COLOR_MAX),1023U},
  {(uint16_t)(0.215*COLOR_MAX),(uint16_t)(0.29*COLOR_MAX),1023U},
  {(uint16_t)(0.225*COLOR_MAX),(uint16_t)(0.35*COLOR_MAX),1023U},
  {(uint16_t)(0.235*COLOR_MAX),(uint16_t)(0.41*COLOR_MAX),1023U},
  {(uint16_t)(0.245*COLOR_MAX),(uint16_t)(0.47*COLOR_MAX),1023U},
  {(uint16_t)(0.255*COLOR_MAX),(uint16_t)(0.53*COLOR_MAX),1023U},
  {(uint16_t)(0.265*COLOR_MAX),(uint16_t)(0.59*COLOR_MAX),1023U},
  {(uint16_t)(0.275*COLOR_MAX),(uint16_t)(0.65*COLOR_MAX),1023U},
  {(uint16_t)(0.220*COLOR_MAX),(uint16_t)(0.08*COLOR_MAX),1023U},
  {(uint16_t)(0.230*COLOR_MAX),(uint16_t)(0.14*COLOR_MAX),1023U},
  {(uint16_t)(0.240*COLOR_MAX),(uint16_t)(0.20*COLOR_MAX),1023U},
  {(uint16_t)(0.260*COLOR_MAX),(uint16_t)(0.26*COLOR_MAX),1023U},
  {(uint16_t)(0.260*COLOR_MAX),(uint16_t)(0.32*COLOR_MAX),1023U},
  
  {(uint16_t)(0.27*COLOR_MAX),(uint16_t)(0.38*COLOR_MAX),1023U},
  {(uint16_t)(0.28*COLOR_MAX),(uint16_t)(0.44*COLOR_MAX),1023U},
  {(uint16_t)(0.29*COLOR_MAX),(uint16_t)(0.50*COLOR_MAX),1023U},
  {(uint16_t)(0.30*COLOR_MAX),(uint16_t)(0.56*COLOR_MAX),1023U},
  {(uint16_t)(0.31*COLOR_MAX),(uint16_t)(0.62*COLOR_MAX),1023U},
  {(uint16_t)(0.27*COLOR_MAX),(uint16_t)(0.11*COLOR_MAX),1023U},
  {(uint16_t)(0.28*COLOR_MAX),(uint16_t)(0.17*COLOR_MAX),1023U},
  {(uint16_t)(0.29*COLOR_MAX),(uint16_t)(0.23*COLOR_MAX),1023U},
  {(uint16_t)(0.30*COLOR_MAX),(uint16_t)(0.30*COLOR_MAX),1023U},
  {(uint16_t)(0.31*COLOR_MAX),(uint16_t)(0.35*COLOR_MAX),1023U},
  {(uint16_t)(0.32*COLOR_MAX),(uint16_t)(0.41*COLOR_MAX),1023U},
  {(uint16_t)(0.33*COLOR_MAX),(uint16_t)(0.47*COLOR_MAX),1023U},
  {(uint16_t)(0.34*COLOR_MAX),(uint16_t)(0.53*COLOR_MAX),1023U},
  {(uint16_t)(0.35*COLOR_MAX),(uint16_t)(0.59*COLOR_MAX),1023U},
  {(uint16_t)(0.32*COLOR_MAX),(uint16_t)(0.14*COLOR_MAX),1023U},
  {(uint16_t)(0.33*COLOR_MAX),(uint16_t)(0.20*COLOR_MAX),1023U},
  
  {(uint16_t)(0.34*COLOR_MAX),(uint16_t)(0.26*COLOR_MAX),1023U},
  {(uint16_t)(0.35*COLOR_MAX),(uint16_t)(0.32*COLOR_MAX),1023U},
  {(uint16_t)(0.36*COLOR_MAX),(uint16_t)(0.38*COLOR_MAX),1023U},
  {(uint16_t)(0.37*COLOR_MAX),(uint16_t)(0.44*COLOR_MAX),1023U},
  {(uint16_t)(0.38*COLOR_MAX),(uint16_t)(0.50*COLOR_MAX),1023U},
  {(uint16_t)(0.39*COLOR_MAX),(uint16_t)(0.56*COLOR_MAX),1023U},
  {(uint16_t)(0.37*COLOR_MAX),(uint16_t)(0.17*COLOR_MAX),1023U},
  {(uint16_t)(0.38*COLOR_MAX),(uint16_t)(0.23*COLOR_MAX),1023U},
  {(uint16_t)(0.39*COLOR_MAX),(uint16_t)(0.29*COLOR_MAX),1023U},
  {(uint16_t)(0.40*COLOR_MAX),(uint16_t)(0.35*COLOR_MAX),1023U},
  {(uint16_t)(0.41*COLOR_MAX),(uint16_t)(0.41*COLOR_MAX),1023U},
  {(uint16_t)(0.42*COLOR_MAX),(uint16_t)(0.47*COLOR_MAX),1023U},
  {(uint16_t)(0.43*COLOR_MAX),(uint16_t)(0.53*COLOR_MAX),1023U},
  {(uint16_t)(0.42*COLOR_MAX),(uint16_t)(0.19*COLOR_MAX),1023U},
  {(uint16_t)(0.43*COLOR_MAX),(uint16_t)(0.25*COLOR_MAX),1023U},
  {(uint16_t)(0.44*COLOR_MAX),(uint16_t)(0.31*COLOR_MAX),1023U},
  
  {(uint16_t)(0.45*COLOR_MAX),(uint16_t)(0.37*COLOR_MAX),1023U},
  {(uint16_t)(0.46*COLOR_MAX),(uint16_t)(0.43*COLOR_MAX),1023U},
  {(uint16_t)(0.47*COLOR_MAX),(uint16_t)(0.49*COLOR_MAX),1023U},
  {(uint16_t)(0.47*COLOR_MAX),(uint16_t)(0.22*COLOR_MAX),1023U},
  {(uint16_t)(0.48*COLOR_MAX),(uint16_t)(0.28*COLOR_MAX),1023U},
  {(uint16_t)(0.49*COLOR_MAX),(uint16_t)(0.34*COLOR_MAX),1023U},
  {(uint16_t)(0.50*COLOR_MAX),(uint16_t)(0.40*COLOR_MAX),1023U},
  {(uint16_t)(0.51*COLOR_MAX),(uint16_t)(0.46*COLOR_MAX),1023U},
  {(uint16_t)(0.52*COLOR_MAX),(uint16_t)(0.25*COLOR_MAX),1023U},
  {(uint16_t)(0.53*COLOR_MAX),(uint16_t)(0.31*COLOR_MAX),1023U},
  {(uint16_t)(0.54*COLOR_MAX),(uint16_t)(0.37*COLOR_MAX),1023U},
  {(uint16_t)(0.55*COLOR_MAX),(uint16_t)(0.43*COLOR_MAX),1023U},
  {(uint16_t)(0.57*COLOR_MAX),(uint16_t)(0.29*COLOR_MAX),1023U},
  {(uint16_t)(0.58*COLOR_MAX),(uint16_t)(0.37*COLOR_MAX),1023U},
  {(uint16_t)(0.61*COLOR_MAX),(uint16_t)(0.32*COLOR_MAX),1023U},
  {(uint16_t)(0.65*COLOR_MAX),(uint16_t)(0.32*COLOR_MAX),1023U},
  
  {(uint16_t)(0.27*COLOR_MAX),(uint16_t)(0.111477957*COLOR_MAX),1023U},
  {(uint16_t)(0.164138165*COLOR_MAX),(uint16_t)(0.59*COLOR_MAX),1023U},
  {(uint16_t)(0.30816128*COLOR_MAX),(uint16_t)(0.59*COLOR_MAX),1023U},
  
  {(uint16_t)(0.6880*COLOR_MAX),(uint16_t)(0.30852*COLOR_MAX),1023U},
  {(uint16_t)(0.1681*COLOR_MAX),(uint16_t)(0.69382*COLOR_MAX),1023U},
  {(uint16_t)(0.1436*COLOR_MAX),(uint16_t)(0.05192*COLOR_MAX),1023U},
};


RGB_Slave rgbs;
int16_t delta_level;
uint8_t trans_time0;//100ms
uint8_t trans_time1;//100ms
uint16_t start_level_t;
uint8_t target_level_t;
double funcA;
double funcB;
//uint8_t tableIndex;
void Caculate_Func(void)
{
    switch(rgbs.target_type){
    case LEVEL_LINEAR://线性函数
        funcA = (double) delta_level/trans_time1;
      break;
    case LEVEL_SINUS://sin函数
        funcA = (double) 3.1415/trans_time1;
      break;
    case LEVEL_LOG1://指数函数  log函数
        funcA = (double) delta_level/(exp(trans_time1/10.0)-1);//求函数参数
      break;
    case LEVEL_LOG2://log函数  指数函数
        funcA = (double) delta_level/log(trans_time1+1);//求函数参数
      break;
    }
}
void ApplTimerExpired(SoftTimer_t *timer)//这个地方要改成正式版本
{
    int8_t temperature = 0U;
    temperature = MES_GetLedTemperature(rgbs.rgb_sta);
    /*IntensityRatio_t iRatio = {255U,255U,255U};*/
    
    //状态
    if(trans_time1 > 0U){ 
      trans_time1--;
    }
    else{
      if(rgbs.current_level == 0U){
        rgbs.rgb_sta = RGBS_OFF;
      }else{
        rgbs.rgb_sta = RGBS_IDLE;
      }
    }
    
    if(rgbs.rgb_sta == RGBS_LEVELX){
      switch(rgbs.target_type)
      {
      case LEVEL_LINEAR://线性函数     
        //rgbs.current_level = rgbs.current_level + funcA;
        rgbs.current_level = (uint16_t)(start_level_t + (delta_level - funcA * trans_time1));
        CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, 100U);
        break;
      case LEVEL_SINUS:
        rgbs.current_level = (uint16_t)(start_level_t + delta_level / 2 * (1 + cos(funcA * trans_time1)));
        CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, 100U);
        break;
      case LEVEL_LOG1://log函数1
        rgbs.current_level = (uint16_t)(start_level_t + delta_level - funcA * (exp(trans_time1/10.0)-1));
        CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, 100U);
        break;
      case LEVEL_LOG2://log上升
        rgbs.current_level = (uint16_t)(start_level_t + delta_level - funcA * log(trans_time1+1));
        CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, 100U);
        break;
      }
    }
    
    //color:X  time=0  level>0 忽略start
    if(rgbs.current_color == rgbs.target_color){
      if(rgbs.target_time == 0U && rgbs.target_level > 0){
        if(rgbs.rgb_sta == RGBS_IDLE || rgbs.rgb_sta == RGBS_OFF){
          rgbs.current_level = rgbs.target_level;
          CLM_SetXYY(temperature, xyYTable[rgbs.target_color].x,xyYTable[rgbs.target_color].y, rgbs.current_level,0U); 
          rgbs.rgb_sta = RGBS_IDLE;
        } 
      }
    }
    
    if(rgbs.update_flag == 1U){
      rgbs.update_flag = 0U;
      if(rgbs.target_level == rgbs.current_level){//level:N
        if(rgbs.target_color == rgbs.current_color){//level:N color N
          ;//无操作
        }else{//level:N  color:X
          if(rgbs.target_time > 0U){//level:N color:X time>0
            if(rgbs.rgb_sta == RGBS_IDLE || rgbs.rgb_sta == RGBS_OFF){
              trans_time1 = rgbs.target_time;
              rgbs.current_color = rgbs.target_color;
              rgbs.rgb_sta = RGBS_COLORX;
              CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, rgbs.target_time*100); 
            }
          }else{//level:N color:X time=0
            if(rgbs.rgb_sta == RGBS_IDLE || rgbs.rgb_sta == RGBS_OFF){
              rgbs.current_color = rgbs.target_color;
              CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, 0U);
              rgbs.rgb_sta = RGBS_IDLE;
            }
          }
        }
      }
      else{//level:X
        if(rgbs.target_color == rgbs.current_color){//level:X color:N
          if(rgbs.target_time > 0U){
            //level:X  color:N  time>0  亮度变化模式
            if(rgbs.rgb_sta == RGBS_IDLE || rgbs.rgb_sta == RGBS_OFF){
                rgbs.rgb_sta = RGBS_LEVELX;
                trans_time1 = rgbs.target_time;//获取变化时间
                delta_level = rgbs.target_level - rgbs.current_level;//亮度差值
                start_level_t = rgbs.current_level;
                (void)Caculate_Func();
            }
          }else{//level:X color:N time=0
            if(rgbs.rgb_sta == RGBS_IDLE || rgbs.rgb_sta == RGBS_OFF){
                rgbs.current_level = rgbs.target_level;
                CLM_SetXYY(temperature, xyYTable[rgbs.target_color].x, xyYTable[rgbs.target_color].y, rgbs.current_level, 0U);
                if(rgbs.current_level > 0U){//evel:X color:N time=0 level>0
                  rgbs.rgb_sta = RGBS_IDLE;//空闲 
                }else{
                  rgbs.rgb_sta = RGBS_OFF;//switch off
                }                     
            }           
          }
        }else{//level:X color:X
          if(rgbs.target_time > 0U){//level:X color:X time>0
            if(rgbs.rgb_sta == RGBS_IDLE){// switch on
              //先变level color渐变
              trans_time1 = rgbs.target_time;
              rgbs.current_level = rgbs.target_level;
              rgbs.rgb_sta = RGBS_COLORX;
              CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, 0U);
              rgbs.current_color = rgbs.target_color;
              CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, rgbs.target_time*100);   
            }else if(rgbs.rgb_sta == RGBS_OFF){//switch off
              //先变color level渐变
              trans_time1 = rgbs.target_time;//获取变化时间
              delta_level = rgbs.target_level - rgbs.current_level;//亮度差值
              start_level_t = rgbs.current_level;
              rgbs.current_color = rgbs.target_color;
              rgbs.rgb_sta = RGBS_LEVELX;
              CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, 0U);
              (void)Caculate_Func();
            }
          }else{//level:X color:X time=0
            if(rgbs.rgb_sta == RGBS_IDLE || rgbs.rgb_sta == RGBS_OFF){
              rgbs.current_color = rgbs.target_color;
              rgbs.current_level = rgbs.target_level;
              CLM_SetXYY(temperature, xyYTable[rgbs.current_color].x, xyYTable[rgbs.current_color].y, rgbs.current_level, 0U);
              rgbs.rgb_sta = RGBS_IDLE;
            }           
          }
        }
      }
    }
   

     
    /*CLM_SetXYY(ROOM_TEMPERATURE, xyYTable[tableIndex].x,xyYTable[tableIndex].y, 1023U,10);
    tableIndex++;
    if (tableIndex>=70)tableIndex = 0;*/
}


#define RED_MAX_INTENSITY 1120U
#define GREEN_MAX_INTENSITY 1800U
#define BLUE_MAX_INTENSITY 280U
void APPL_TaskHandler(void)
{
    switch(applState){
    case TASK_STATE_INIT:
    {
      Coordinate_t red        = {(uint16_t)(0.6880*COLOR_MAX),(uint16_t)(0.3085*COLOR_MAX)};
      Coordinate_t green      = {(uint16_t)(0.1681*COLOR_MAX),(uint16_t)(0.6938*COLOR_MAX)};
      Coordinate_t blue       = {(uint16_t)(0.1436*COLOR_MAX),(uint16_t)(0.0519*COLOR_MAX)};
      Coordinate_t whitepoint = {(uint16_t)(0.3333*COLOR_MAX),(uint16_t)(0.3333*COLOR_MAX)};
/*
      Coordinate_t red        = {45088U,20218U};
      Coordinate_t green      = {11016U,45468U};
      Coordinate_t blue       = {10407U ,1592U};
      Coordinate_t whitepoint = {21845U,21845U};
*/
      (void)CLM_SetLedPhyParams(LED_RED,   ROOM_TEMPERATURE, &red,   RED_MAX_INTENSITY);
      (void)CLM_SetLedPhyParams(LED_GREEN, ROOM_TEMPERATURE, &green, GREEN_MAX_INTENSITY);
      (void)CLM_SetLedPhyParams(LED_BLUE,  ROOM_TEMPERATURE, &blue,  BLUE_MAX_INTENSITY);
      (void)CLM_SetWhitePointParams(&whitepoint);
      (void)CLM_SetMinimumIntensity(RED_MAX_INTENSITY*65U/100U, GREEN_MAX_INTENSITY*95U/100U, BLUE_MAX_INTENSITY*95U/100U);
      (void)CLM_SetLedPNVolts(ROOM_TEMPERATURE, 1810, 2793, 2601);
      SoftTimer_Start(&ApplTimer);
      applState = TASK_STATE_ACTIVE;
      break;
    }
    case TASK_STATE_ACTIVE:
      break;
    default:
      break;
    }
}
