#ifndef SAFETY_MONITOR_TASK_H
#define SAFETY_MONITOR_TASK_H

#include <appConfig.h>
#include <hwProtection.h>
#include <softTimerTask.h>
#include <taskManager.h>
#include <ColorMixingTask.h>
#include <measureTask.h>
#include <wdt_device.h>



void SAFM_TaskHandler(void);


int8_t SAFM_GetLedTemperature(void);











#endif