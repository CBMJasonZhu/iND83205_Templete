#include <measureTask.h>

#define GET_SCALED_DATA(VALUE, POS)  ( VALUE >= 0? (VALUE >> POS):(-((-VALUE) >> POS)))

#define MES_FLAGS_UPDATE_LED_TEMP       (1U << 0)
#define MES_FLAGS_UPDATE_BATT           (1U << 1)
#define MES_FLAGS_UPDATE_CHIP_TEMP      (1U << 2)

#define AVERAGE_MEASURE_POS     (4U)
#define AVERAGE_MEASURE_GAIN    (1U << AVERAGE_MEASURE_POS)

typedef struct{
    /* Bandgap Volt */
    uint32_t bandgapVolt;
    /* VBat Calib Param */
    int32_t calibVBatVL;      /* battery volt calib point low 8000mV   */
    int32_t calibVBatVH;      /* battery volt calib point high 13500mV */
    int32_t calibVBatVLCode;  /* adc count @ battery volt low(8000mV)  calib point low */
    int32_t calibVBatVHCode;  /* adc count @ battery volt high(8000mV) calib point high */
    /* PN Volt Calib Param */
    int32_t calibVL;          /* calib Point: 8000mV    */
    int32_t calibVH;          /* calib Point: 13500mV   */
    int32_t calibPNLVolt;     /* delta PN volt : 1500mV */
    int32_t calibPNHVolt;     /* delta PN volt : 4000mV */

    int32_t calibCodeVLPNL;   /* VBAT=8V    deltaPN:1500mV adc code */
    int32_t calibCodeVLPNH;   /* VBAT=8V    deltaPN:4000mV adc code */
    int32_t calibCodeVHPNL;   /* VBAT=13.5V deltaPN:1500mV adc code */
    int32_t calibCodeVHPNH;   /* VBAT=13.5V deltaPN:4000mV adc code */
    
    int32_t lowGain;
    int32_t highGain;
    int32_t lowOffset;
    int32_t highOffset;
    
}AdcCompParam_t;
    

typedef struct{
    uint32_t currCode;
    uint16_t buff[AVERAGE_MEASURE_GAIN];
    uint16_t count;
}AdcDatabuff_t;


typedef struct{
    AdcDatabuff_t chipTemp;
    AdcDatabuff_t batt;
    AdcDatabuff_t ledPN[3];
    AdcDatabuff_t ledBatt[3];
}AdcResult_t;

static uint8_t meausureFlags = 0U;
static uint8_t paramUpdateCount = 0U;

static AdcCompParam_t acParam;
static MeasureParam_t msParam;
static uint16_t ledAdcValue     = 0U;
static uint16_t generalAdcValue = 0U;
static AdcResult_t adcResult;
static TaskState_t adcTaskState = TASK_STATE_INIT;
static AdcMeasureItem_t adcItem = ADC_MEASURE_ITEM_NONE;
static uint8_t syncEnable = FALSE;

void measureTimerExpired(SoftTimer_t *timer);
void measureDoneISR(AdcMeasureItem_t item, uint16_t *const result);
void measureInit(void);
void measureStart(void);

void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff);
int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt);
int8_t adc_getVolt(uint16_t adcCode,uint16_t *battVolt);
uint16_t get_led_volt(int16_t battVolt, uint16_t ledAdcCode);
void get_led_pn_volts(uint16_t *voltR,uint16_t *voltG,uint16_t *voltB);
void get_runtime_led_pn_volt(uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB);

void updateBatteryVolt(void);
void updateChipTemperature(void);
void updateLedsTemperature(uint16_t voltR,uint16_t voltG,uint16_t voltB);

static uint16_t battVolt_mV;
static uint16_t pnVolt_mV[3];
static int8_t   pnTemp_C[3];
static int16_t  chipTemp_C;
static uint32_t adcConvertDone = 0U;

static SoftTimer_t measureTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 50U,
    .handler  = measureTimerExpired
};

