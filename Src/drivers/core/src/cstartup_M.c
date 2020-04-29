/**************************************************
 *
 * This file contains an interrupt vector 
 * KamCho. (Cortex-M written in C)
 * The actual interrupt functions must be provided 
 * by the application developer.
 *
 * Copyright 2007 IAR Systems. All rights reserved.
 *
 * $Revision: 3998 $
 *
 **************************************************/

#pragma language=extended
#pragma segment="CSTACK"

#include <isrfuncs.h>

extern void __iar_program_start( void );

extern void NMI_Handler( void );
extern void HardFault_Handler( void );
extern void MemManage_Handler( void );
extern void BusFault_Handler( void );
extern void UsageFault_Handler( void );
extern void SVC_Handler( void );
extern void DebugMon_Handler( void );
extern void PendSV_Handler( void );
extern void DEFAULT_Handler(void);

/*
extern void SysTick_Handler( void );
extern void WULINS_Handler(void);
extern void WUGPIO_Handler(void);
extern void WUTIMER_Handler(void);
extern void BOR_Handler(void);
extern void WatchdogA_Handler(void);
extern void UV_Handler(void);
extern void OV_Handler(void);
extern void LINS_Handler(void);
extern void ADC_Handler(void);
extern void PWM_Handler(void);
extern void LINM_Handler(void);
extern void GPIO_Handler(void);
extern void WULINM_Handler(void);
extern void OVTemp_Handler(void);
extern void Timer0_Handler(void);
extern void Timer1_Handler(void);
extern void Timer2_Handler(void);
extern void Watchdog_Handler(void);
*/

/*
typedef void( *intfunc )( void );
typedef union { intfunc __fun; void * __ptr; } intvec_elem;
*/
typedef void( *intfunc_t )( void );

typedef struct{
    void * __ptr;
    intfunc_t _iar_program_start;
    intfunc_t _NMI_Handler;
    intfunc_t _HardFault_Handler;
    intfunc_t _MemManage_Handler;
    intfunc_t _BusFault_Handler;
    intfunc_t _UsageFault_Handler;
    intfunc_t _rsv0;
    intfunc_t _rsv1;
    intfunc_t _rsv2;
    intfunc_t _rsv3;
    intfunc_t _SVC_Handler;
    intfunc_t _DebugMon_Handler;
    intfunc_t _rsv4;
    intfunc_t _PendSV_Handler;
    intfunc_t _SysTick_Handler;
    intfunc_t _WULINS_Handler;
    intfunc_t _WUGPIO_Handler;
    intfunc_t _WUTIMER_Handler;
    intfunc_t _BOR_Handler;
    intfunc_t _WatchdogA_Handler;
    intfunc_t _UV_Handler;
    intfunc_t _OV_Handler;
    intfunc_t _LINS_Handler;
    intfunc_t _ADC_Handler;
    intfunc_t _PWM_Handler;
    intfunc_t _LINM_Handler;
    intfunc_t _GPIO_Handler;
    intfunc_t _WULINM_Handler;
    intfunc_t _OVTemp_Handler;
    intfunc_t _DEFAULT_Handler;
    intfunc_t _Lullaby_Handler;
    intfunc_t _Timer0_Handler;
    intfunc_t _Timer1_Handler;
    intfunc_t _Timer2_Handler;
    intfunc_t _Watchdog_Handler;
  
}intvecTable_t;

/*
 The vector table is normally located at address 0.
 When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
 If you need to define interrupt service routines,
 make a copy of this file and include it in your project.
 The name "__vector_table" has special meaning for C-SPY:
 it is where the SP start value is found, and the NVIC vector
 table register (VTOR) is initialized to this address if != 0.
*/

#pragma location = ".intvec"
static const intvecTable_t __vector_table =
{
  .__ptr = __sfe( "CSTACK" ),
  ._iar_program_start = __iar_program_start,
  ._NMI_Handler  = NMI_Handler,
  ._HardFault_Handler = HardFault_Handler,
  ._MemManage_Handler = MemManage_Handler,
  ._BusFault_Handler = BusFault_Handler,
  ._UsageFault_Handler = UsageFault_Handler,
  ._rsv0 = 0,
  ._rsv1 = 0,
  ._rsv2 = 0,
  ._rsv3 = 0,
  ._SVC_Handler = SVC_Handler,
  ._DebugMon_Handler = DebugMon_Handler,
  ._rsv4 = 0,
  ._PendSV_Handler = PendSV_Handler,
  ._SysTick_Handler = SysTick_Handler,
  ._WULINS_Handler = WULINS_Handler,
  ._WUGPIO_Handler = WUGPIO_Handler,
  ._WUTIMER_Handler = WUTIMER_Handler,
  ._BOR_Handler = BOR_Handler,
  ._WatchdogA_Handler = WatchdogA_Handler,
  ._UV_Handler = UV_Handler,
  ._OV_Handler = OV_Handler,
  ._LINS_Handler = LINS_Handler,
  ._ADC_Handler = ADC_Handler,
  ._PWM_Handler = PWM_Handler,
  ._LINM_Handler = LINM_Handler,
  ._GPIO_Handler = GPIO_Handler,
  ._WULINM_Handler = WULINM_Handler,
  ._OVTemp_Handler = OVTemp_Handler,
  ._DEFAULT_Handler = DEFAULT_Handler,
  ._Lullaby_Handler = Lullaby_Handler,
  ._Timer0_Handler = Timer0_Handler,			/* IRQ 10h Timer0 */
  ._Timer1_Handler = Timer1_Handler,			/* IRQ 11h Timer1 */
  ._Timer2_Handler = Timer2_Handler,			/* IRQ 12h Timer2 */
  ._Watchdog_Handler = Watchdog_Handler			/* IRQ 13h WDT */
};

__weak void NMI_Handler( void )         { for(;;){} }
__weak void HardFault_Handler( void )   { for(;;){} }
__weak void MemManage_Handler( void )   { for(;;){} }
__weak void BusFault_Handler( void )    { for(;;){} }
__weak void UsageFault_Handler( void )  { for(;;){} }
__weak void SVC_Handler( void )         { for(;;){} }
__weak void DebugMon_Handler( void )    { for(;;){} }
__weak void PendSV_Handler( void )      { for(;;){} }

/*__weak extern void Lullaby_Handler(void) {}*/
__weak extern void DEFAULT_Handler(void) {}

/*__weak extern void SysTick_Handler(void) {}
__weak extern void BOR_Handler(void) {}
__weak extern void WatchdogA_Handler(void) {}
__weak extern void UV_Handler(void) {}
__weak extern void OV_Handler(void) {}
__weak extern void LINS_Handler(void) {}
__weak extern void ADC_Handler(void) {}
__weak extern void PWM_Handler(void) {}
__weak extern void LINM_Handler(void) {}
__weak extern void GPIO_Handler(void) {}
__weak extern void OVTemp_Handler(void) {}
__weak extern void Timer0_Handler(void) {}
__weak extern void Timer1_Handler(void) {}
__weak extern void Timer2_Handler(void) {}
__weak extern void Watchdog_Handler(void) {}
__weak extern void WULINS_Handler(void)  {}
__weak extern void WUGPIO_Handler(void)  {}
__weak extern void WUTIMER_Handler(void) {}
__weak extern void WULINM_Handler(void)  {}
*/

void __cmain( void );
__weak void __iar_init_core( void );
__weak void __iar_init_vfp( void );

#pragma required=__vector_table
void __iar_program_start( void )
{
    __iar_init_core();
    __iar_init_vfp();
    __cmain();
}




