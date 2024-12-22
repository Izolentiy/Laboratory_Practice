#ifndef INIT_H
#define INIT_H

#include <stdint.h>
#include "stm32f303xc.h"
#include "stm32f3xx.h"
#include "system_stm32f3xx.h"
// #include "stm32f3xx_hal_rcc.h"

void GPIO_Ini();
// void HAL_RCC_Ini();
void RCC_Ini();
void ITR_Ini();
void SysTick_Ini();

#endif