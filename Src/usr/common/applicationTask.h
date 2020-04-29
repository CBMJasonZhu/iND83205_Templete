#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <appConfig.h>
#include <softTimerTask.h>
#include "systick_device.h"
#include <taskManager.h>

#include <ColorMixingTask.h>
#include <lin_device.h>
#include <adc_device.h>
#include <gpio_device.h>
#include <linMasterTask.h>
#include <ledParamTables.h>
#include <timer_device.h>
#include <pmu_device.h>




void APPL_TaskHandler(void);














#endif