void measureTimerExpired(SoftTimer_t *timer)
{
    paramUpdateCount ++;
    if (paramUpdateCount >= (AVERAGE_MEASURE_GAIN*4U/8U)){
        paramUpdateCount = 0U;
    }else if (paramUpdateCount == (AVERAGE_MEASURE_GAIN*3U/8U)){
        meausureFlags |= MES_FLAGS_UPDATE_CHIP_TEMP;    /* update chip temperature parameters */
    }else if (paramUpdateCount == (AVERAGE_MEASURE_GAIN*2U/8U)){
        meausureFlags |= MES_FLAGS_UPDATE_BATT;         /* update battery parameters */
    }else if (paramUpdateCount == (AVERAGE_MEASURE_GAIN*1U/8U) ){
        meausureFlags |= MES_FLAGS_UPDATE_LED_TEMP;     /* update led temperature parameters */
    }else{
        
    }
    TM_PostTask(TASK_ID_ADC_MEASURE);
}


void measureDoneISR(AdcMeasureItem_t item, uint16_t *const result)
{
    if (item >= ADC_MEASURE_ITEM_VLED0_VBAT){
        ledAdcValue = result[0];
        generalAdcValue = result[1];
    }else{
        generalAdcValue = result[0];
    }
    adcConvertDone = 1U;
    TM_PostTask(TASK_ID_ADC_MEASURE);
}

void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff)
{
    if (*original == INVALID_PARAM){
        *original = newData;
        *bufferIndex = 0;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            buff[i] = newData;
        }
    }else{
        buff[*bufferIndex] = newData;
    }
    (*bufferIndex) ++;
    if ((*bufferIndex) >= AVERAGE_MEASURE_GAIN){
        (*bufferIndex) = 0U;
    }
}

void measureStart(void)
{
    switch(adcItem){
    case ADC_MEASURE_ITEM_NONE:
      syncEnable = FALSE;
      adcItem = ADC_MEASURE_ITEM_VTEMP;
      break;
    case ADC_MEASURE_ITEM_VTEMP:
      add_data_to_buff(&adcResult.chipTemp.currCode, generalAdcValue, &adcResult.chipTemp.count,adcResult.chipTemp.buff);
      adcItem = ADC_MEASURE_ITEM_VBAT;
      break;
    case ADC_MEASURE_ITEM_VBAT:
      add_data_to_buff(&adcResult.batt.currCode, generalAdcValue, &adcResult.batt.count,adcResult.batt.buff);
      
      if (PWM_GetCurrentPWM(0)== 0U){                           /* Led channel 0  G*/
          adcResult.ledPN[0].currCode =  INVALID_PARAM;
          if (PWM_GetCurrentPWM(1)== 0U){                       /* Led channel 1  B*/
              adcResult.ledPN[1].currCode =  INVALID_PARAM;
              if (PWM_GetCurrentPWM(2)== 0U){                   /* Led channel 2  R*/
                  adcResult.ledPN[2].currCode =  INVALID_PARAM;
                  adcItem = ADC_MEASURE_ITEM_NONE;
              }else{
                  syncEnable = TRUE;
                  adcItem = ADC_MEASURE_ITEM_VLED2_VBAT;
              }
          }else{
              syncEnable = TRUE;
              adcItem = ADC_MEASURE_ITEM_VLED1_VBAT;
          }
      }else{
          syncEnable = TRUE;
          adcItem = ADC_MEASURE_ITEM_VLED0_VBAT;
      }
      break;
      
    case ADC_MEASURE_ITEM_VLED0_VBAT: /* Led channel 0  G*/
      add_data_to_buff(&adcResult.ledPN[0].currCode, ledAdcValue, &adcResult.ledPN[0].count,adcResult.ledPN[0].buff);
      add_data_to_buff(&adcResult.ledBatt[0].currCode, generalAdcValue, &adcResult.ledBatt[0].count,adcResult.ledBatt[0].buff);
      if (PWM_GetCurrentPWM(1)== 0U){               /* Led channel 1  B*/
          adcResult.ledPN[1].currCode =  INVALID_PARAM;
          if (PWM_GetCurrentPWM(2)== 0U){           /* Led channel 2  R*/
              adcResult.ledPN[2].currCode =  INVALID_PARAM;
              adcItem = ADC_MEASURE_ITEM_NONE;
          }else{
              syncEnable = TRUE;
              adcItem = ADC_MEASURE_ITEM_VLED2_VBAT;
          }
      }else{
          syncEnable = TRUE;
          adcItem = ADC_MEASURE_ITEM_VLED1_VBAT;
      }
      break;
    case ADC_MEASURE_ITEM_VLED1_VBAT: /* Led channel 1  B*/
      add_data_to_buff(&adcResult.ledPN[1].currCode, ledAdcValue, &adcResult.ledPN[1].count,adcResult.ledPN[1].buff);
      add_data_to_buff(&adcResult.ledBatt[1].currCode, generalAdcValue, &adcResult.ledBatt[1].count,adcResult.ledBatt[1].buff);
      if (PWM_GetCurrentPWM(2)== 0U){           /* Led channel 2  R*/
          adcResult.ledPN[2].currCode =  INVALID_PARAM;
          adcItem = ADC_MEASURE_ITEM_NONE;
      }else{
          syncEnable = TRUE;
          adcItem = ADC_MEASURE_ITEM_VLED2_VBAT;
      }
      break;
    case ADC_MEASURE_ITEM_VLED2_VBAT: /* Led channel 2  R*/
      add_data_to_buff(&adcResult.ledPN[2].currCode, ledAdcValue, &adcResult.ledPN[2].count,adcResult.ledPN[2].buff);
      add_data_to_buff(&adcResult.ledBatt[2].currCode, generalAdcValue, &adcResult.ledBatt[2].count,adcResult.ledBatt[2].buff);
      adcItem = ADC_MEASURE_ITEM_NONE;
      break;
    default:
      break;
    }
    if (adcItem != ADC_MEASURE_ITEM_NONE){
        ADC_Init(adcItem,syncEnable);
        ADC_Start();
    }
}

