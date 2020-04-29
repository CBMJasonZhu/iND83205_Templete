#include <colorCompensationTask.h>

#define COLOR_COMP_INTERVAL 1000U
#define COLOR_COMP_DIMMING_INTERVAL 0U /* (COLOR_COMP_INTERVAL - 500U)*/
static TaskState_t ccpState = TASK_STATE_INIT;
void doColorCompensationHandle(void);
void colorCompTimerExpired(SoftTimer_t *timer);

static int8_t lastTemperature = -127;

static SoftTimer_t colorCompTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = COLOR_COMP_INTERVAL, /* color compensation detect time interval */
    .handler  = colorCompTimerExpired
};


void colorCompTimerExpired(SoftTimer_t *timer)
{
    /* color compensation only starts until the diming operation has been finished */
    if ( CLM_FadingIsFinished() != 0U){
        TM_PostTask(TASK_ID_COLOR_COMPENSATION);
    }
}

void CCP_TaskHandler(void)
{
    switch(ccpState){
    case TASK_STATE_INIT:
      SoftTimer_Start(&colorCompTimer);
      ccpState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      doColorCompensationHandle(); /* do color compensation operation if temperature has changed */
      break;
    default:
      break;
    }
}

void doColorCompensationHandle(void)
{
    int8_t currTemperature;
    /* Get current LED temperature */
    if (PWM_AllChannelsAreOff() != 0 ){
         currTemperature = MES_GetLedTemperature(TRUE);
         if(currTemperature != lastTemperature){
            lastTemperature = currTemperature;
            ColorParam_t *color = CLM_GetCurrentColorParams();
            switch(color->colorMode){
            case COLOR_MODE_XYY:
              (void)CLM_SetXYY(currTemperature, color->XYY.x, color->XYY.y, color->XYY.Y, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_ACCURATE_XYY:
              (void)CLM_SetAccurateXYY(color->iRatio, currTemperature, color->XYY.x, color->XYY.y, (uint8_t)color->XYY.Y, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_RGBL:
              (void)CLM_SetRGBL(currTemperature, color->RGBL.red, color->RGBL.green, color->RGBL.blue,color->RGBL.level,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_ACCURATE_RGBL:
              (void)CLM_SetAccurateRGBL(color->iRatio,currTemperature, color->RGBL.red, color->RGBL.green, color->RGBL.blue,(uint8_t)color->RGBL.level,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_LUV:
              (void)CLM_SetLUV(currTemperature, color->LUV.u, color->LUV.v, color->LUV.L, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_ACCURATE_LUV:
              (void)CLM_SetAccurateLUV(color->iRatio,currTemperature, color->LUV.u, color->LUV.v, (uint8_t)color->LUV.L, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_RGB:
              (void)CLM_SetRGB(currTemperature, color->RGB.red, color->RGB.green, color->RGB.blue,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_HSL:
              (void)CLM_SetHSL(currTemperature, color->HSL.hue, color->HSL.saturation, color->HSL.level, COLOR_COMP_DIMMING_INTERVAL);
              break;
            default:
              break;
            }
         }
    }
}

