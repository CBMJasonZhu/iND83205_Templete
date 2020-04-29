#ifndef COLOR_MIXING_H_
#define COLOR_MIXING_H_

#include <stdint.h>
#include <stdbool.h>

#include <appConfig.h>
#include <softTimerTask.h>
#include <taskManager.h>
#include <colorFormula.h>

typedef enum{
  FADING_MODE_DERIVATIVE = 0,   /* 微分方式，在亮度低时提高调解精细度 */
  FADING_MODE_PROPORATION,      /* 比例模式,等比例变化 */
}FadingMode_t;


typedef struct{
  uint16_t version;
  uint16_t revision;
  uint16_t subVersion;
  uint16_t build;
}LibVersionInfo_t;


typedef enum{
  COLOR_MODE_PWM = 0,
  COLOR_MODE_RGB,
  COLOR_MODE_HSL,
  COLOR_MODE_RGBL,
  COLOR_MODE_XYY,
  COLOR_MODE_LUV,
  COLOR_MODE_ACCURATE_RGBL,
  COLOR_MODE_ACCURATE_XYY,
  COLOR_MODE_ACCURATE_LUV,
}ColorMode_t;

typedef struct{
  ColorMode_t  colorMode;
  union{
      struct{
        uint16_t pwmR;
        uint16_t pwmG;
        uint16_t pwmB;
      }PWM;
      struct{ 
        uint8_t red;
        uint8_t green;
        uint8_t blue;
      }RGB;
      struct{
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint16_t level;
      }RGBL;
      struct{
        uint16_t hue;
        uint8_t saturation;
        uint16_t level;
      }HSL;
      struct{
        uint16_t x;
        uint16_t y;
        uint16_t Y;
      }XYY;
      struct{
        uint16_t u;
        uint16_t v;
        uint16_t L;
      }LUV;
  };
  IntensityRatio_t iRatio; /*   intensity ratio for RGB*/
}ColorParam_t;

/*
 *  get color mixing library version 
 *  @return LibVersionInfo_t
 */
LibVersionInfo_t CLM_GetColorLibraryVersion(void);

/*
 * set Fading mode 
 *  @param [in] FADING_MODE_DERIVATIVE: derivative mode more accurate at low brightness, FADING_MODE_PROPORATION: proporation in whole range
 *  @return none
 */
void CLM_SetFadingMode(FadingMode_t mode);

/*
 *  Set max limited intensity level for all of color control commands excepts CLM_SetPWMs command, this function often is used to limit the led brightness when Led is hot for led over hot protection.
 *  @param [in] intensity: 0-1024 ,means 0% -100%
 *  @return 0
 */
int8_t CLM_SetLimitedMaxIntensity(uint16_t intensity);

/*
 *  Get max limited intensity level see "CLM_SetLimitedMaxIntensity" function for the specific function introduction .
 *  @param [out] intensity: 0-1024 ,means 0% -100%
 *  @return 0
 */
int8_t CLM_GetLimitedMaxIntensity(uint16_t *intensity);


/*
 *  Set accurate color xyY output with calibration parameters
 *  @param [in] iRatio:0-255 for 85%-100% of max Intensity, for accurate calibrated color Intensity compensation, set all 255 for normal color control
 *  @param [in] temperature: -40-100 in 1C
 *  @param [in] x: 0-65535
 *  @param [in] y: 0-65535
 *  @param [in] Y: 0-100
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0
 */
uint8_t CLM_SetAccurateXYY(IntensityRatio_t iRatio, int8_t temperature, uint16_t x, uint16_t y, uint8_t Y, uint16_t transitionTime);

/*
 *  Set accurate color L'u'v output with calibration parameters
 *  @param [in] iRatio:0-255 for 85%-100% of max Intensity, for accurate calibrated color Intensity compensation, set all 255 for normal color control
 *  @param [in] temperature: -40-100 in 1C
 *  @param [in] u': 0-65535
 *  @param [in] v': 0-65535
 *  @param [in] level: 0-100
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0
 */
uint8_t CLM_SetAccurateLUV(IntensityRatio_t iRatio, int8_t temperature, uint16_t u, uint16_t v, uint8_t level, uint16_t transitionTime);
/*
 *  Set accurate color RGBL output with calibration parameters
 *  @param [in] iRatio:0-255 for 85%-100% of max Intensity, for accurate calibrated color Intensity compensation, set all 255 for normal color control
 *  @param [in] temperature: -40-100 in 1C
 *  @param [in] red: 0-255
 *  @param [in] green: 0-255
 *  @param [in] blue: 0-255
 *  @param [in] level: 0-100
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0
 */