int8_t adc_getVolt(uint16_t adcCode,uint16_t *battVolt)
{
    int32_t volt;
    uint32_t uVolt;
    volt = msParam.batt.coefficient * (int32_t)adcCode;
    volt = volt + msParam.batt.offset;
    uVolt = (uint32_t)volt;
    uVolt = uVolt >> MEASURE_GAIN_POS;
    *battVolt = (uint16_t)uVolt;
    return 0;
}


int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt)
{
    int8_t temperature;
    uint8_t temp;
    uint8_t index;
    for (index = 0U; index < TEMP_CMP_NUM_MAX; index++){
        if (deltaPNVolt >= table[index].deltaVoltPN){
            break;
        }
    }
    temp = index << DELTA_TEMP_POS;
    temperature = (int8_t)temp;
    temperature = temperature + MINIMUM_TEMP;
    return temperature;
}

uint16_t get_led_volt(int16_t battVolt, uint16_t ledAdcCode)
{
    
    int32_t codeA,codeB;
    uint32_t codeU;
    int32_t volt; 
    codeA = (int32_t)(battVolt - acParam.calibVL)*acParam.lowGain + acParam.lowOffset;
    codeB = (int32_t)(battVolt - acParam.calibVL)*acParam.highGain+ acParam.highOffset;
    
    if (codeA >= 0){
        codeU = (uint32_t)codeA;
        codeU = codeU >> 16;
        codeA = (int32_t)codeU;
    }else{
        codeA = -codeA;
        codeU = (uint32_t)codeA;
        codeU = codeU >> 16;
        codeA = (int32_t)codeU;
        codeA = -codeA;
    }
    
    if (codeB >= 0){
        codeU = (uint32_t)codeB;
        codeU = (uint32_t)codeU >> 16;
        codeB = (int32_t)codeU;
    }else{
        codeB = -codeB;
        codeU = (uint32_t)codeB;
        codeU = codeU >> 16;
        codeB = (int32_t)codeU;
        codeB = -codeB;
    }
    
    volt = ((int32_t)ledAdcCode - codeA)*(acParam.calibPNHVolt - acParam.calibPNLVolt)/(codeB - codeA) + acParam.calibPNLVolt;
    return (uint16_t)volt;
}

