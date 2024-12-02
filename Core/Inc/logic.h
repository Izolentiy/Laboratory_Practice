#ifndef LOGIC_H
#define LOGIC_H

#include <stdint.h>
#include "..\..\CMSIS\Devices\stm32f411xe.h"
#include "..\..\CMSIS\Devices\stm32f4xx.h"
#include "..\..\CMSIS\Devices\system_stm32f4xx.h"

void B1_short_press();
void B2_short_press();
void B1_long_press();
void B2_long_press();
void init_periods_and_ticks();
void manage_leds();

#endif