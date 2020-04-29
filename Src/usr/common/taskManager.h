#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <atomic.h>
#include <appConfig.h>



/*! The list of task IDs. The IDs are sorted according to descending
priority. For each task ID there is the corresponding task handler function. */
typedef enum
{
  TASK_ID_SOFT_TIMER = 0U,      /*!< Task ID of Global timer*/
  TASK_ID_LINS,                 /*!< Task ID of LIN slave*/
#if LIN_MASTER_EN == 1
  TASK_ID_LINM,                 /*!< Task ID of LIN master*/
#endif
  TASK_ID_COLOR_MIXING,         /*!< Task ID of colorMixing*/
  TASK_ID_ADC_MEASURE,       /*!< Task ID of ADC measurement*/
  TASK_ID_SAFETY_MONITOR,       /*!< Task ID of Safety protection*/
  TASK_ID_COLOR_COMPENSATION,   /*!< Task ID of color compensation*/
  TASK_ID_APPL,                 /*!< Task ID of APPlication purpose for debug and demo*/
} TM_TaskId_t;

typedef enum{
  TASK_STATE_INIT = 0U,
  TASK_STATE_ACTIVE,
  TASK_STATE_IDLE,
}TaskState_t;


/*
 *  brief runs specified task.
 *  @param [in] taskId - task to be run.
 *  @return none
 */
void TM_PostTask(TM_TaskId_t taskId);

/*
 *  brief Enables specified task.
 *  @param [in] taskId - task to be enabled.
 *  @return none
 */
void TM_EnableTask(TM_TaskId_t taskId);

/*
 *  Disables specified task.
 *  @param [in] taskId - task to be disabled.
 *  @return none
 */
void TM_DisableTask(TM_TaskId_t taskId);

/*
 *  brief task manager.
 *  @return none
 */
void TM_RunTasks(void);








#endif