void get_led_pn_volts(uint16_t *voltR,uint16_t *voltG,uint16_t *voltB)
{
    uint16_t volt[3];
    for (uint8_t index = 0; index < 3U; index++){
        if (adcResult.ledBatt[index].currCode != INVALID_PARAM){
            adcResult.ledBatt[index].currCode = 0U;
            for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
                adcResult.ledBatt[index].currCode += adcResult.ledBatt[index].buff[i];
            }
            adcResult.ledBatt[index].currCode  = adcResult.ledBatt[index].currCode >> AVERAGE_MEASURE_POS;
            (void)adc_getVolt((uint16_t)adcResult.ledBatt[index].currCode,&volt[index]);
            
            /* calculate LED PN volt as refers to VBAT*/
            if (adcResult.ledPN[index].currCode != INVALID_PARAM){
                adcResult.ledPN[index].currCode = 0U;
                for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
                    adcResult.ledPN[index].currCode += adcResult.ledPN[index].buff[i];
                }
                adcResult.ledPN[index].currCode  = adcResult.ledPN[index].currCode >> AVERAGE_MEASURE_POS;
                volt[index] = get_led_volt((int16_t)volt[index], (uint16_t)adcResult.ledPN[index].currCode);
            }else{
                volt[index] = INVALID_VOLT;
            }
        }else{
            volt[index] = INVALID_VOLT;
        }
    }
    *voltR = volt[PHY_CHANNEL_RED];
    *voltG = volt[PHY_CHANNEL_GREEN];
    *voltB = volt[PHY_CHANNEL_BLUE];
}




void updateBatteryVolt(void)
{
    if (adcResult.batt.currCode != INVALID_PARAM){
        adcResult.batt.currCode = 0U;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            adcResult.batt.currCode += adcResult.batt.buff[i];
        }
        adcResult.batt.currCode = adcResult.batt.currCode >> AVERAGE_MEASURE_POS;
        (void)adc_getVolt((uint16_t)adcResult.batt.currCode,&battVolt_mV);
    }else{
        battVolt_mV = INVALID_VOLT;
    }
}


void updateChipTemperature(void)
{
    int32_t volt = 0;
    uint32_t uVolt;
    if (adcResult.chipTemp.currCode != INVALID_PARAM){
        adcResult.chipTemp.currCode = 0U;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            adcResult.chipTemp.currCode += adcResult.chipTemp.buff[i];
        }
        adcResult.chipTemp.currCode = adcResult.chipTemp.currCode >> AVERAGE_MEASURE_POS;
        /* get adc volt */
        volt = msParam.adc.coefficient * (int32_t)adcResult.chipTemp.currCode;
        volt = volt + msParam.adc.offset;
        uVolt = (uint32_t)volt;
        uVolt = uVolt >> MEASURE_GAIN_POS; /* mV */
        volt = (int32_t)uVolt;
        
        /* get Temperature */
        volt = msParam.chipTemp.coefficient * volt;
        volt = volt + msParam.chipTemp.offset;
        if (volt <= 0){
            volt = -volt;
            uVolt = (uint32_t)volt;
            uVolt = uVolt >> MEASURE_TEMP_GAIN_POS;
            volt = (int32_t)uVolt;
            volt = - volt;
        }else{
            uVolt = (uint32_t)volt;
            uVolt = uVolt >> MEASURE_TEMP_GAIN_POS;
            volt = (int32_t)uVolt;
        }
        if (volt <= -127){
            chipTemp_C = INVALID_TEMP + 1;
        }else{
            chipTemp_C = (int16_t)volt;
        }
    }else{
        chipTemp_C = INVALID_TEMP;
    }
}


