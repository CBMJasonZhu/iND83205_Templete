#ifndef LED_TABLES_H
#define LED_TABLES_H

#include <colorFormula.h>

#define TEMP_CMP_NUM_MAX          (61U)

#define MINIMUM_TEMP              (-20)
#define MAXIMUM_TEMP              (100)
#define DELTA_TEMP_POS            (1U)
#define DELTA_TEMP                (1U << DELTA_TEMP_POS)/*must be 2^DELTA_TEMP_POS*/

#define MAX_RED_INTENSITY_INDEX   (51U) /* Max intensity of Red, beasuse red has more degragation*/

typedef struct{
    uint16_t intensityEfficiency; /*efficiency:100.0% means 1024*/
    int16_t deltaVoltPN;          /*mV*/
    uint16_t x;                   /*float rate: 100.0% means 1024*/
    uint16_t y;                   /*float rate: 100.0% means 1024*/
}TempParams_t;

TempParams_t const * LP_GetTempTableRed(void);
TempParams_t const * LP_GetTempTableGreen(void);
TempParams_t const * LP_GetTempTableBlue(void);


























#endif