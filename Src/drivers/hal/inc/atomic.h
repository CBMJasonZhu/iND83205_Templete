#ifndef _ATOMIC_H
#define _ATOMIC_H

#include <cmsis_iccarm.h>

typedef  uint32_t atomic_t;
/******************************************************************************
 Disables global interrupt. Enter atomic section.
******************************************************************************/
void halStartAtomic(atomic_t volatile *pAtomic);
/******************************************************************************
 Exit atomic section
******************************************************************************/
void halEndAtomic(atomic_t volatile *pAtomic);
/** \brief Marks the begin of atomic section */
#define __atomic_enter()  __disable_irq();
/** \brief Marks the end of atomic section */
#define __atomic_exit()   __enable_irq();

#endif 

