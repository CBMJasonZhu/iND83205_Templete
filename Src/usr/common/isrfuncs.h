#ifndef ISR_FUNCS_H_
#define ISR_FUNCS_H_


void SysTick_Handler(void);
void BOR_Handler(void);
void WatchdogA_Handler(void);
void UV_Handler(void);
void OV_Handler(void);
void LINS_Handler(void);
void ADC_Handler(void);
void PWM_Handler(void);
void LINM_Handler(void);
void GPIO_Handler(void);
void OVTemp_Handler(void);
void Timer0_Handler( void );
void Timer1_Handler( void );
void Timer2_Handler( void );
void Watchdog_Handler(void);
void WULINS_Handler(void);
void WUGPIO_Handler(void);
void WUTIMER_Handler(void);
void WULINM_Handler(void);
void Lullaby_Handler(void);


#endif