void updateLedsTemperature(uint16_t voltR,uint16_t voltG,uint16_t voltB)
{
    int16_t typicalVolt_R, typicalVolt_G, typicalVolt_B;

    /* get typical LED PN volt @24C*/
    (void)CLM_GetLedPNVolts(ROOM_TEMPERATURE, &typicalVolt_R, &typicalVolt_G, &typicalVolt_B); /* get init PN volt @24C */
    
    /* get LED R volt and temperature */
    if (voltR != INVALID_VOLT){
        pnVolt_mV[LED_RED]= voltR;
        pnTemp_C[LED_RED] = get_led_temperature(LP_GetTempTableRed(),((int16_t)voltR - typicalVolt_R));
    }else{
        pnVolt_mV[LED_RED]= INVALID_VOLT;
        pnTemp_C[LED_RED] = INVALID_TEMP;
    }
    
    /* get LED G volt and temperature */
    if (voltG != INVALID_VOLT){
        pnVolt_mV[LED_GREEN]= voltG;
        pnTemp_C[LED_GREEN] = get_led_temperature(LP_GetTempTableGreen(),((int16_t)voltG - typicalVolt_G));
    }else{
        pnVolt_mV[LED_GREEN]= INVALID_VOLT;
        pnTemp_C[LED_GREEN] = INVALID_TEMP;
    }
    
    /* get LED B volt and temperature */
    if (voltB != INVALID_VOLT){
        pnVolt_mV[LED_BLUE]= voltB;
        pnTemp_C[LED_BLUE] = get_led_temperature(LP_GetTempTableBlue(),((int16_t)voltB - typicalVolt_B));
    }else{
        pnVolt_mV[LED_BLUE]= INVALID_VOLT;
        pnTemp_C[LED_BLUE] = INVALID_TEMP;
    }
}


/*
 *  Get battery voltage in mV
 *  @param [out] battVolt: 0-65535
 *  @return 0: get voltage success, other : failed
 */
int8_t MES_GetBatteryVolt(uint16_t *battVolt)
{
    int8_t result = -1;
    if (battVolt_mV != INVALID_VOLT){
        result = 0;
    }
    *battVolt = battVolt_mV;
    return result;
}

int16_t MES_GetChipTemperature(int16_t *chipTemperature)
{
    int8_t result = -1;
    if (chipTemp_C != INVALID_TEMP){
        result = 0;
    }
    *chipTemperature = chipTemp_C;
    return result;
}


void MES_MnfGetLedPNVoltage(uint16_t *voltR,uint16_t *voltG,uint16_t *voltB)
{
    *voltR = pnVolt_mV[LED_RED];
    *voltG = pnVolt_mV[LED_GREEN];
    *voltB = pnVolt_mV[LED_BLUE];
}

void MES_MnfGetLedChannelTemperature(int8_t *tempR,int8_t *tempG,int8_t *tempB)
{
    *tempR = pnTemp_C[LED_RED];
    *tempG = pnTemp_C[LED_GREEN];
    *tempB = pnTemp_C[LED_BLUE];
}



/*  1:  target led is on, 0:  target led is off*/
 int8_t MES_GetLedTemperature(uint8_t ledOn)
 {
    int8_t temperature;
    int16_t tempSum = 0;
    int16_t pnVoltvalid = 0;
    uint16_t pnVoltR,pnVoltG,pnVoltB;
    if ( ledOn != 0U){/* target led is on*/
        if (PWM_AllChannelsAreOff() == 0){/* current all of leds have been turned off */
            if ( (pnTemp_C[0] == INVALID_TEMP) && (pnTemp_C[1] == INVALID_TEMP) && (pnTemp_C[2] == INVALID_TEMP) ){
                while(PWM_UpdateFInished()!= 0){}   /* if current value has not been taken effect, wait until taking effect */
                get_runtime_led_pn_volt(&pnVoltR, &pnVoltG,&pnVoltB);
                updateLedsTemperature(pnVoltR,pnVoltG,pnVoltB);
            }
        }
    }else{
        if (PWM_AllChannelsAreOff() == 0){/* current all of leds have been turned off, off->off */
            for (uint8_t i = 0U; i < 3U; i++){
                pnTemp_C[i] = ROOM_TEMPERATURE;
            }
        }
    }
    
    for (uint8_t i = 0U; i < 3U; i++){
        if ( pnTemp_C[i] != INVALID_TEMP){
            tempSum +=pnTemp_C[i];
            pnVoltvalid++;
        }
    }
    
    if (pnVoltvalid != 0){
        temperature = (int8_t)(tempSum/pnVoltvalid);
    }else{
        temperature = ROOM_TEMPERATURE;
    }
    return temperature;
 }



