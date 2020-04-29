#ifndef COLOR_FORMULA_H_
#define COLOR_FORMULA_H_

#include <stdint.h>
#include <stdbool.h>
#include <appConfig.h>
#include <ledParamTables.h>
#include <flash_device.h>

typedef enum{
  LED_RED = 0U,
  LED_GREEN,
  LED_BLUE
}LedType_t;

typedef struct{ 
    uint16_t x;         /*0-65535  based on color xy range xy(0.3,0.3) = (19660,19660)*/
    uint16_t y;         /*0-65535  based on color xy range xy(0.3,0.3) = (19660,19660)*/
}Coordinate_t;
  
typedef struct{ 
    uint32_t X;         /* 0-16383*/
    uint32_t Y;         /* 0-16383*/
    uint32_t Z;         /* 0-16383*/
}CoordinateXYZ_t;

typedef struct{
    Coordinate_t        whitePoint;
    Coordinate_t        color[3];
    uint16_t            maxTypicalIntensity[3];/*0-65535 @25C*/
    uint16_t            minIntensity[3];       /*min rgb Intensity, red has more degration */
    int16_t             ledTypicalPNVolt[3];   /* @25C*/
    
    uint8_t             linNAD;
    uint32_t            valid;
}SystemPhyParam_t;

typedef struct{
    uint16_t            ratio[3];       /*0-16383*/
    uint16_t            maxIntensity[3];/*0-65535*/
    uint16_t            maxPWM[3];      /*0-65535*/
}LedRunTimeParam_t;

typedef struct{
    uint16_t red;       /*0-65535*/
    uint16_t green;     /*0-65535*/
    uint16_t blue;      /*0-65535*/
    uint8_t result;     /* 0: OK, other: NG*/
}LedTarget_t;

typedef struct{ 
    uint16_t x;         /*0-65535 based on color xy range xy(0.3,0.3) = (19660,19660)*/
    uint16_t y;         /*0-65535 based on color xy range xy(0.3,0.3) = (19660,19660)*/
}CPoint_t;

typedef struct{ 
    uint16_t Yr;        /*0-16383*/
    uint16_t Yg;        /*0-16383*/
    uint16_t Yb;        /*0-16383*/
}CPointY_t;

/* Intensity error ratio for specific color calibration */
typedef struct{ 
    uint8_t Ir;         /*0-255*/
    uint8_t Ig;         /*0-255*/
    uint8_t Ib;         /*0-255*/
}IntensityRatio_t;


typedef struct{ 
    uint16_t x;         /*0-65535 based on color xy range xy(0.3,0.3) = (19660,19660)*/
    uint16_t y;         /*0-65535 based on color xy range xy(0.3,0.3) = (19660,19660)*/
    uint8_t  Y;         /* 0-100 */
    IntensityRatio_t ratio;
}AccurateColorXYY_t;

typedef struct{ 
    uint16_t u;         /*0-65535  U'V(0.1,0.2) = (6553,131071)*/
    uint16_t v;         /*0-65535*/
    uint8_t  level;     /* 0-100 */
    IntensityRatio_t ratio;
}SpecificColorUVL_t;

typedef struct{ 
    uint8_t red;        /* 0-255 */
    uint8_t green;      /* 0-255 */
    uint8_t blue;       /* 0-255 */
    uint8_t level;      /* 0-100 */
    IntensityRatio_t ratio;
}SpecificColorRGBL_t;

/* The following functions are only for internal usage */
int8_t CFL_SetLedPhyParams(LedType_t type, int8_t temperature, Coordinate_t const *coordinate, uint16_t intensity);
int8_t CFL_GetLedPhyParams(LedType_t type, int8_t temperature, Coordinate_t *coordinate, uint16_t *intensity);
int8_t CFL_SetMinimumIntensity(uint16_t redIntensity,uint16_t greenIntensity,uint16_t blueIntensity );
int8_t CFL_GetMinimumIntensity(uint16_t *redIntensity,uint16_t *greenIntensity,uint16_t *blueIntensity);
int8_t CFL_SetWhitePointParams(Coordinate_t const *coordinate);
Coordinate_t *CFL_GetWhitePointParams(void);
int8_t CFL_SetLedPNVolts(int8_t temperature, int16_t volt_R, int16_t volt_G, int16_t volt_B);
int8_t CFL_GetLedPNVolts(int8_t temperature, int16_t *const volt_R, int16_t *const volt_G, int16_t *const volt_B);
int8_t CFL_CalculatingWhitePoint(int8_t temperature, Coordinate_t const *whitePoint);
int8_t CFL_SetWhitePointPWMs(uint16_t maxPWM_R,uint16_t maxPWM_G,uint16_t maxPWM_B);
int8_t CFL_GetWhitePointPWMs(uint16_t *maxPWM_R,uint16_t *maxPWM_G,uint16_t *maxPWM_B);
int8_t CFL_StorageColorParams(void);
int8_t CFL_RestoreColorParams(void);
LedTarget_t CFL_XYYToDutyCycle(IntensityRatio_t iRatio,int8_t temperature, uint16_t xPoint, uint16_t yPoint, uint16_t Y);
LedTarget_t CFL_LuvToDutyCycle(IntensityRatio_t iRatio,int8_t temperature, uint16_t u, uint16_t v,uint16_t level);
LedTarget_t CFL_RGBLToDutyCycle(IntensityRatio_t iRatio,int8_t temperature,uint8_t Red, uint8_t Green, uint8_t Blue,uint16_t level);
LedTarget_t CFL_HSLToDutyCycle(int8_t temperature, uint16_t hue, uint8_t saturation, uint16_t level);
LedTarget_t CFL_RGBToDutyCycle(int8_t temperature, uint8_t red, uint8_t green, uint8_t blue);
uint16_t CFL_GetRuntimeLedMaxPWM(LedType_t type);
uint16_t CFL_GetRunTimePhyMaxIntensity(LedType_t type,int8_t temperature);
uint8_t getScaleRateFromU32ToU16(uint32_t value);

#endif