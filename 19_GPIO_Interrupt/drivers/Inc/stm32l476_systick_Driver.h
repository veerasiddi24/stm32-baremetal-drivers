/*
 * stm32l476_systick_Driver.h
 *
 *  Created on: May 11, 2026
 *      Author: lenovo
 */

#ifndef INC_STM32L476_SYSTICK_DRIVER_H_
#define INC_STM32L476_SYSTICK_DRIVER_H_



#include <stdint.h>

typedef struct
{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;

}SysTick_TypeDef;

#define SYSTICK_BASEADDR  0xE000E010

#define SYSTICK ((SysTick_TypeDef*)SYSTICK_BASEADDR)


#endif /* INC_STM32L476_SYSTICK_DRIVER_H_ */