void measureInit(void)
{
    int32_t adcCode0_5V;
    int32_t adcCode1_0V;
    int32_t tempVolt_mV_25C;
    
    acParam.bandgapVolt  = HWCFG_SFRS->BANDGAP_VOLT;
    
    acParam.calibVBatVL      = 8000;    /* Battery Volt 8V */
    acParam.calibVBatVH      = 13500;   /* Battery Volt 13.5V */
    acParam.calibVBatVLCode  = (int32_t)HWCFG_SFRS->VBAT_CODE_8V;     /*8V adc count 1650*/
    acParam.calibVBatVHCode  = (int32_t)HWCFG_SFRS->VBAT_CODE_13V5;   /*13.5V adc count */
    
    acParam.calibVL      = 8000;        /* PN @Battery Volt 8V */
    acParam.calibVH      = 13500;       /* PN @Battery Volt 13.5V */
    acParam.calibPNLVolt = 1500;        /* Delta PN 1.5V*/
    acParam.calibPNHVolt = 4000;        /* Delta PN 4.0V*/
    
    acParam.calibCodeVLPNL   = (int32_t)HWCFG_SFRS->LED_VL_PNL_CODE;
    acParam.calibCodeVLPNH   = (int32_t)HWCFG_SFRS->LED_VL_PNH_CODE;
    acParam.calibCodeVHPNL   = (int32_t)HWCFG_SFRS->LED_VH_PNL_CODE;
    acParam.calibCodeVHPNH   = (int32_t)HWCFG_SFRS->LED_VH_PNH_CODE;
    
    acParam.lowGain = (acParam.calibCodeVHPNL - acParam.calibCodeVLPNL)*65535/(acParam.calibVH - acParam.calibVL);
    acParam.highGain= (acParam.calibCodeVHPNH - acParam.calibCodeVLPNH)*65535/(acParam.calibVH - acParam.calibVL);
    acParam.lowOffset  = 65535*acParam.calibCodeVLPNL;
    acParam.highOffset = 65535*acParam.calibCodeVLPNH;
    
    adcResult.chipTemp.currCode = INVALID_PARAM;
    adcResult.batt.currCode     = INVALID_PARAM;
    
    for (uint8_t i = 0U; i < 3U; i++){
        adcResult.ledBatt[i].currCode = INVALID_PARAM;
        adcResult.ledPN[i].currCode   = INVALID_PARAM;
    }

    /****************************************************************************/
    /* Vadc*/
    adcCode0_5V = (int16_t)HWCFG_SFRS->ADC_0V5;
    adcCode1_0V = (int16_t)HWCFG_SFRS->ADC_1V0;
    msParam.adc.coefficient = (1000 - 500)*MEASURE_GAIN/(adcCode1_0V - adcCode0_5V);
    msParam.adc.offset      = 500*MEASURE_GAIN - msParam.adc.coefficient*adcCode0_5V;
    
    /* Vbattery */
    msParam.batt.coefficient = (acParam.calibVBatVH - acParam.calibVBatVL)*MEASURE_GAIN/(acParam.calibVBatVHCode - acParam.calibVBatVLCode);
    msParam.batt.offset      = acParam.calibVBatVL*MEASURE_GAIN - msParam.batt.coefficient*acParam.calibVBatVLCode;
    
    /* Tsensor */
    /* Tsensor_Volt = 2.5162*T_Sensor_Temp + offset  T_Sensor_Temp = (Tsensor_Volt - offset)/2.5162; offset = tempVolt_mV_25C - 2.5162*25(63)
    */
    tempVolt_mV_25C = (int32_t)HWCFG_SFRS->TSensor_mV_25C;
    msParam.chipTemp.coefficient   = MEASURE_TEMP_GAIN;
    msParam.chipTemp.offset        = 25*(int32_t)(1UL << MEASURE_TEMP_GAIN_POS) - MEASURE_TEMP_GAIN*tempVolt_mV_25C;
    
    /*  init ADC */
    ADC_GeneralInit();
    ADC_RegisterIRQ(measureDoneISR);
}


