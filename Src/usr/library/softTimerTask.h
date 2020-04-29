#ifndef GLOBAL_TIMER_H
#define GLOBAL_TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include <taskManager.h>

#define SOFT_TIMER_INTERVAL_POS  3U 
#define SOFT_TIMER_INTERVAL      (1UL << SOFT_TIMER_INTERVAL_POS) /* ms, must be 2^SOFT_TIMER_INTERVAL_POS*/

/*- Types ------------------------------------------------------------------*/
typedef enum{
    TIMER_ONE_SHOT_MODE, /*this a one time timer, when timming expired, timer would be killed automatically*/
    TIMER_PERIODIC_MODE, /*this a period timer, timer would keep running unitl execute SoftTimer_Stop(), or change the mode by manual.*/
}SoftTimerMode_t;


/* Note that: the SoftTimer_t entity MUST be global/static variable!! */ 
typedef struct SoftTimer {
    struct SoftTimer *next;     /* Internal data don't use!! */
    uint32_t timeout;           /* Internal data don't use!! */
    
    uint32_t interval;          /* Timer parameters  unit : ms*/
    SoftTimerMode_t mode;
    uint32_t param;             /* variables for user use*/
    void (*handler)(struct SoftTimer *timer);
}SoftTimer_t;

/*
 *  Start running target timer
 *  @param [in] timer: target defined global/ static timer
 *  @return none
 */
void SoftTimer_Start(SoftTimer_t *timer);
/*
 *  Stop target timer
 *  @param [in] timer: target defined global/ static timer
 *  @return none
 */
void SoftTimer_Stop(SoftTimer_t *timer);
/*
 *  query whether target timer has been started
 *  @param [in] timer: target defined global/ static timer
 *  @return 0: not start, other: statred.
 */
uint8_t SoftTimer_Started(SoftTimer_t *timer);


/* Soft time handle task only for internal use*/
void SoftTimer_TaskHandler(void);
/* Soft time base callback function only for internal use*/
void SoftTimer_ExpireCallback(void);

























#endif