uint8_t CLM_SetAccurateRGBL(IntensityRatio_t iRatio, int8_t temperature, uint8_t red, uint8_t green, uint8_t blue,uint8_t level, uint16_t transitionTime);

/*
 *  Set xyY output 
 *  @param [in] temperature: -40-100 in 1C
 *  @param [in] x: 0-65535
 *  @param [in] y: 0-65535
 *  @param [in] Y: 0-1023
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0
 */
uint8_t CLM_SetXYY(int8_t temperature, uint16_t x, uint16_t y, uint16_t Y, uint16_t transitionTime);

/*
 *  Set Lu'v' output 
 *  @param [in] temperature: -40-100 in 1C
 *  @param [in] u': 0-65535
 *  @param [in] v': 0-65535
 *  @param [in] level: 0-1023
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0
 */
uint8_t CLM_SetLUV(int8_t temperature, uint16_t u, uint16_t v, uint16_t level, uint16_t transitionTime);
/*
 *  Set RGBL output 
 *  @param [in] temperature: -40-100 in 1C
 *  @param [in] red: 0-255
 *  @param [in] green: 0-255
 *  @param [in] blue: 0-255
 *  @param [in] level: 0-1023
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0
 */
uint8_t CLM_SetRGBL(int8_t temperature, uint8_t red, uint8_t green, uint8_t blue,uint16_t level, uint16_t transitionTime);

/*
 *  Set hue saturation level output 
 *  @param [in] temperature: -40-100 in 1C
 *  @param [in] hue: 0-65535
 *  @param [in] saturation: 0-255
 *  @param [in] level: 0-1023
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0
 */
uint8_t CLM_SetHSL(int8_t temperature, uint16_t hue, uint8_t saturation, uint16_t level, uint16_t transitionTime);
/*
 *  Set RGB output 
 *  @param [in] temperature: -40-100 in 1C
 *  @param [in] red: 0-255
 *  @param [in] green: 0-255
 *  @param [in] blue: 0-255
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0
 */
uint8_t CLM_SetRGB(int8_t temperature, uint8_t red, uint8_t green, uint8_t blue, uint16_t transitionTime);

/*
 * Directly Set PWMs output , only for test purpose,don't use for color mixing parameters
 *  @param [in] PWM_R: 0-65535 Red   Channel PWM value
 *  @param [in] PWM_G: 0-65535 Green Channel PWM value
 *  @param [in] PWM_B: 0-65535 Blue  Channel PWM value
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0
 */
uint8_t CLM_SetPWMs(uint16_t pwmR, uint16_t pwmG, uint16_t pwmB, uint16_t transitionTime);

/*
 * Directly Get current PWMs value 
 *  @param [out] PWM_R: 0-65535 Red   Channel PWM value
 *  @param [out] PWM_G: 0-65535 Green Channel PWM value
 *  @param [out] PWM_B: 0-65535 Blue  Channel PWM value
 *  @return 0
 */
uint8_t CLM_GetCurrentPWMs(uint16_t *pwmR, uint16_t *pwmG, uint16_t *pwmB);

/*
 * Get current Fading  status, color compensition only runs when fading is finished.
 *  @param [out] 0: fading is runing, 1: fading is finished
 */
uint8_t CLM_FadingIsFinished(void);

/*
 * Get current color control parameters , this function is purpose to use for color compensation together with CLM_FadingIsFinished function.
 *  @param [out] ColorParam_t, read color mode then get Color Param.
 */ 
ColorParam_t *CLM_GetCurrentColorParams(void);


/*
 * Set Led physical param : xy coordinate, intensity in Lumin (for example)
 *  @param [in] type: LED type
 *  @param [in] temperature: current temperature -40-100 in 1C
 *  @param [in] coordinate: Led coordinate for example:P(0.3333,0.3333) = P(21845,21845);
 *  @param [in] intensity: 0-65535 in mcd (for example)
 *  @return 0
 */
int8_t CLM_SetLedPhyParams(LedType_t type, int8_t temperature, Coordinate_t const *coordinate, uint16_t intensity);