void MES_TaskHandler(void)
{
    uint16_t voltR,voltG,voltB;
  
    switch(adcTaskState){
    case TASK_STATE_INIT:
      measureInit();
      SoftTimer_Start(&measureTimer);
      adcTaskState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      if( (meausureFlags & MES_FLAGS_UPDATE_LED_TEMP) == MES_FLAGS_UPDATE_LED_TEMP){
          meausureFlags &=(~MES_FLAGS_UPDATE_LED_TEMP);
          /* get current LED PN volt */
          get_led_pn_volts(&voltR,&voltG,&voltB);
          /* calculate led temperture*/
          updateLedsTemperature(voltR,voltG,voltB);
      }else if( (meausureFlags & MES_FLAGS_UPDATE_BATT) == MES_FLAGS_UPDATE_BATT){
          meausureFlags &=(~MES_FLAGS_UPDATE_BATT);
          /* calculate battery volt */
          updateBatteryVolt();
      }else if( (meausureFlags & MES_FLAGS_UPDATE_CHIP_TEMP) == MES_FLAGS_UPDATE_CHIP_TEMP){
          meausureFlags &=(~MES_FLAGS_UPDATE_CHIP_TEMP);
          /* calculate chip temperature */
          updateChipTemperature();
      }else{
          
      }
      measureStart();
      break;
    default:
      break;
    }
}


void get_runtime_led_pn_volt(uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB)
{
    uint16_t ledPNVolt[3];
    uint16_t ledBattVolt[3];
    uint16_t ledBattCode[3] = {0,0,0};
    uint16_t ledCode[3]     = {0,0,0};  
    adcConvertDone = 0U;
    for (uint8_t channel = 0U; channel < 3U; channel++){
        ADC_Init( (AdcMeasureItem_t)((uint8_t)ADC_MEASURE_ITEM_VLED0_VBAT + channel), TRUE);
        for (uint8_t i = 0U; i < (AVERAGE_MEASURE_GAIN >> 1); i++){
            ADC_Start();
            ADC_SFRS->CONF.SWSYNCIN = 1;
            ADC_SFRS->CONF.SWSYNCIN = 0;
            while(adcConvertDone == 0U){}
            adcConvertDone = 0U;
            ledBattCode[channel] += generalAdcValue;
            ledCode[channel] += ledAdcValue;
        }
        ledBattCode[channel] = ledBattCode[channel] >> (AVERAGE_MEASURE_POS -1);
        ledCode[channel] = ledCode[channel] >> (AVERAGE_MEASURE_POS -1);
        (void)adc_getVolt(ledBattCode[channel],&ledBattVolt[channel]);
        ledPNVolt[channel] = get_led_volt((int16_t)ledBattVolt[channel], ledCode[channel]);
    }
    *pnVoltR = ledPNVolt[PHY_CHANNEL_RED];
    *pnVoltG = ledPNVolt[PHY_CHANNEL_GREEN];
    *pnVoltB = ledPNVolt[PHY_CHANNEL_BLUE];
    
//    TM_DisableTask(TASK_ID_ADC_MEASURE);/* restart TASK_ID_ADC_MEASURE TASK*/
}
/*
 *  Get Led PN voltage in mV only for manufacturing PN volt reading block mode !!!!!
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return 0: get voltage success, other : failed
 */
void MES_MnftGetLedPNVolt(uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB)
{     
    (void)CLM_SetPWMs(0U, 0U, 0U, 0U);  /* turn off all of channels */
    while(PWM_UpdateFInished()!= 0){}   /* wait until taking effect */
    get_runtime_led_pn_volt(pnVoltR, pnVoltG,pnVoltB);
}

void MES_MnftGetLedPNTemperature(int8_t *pnTempR, int8_t *pnTempG,int8_t *pnTempB)
{
    uint16_t pnVoltR, pnVoltG,pnVoltB;
    (void)CLM_SetPWMs(0U, 0U, 0U, 0U);  /* turn off all of channels */
    while(PWM_UpdateFInished()!= 0){}   /* wait until taking effect */
    get_runtime_led_pn_volt(&pnVoltR, &pnVoltG, &pnVoltB);
    updateLedsTemperature(pnVoltR,pnVoltG,pnVoltB);
    
    *pnTempR = pnTemp_C[LED_RED];
    *pnTempG = pnTemp_C[LED_GREEN];
    *pnTempB = pnTemp_C[LED_BLUE];
    
}


