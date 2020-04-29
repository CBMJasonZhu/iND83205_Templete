#include <systemInit.h>
#include <validation.h>

void leds_driver_Init(void);
void gpios_init(void);
void pmu_init(void);


void leds_driver_Init(void)
{
    PWM_Init(PHY_CHANNEL_RED,   PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX, FALSE);
    PWM_Init(PHY_CHANNEL_GREEN, PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX, FALSE);
    PWM_Init(PHY_CHANNEL_BLUE,  PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX, FALSE);
    
    (void)CLM_SetPWMs(0U, 0U, 0U,0U);
    PWM_EnableAllChannels();
    (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_RED,   CURR_30MA,OFF_CURR_2MA);
    (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_GREEN, CURR_30MA,OFF_CURR_2MA);
    (void)PWM_SetMaxChannelCurrent(PHY_CHANNEL_BLUE,  CURR_30MA,OFF_CURR_2MA);
    
}

void gpios_init(void)
{
    /* set gpios to input with power up resistor */
    GPIO_Init(GPIO_PORT_1, GPIO_MUX_GPIO, GPIO_DIR_INPUT,GPIO_PULL_MODE_PULLUP,GPIO_PWM_NONE);
    GPIO_Init(GPIO_PORT_2, GPIO_MUX_GPIO, GPIO_DIR_INPUT,GPIO_PULL_MODE_PULLUP,GPIO_PWM_NONE);
    GPIO_Init(GPIO_PORT_3, GPIO_MUX_GPIO, GPIO_DIR_INPUT,GPIO_PULL_MODE_PULLUP,GPIO_PWM_NONE);
    GPIO_Init(GPIO_PORT_4, GPIO_MUX_GPIO, GPIO_DIR_INPUT,GPIO_PULL_MODE_PULLUP,GPIO_PWM_NONE);
}

void pmu_init(void)
{
    /* Init set BOR voltage level for cpu low voltage safety*/
    PMU_BORInit(BOR_1V5_1400mV, BOR_3V3_3000mV);
    /* Disable wake up timer */
    PMU_WakeTimerInit(WAKEUP_TIMEER_DISABLE, WAKEUP_TIMEER_INTERVAL_32768ms);
}



void SYS_Init(void)
{
    /* Init system clock */
    Clock_SystemMainClockInit(CLOCK_RC_16MHz, SYS_MAIN_CLOCK_DIV);
    pmu_init();
    /* Init global timer engine for driving soft timer */
    SysTick_Init(SOFT_TIMER_INTERVAL *1000U * MAIN_CPU_CLOCK, SoftTimer_ExpireCallback);
#if WATCH_DOG_EN == 1U
      WDTA_Enable(WDTA_MODE_RESET, WDTA_INTERVAL_1000MS, NULL); /* 1s */
#endif
    /* Init gpios settings */
    gpios_init();
    /* Init LED current and PWM settings */
    leds_driver_Init();

    /* tasks init must be called before use. */
    TM_PostTask(TASK_ID_SOFT_TIMER);
    TM_PostTask(TASK_ID_SAFETY_MONITOR);
    TM_PostTask(TASK_ID_LINS);
#if LIN_MASTER_EN == 1
    TM_PostTask(TASK_ID_LINM);
#endif
    TM_PostTask(TASK_ID_ADC_MEASURE);
    TM_PostTask(TASK_ID_COLOR_MIXING);
    TM_PostTask(TASK_ID_COLOR_COMPENSATION);
    TM_PostTask(TASK_ID_APPL);
}