/*
 * Get Led physical param : x,y coordinate, intensity in Lumin (for example)
 *  @param [in] type: LED type
 *  @param [in] temperature: current temperature -40-100 in 0.1C
 *  @param [Out] coordinate: Led coordinate for example:P(0.3333,0.3333) = P(21845,21845);
 *  @param [Out] intensity: 0-65535 in Lumin (for example)
 *  @return 0
 */
int8_t CLM_GetLedPhyParams(LedType_t type, int8_t temperature, Coordinate_t *coordinate, uint16_t *intensity);   

/*
 *  In order to make the stability of light intensity  in different temperature, Red has maximum intensity degration
 *  @param [in] redIntensity:   0-65535 in Lumin (for example)
 *  @param [in] greenIntensity: 0-65535 in Lumin (for example)
 *  @param [in] blueIntensity:  0-65535 in Lumin (for example)
 *  @return 0
 */
int8_t CLM_SetMinimumIntensity(uint16_t redIntensity,uint16_t greenIntensity,uint16_t blueIntensity );

/*
 *  In order to make the stability of light intensity  in different temperature
 *  @param [out] redIntensity:   0-65535 in Lumin (for example)
 *  @param [out] greenIntensity: 0-65535 in Lumin (for example)
 *  @param [out] blueIntensity:  0-65535 in Lumin (for example)
 *  @return 0
 */
int8_t CLM_GetMinimumIntensity(uint16_t *redIntensity,uint16_t *greenIntensity,uint16_t *blueIntensity);

/*
 * Set calibration point physical param : x,y coordinate
 *  @param [in] coordinate: Led whitePoint coordinate for example:P(0.3333,0.3333) = P(21845,21845);
 *  @return 0
 */
int8_t CLM_SetWhitePointParams(Coordinate_t const *coordinate);

/*
 * Get calibration point physical param : x,y coordinate
 *  @param [in] coordinate: Led coordinate for example:P(0.3333,0.3333) = P(21845,21845);
 *  @return coordinate
 */
int8_t CLM_GetWhitePointParams(Coordinate_t *coordinate);

/*
 * Set Led PN volt @specifical temperature in mV
 *  @param [in] temperature: current temperature -40-100 in 1C
 *  @param [in] volt_R: Red Led PN volt (mV);
 *  @param [in] volt_G: Green Led PN volt (mV);
 *  @param [in] volt_B: Blue Led PN volt (mV);
 *  @return 0
 */
int8_t CLM_SetLedPNVolts(int8_t temperature, int16_t volt_R, int16_t volt_G, int16_t volt_B);

/*
 * Get Led PN volt @25C in mV
 *  @param [in] temperature: current temperature -40-100 in 1C
 *  @param [out] volt_R: Red Led PN volt (mV);
 *  @param [out] volt_G: Green Led PN volt (mV);
 *  @param [out] volt_B: Blue Led PN volt (mV);
 *  @return 0
 */
int8_t CLM_GetLedPNVolts(int8_t temperature, int16_t *const volt_R, int16_t *const volt_G, int16_t *const volt_B);

/*
 *  whitePoint intensity calculating, the room temperature is MUST when doing white point calculating.
 *  @param [in] temperature: -40-100 in 1C
 *  @param [in] whitePoint: white point coordinatefor example:P(0.3333,0.3333) = P(21845,21845);
 *  @return 0
 */
int8_t CLM_CalculatingWhitePoint(int8_t temperature,Coordinate_t *whitePoint);

/*
 * White Point Calibration: Set Led PWM parameters for calculating max intensity of R,G,B
 *  @param [in] maxPWM_R: at White Point
 *  @param [in] maxPWM_G: at White Point
 *  @param [in] maxPWM_B: at White Point
 *  @return 0
 */
int8_t CLM_SetWhitePointPWMs(uint16_t maxPWM_R,uint16_t maxPWM_G,uint16_t maxPWM_B);

/*
 * White Point Calibration: Get Led max PWM parameters of R,G,B
 *  @param [out] maxPWM_R: at White Point
 *  @param [out] maxPWM_G: at White Point
 *  @param [out] maxPWM_B: at White Point
 *  @return 0
 */
int8_t CLM_GetWhitePointPWMs(uint16_t *maxPWM_R,uint16_t *maxPWM_G,uint16_t *maxPWM_B);

/*
 * storage color settings to flash
 *  @return 0,OK,others:NG
 */
int8_t CLM_StoragColorParams(void);


void CLM_TaskHandler(void);

#endif