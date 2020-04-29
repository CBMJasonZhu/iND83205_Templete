#include <hwProtection.h>
#include <appConfig.h>
#include <isrfuncs.h>

static hwProtIsrCallback_t ovTemperatureCallback = NULL;
static hwProtIsrCallback_t ovVoltageCallback = NULL;
static hwProtIsrCallback_t uvVoltageCallback = NULL;


void OVTemp_Handler(void)
{
    if (ovTemperatureCallback != NULL){
        ovTemperatureCallback();
    }else{
      
    }
    CRGA_SFRS->RESETCTRL.OVTEMPFLAGCLR = 1U;
}


void UV_Handler(void)
{
    if (uvVoltageCallback != NULL){
        uvVoltageCallback();
    }else{
      
    }
    PMUA_SFRS->VBAT.UV_IRQ_CLR =1U;
}


void OV_Handler(void)
{
    if (ovVoltageCallback != NULL){
        ovVoltageCallback();
    }else{
      
    }
    PMUA_SFRS->VBAT.OV_IRQ_CLR =1U;
}


void HW_PROT_SetOverTmperature(OverTempThres_t threshold, OverTempAction_t action)
{
    CRGA_SFRS->OVTEMPCONFIG.OVTEMPLEVEL = (uint32_t)threshold;
    CRGA_SFRS->OVTEMPACTION.OVTEMP      = (uint32_t)action;
    CRGA_SFRS->OVTEMPCONFIG.TEMP_SENSOR_DIS = 0U;/* enable temperature sensor */
    CRGA_SFRS->OVTEMPCONFIG.OVTEMPENA   = 1U;    /* enable ov temperature action */ 
    
}

void HW_PROT_RegisterOVTemperatureIRQ(hwProtIsrCallback_t callback)
{
    ovTemperatureCallback = callback;
    CRGA_SFRS->RESETCTRL.OVTEMPFLAGCLR = 1U;
    NVIC_EnableIRQ(OVTEMP_IRQn);
}

void HW_PROT_UnRegisterOVTemperatureIRQ(void)
{
    ovTemperatureCallback = NULL;
    CRGA_SFRS->RESETCTRL.OVTEMPFLAGCLR = 1U;
    NVIC_DisableIRQ(OVTEMP_IRQn);
}


void HW_PROT_SetOverVoltage(OverVoltThres_t threshold)
{
    PMUA_SFRS->VBAT.OVLEVEL_SEL    = (uint32_t)threshold;
    PMUA_SFRS->VBAT.OV_MONITOR_ENA = 1U;
}

void HW_PROT_RegisterOverVoltageIRQ(hwProtIsrCallback_t callback)
{
    ovVoltageCallback           = callback;
    PMUA_SFRS->VBAT.OV_IRQ_CLR  = 1U;
    PMUA_SFRS->VBAT.OV_IRQ_ENA  = 1U;
    NVIC_EnableIRQ(OV_IRQn);
}

void HW_PROT_UnRegisterOverVoltageIRQ(void)
{
    ovVoltageCallback = NULL;
    PMUA_SFRS->VBAT.OV_IRQ_CLR  = 1U;
    PMUA_SFRS->VBAT.OV_IRQ_ENA  = 0U;
    NVIC_DisableIRQ(OV_IRQn);
}

void HW_PROT_SetUnderVoltage(UnderVoltThres_t threshold)
{
    PMUA_SFRS->VBAT.UVLEVEL_SEL     = (uint32_t)threshold;
    PMUA_SFRS->VBAT.LOW_MONITOR_ENA = 1U;
}

void HW_PROT_RegisterUnderVoltageIRQ(hwProtIsrCallback_t callback)
{
    uvVoltageCallback           = callback;
    PMUA_SFRS->VBAT.UV_IRQ_CLR  = 1U;
    PMUA_SFRS->VBAT.UV_IRQ_ENA  = 1U;
    NVIC_EnableIRQ(UV_IRQn);
}

void HW_PROT_UnRegisterUnderVoltageIRQ(void)
{
    ovVoltageCallback = NULL;
    PMUA_SFRS->VBAT.UV_IRQ_CLR  = 1U;
    PMUA_SFRS->VBAT.UV_IRQ_ENA  = 0U;
    NVIC_DisableIRQ(UV_IRQn);
}


uint8_t HW_PROT_BattVoltIsTooLow(void)
{
    uint8_t result = (uint8_t)PMUA_SFRS->VBAT.LOW;
    return result;
}

uint8_t HW_PROT_BattVoltIsTooHigh(void)
{
    uint8_t result = (uint8_t)PMUA_SFRS->VBAT.HIGH;
    return result;
}

uint8_t HW_PROT_ChipIsOverHeat(void)
{
    uint8_t result = (uint8_t)CRGA_SFRS->RESETCTRL.OVTEMPFLAG;
    return result;
}

