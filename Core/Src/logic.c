#include "logic.h"

#define BTN_DELAY_FILTER 150

uint16_t flick_speed[3] = {128, 500, 1000};
uint8_t flick_index = 0;
uint32_t led_tick = 0;
uint32_t button_tick = 0;
uint8_t button_pressed = 0;

uint8_t counter = 0;
uint8_t state = 0b00000000;
uint8_t turn_on = 1;

void SysTick_Interrupt() {
    if (++led_tick > flick_speed[flick_index]) {
        led_tick = 0;
        if (++counter == 8) counter = 0;
        if (counter == 0) turn_on = !turn_on;
        
        if (turn_on) state |= 1 << counter;
        else state &= ~(1 << counter);

        MODIFY_REG(GPIOE->ODR, 0x0000FFFF, state << 8);
    }
    ++button_tick;
}

void Button_Interrupt() {
    button_pressed = READ_BIT(GPIOA->IDR, GPIO_IDR_0) != 0;
    if (!button_pressed && button_tick >= BTN_DELAY_FILTER) {
        if (++flick_index == 3) flick_index = 0;
    } else {
        button_tick = 0;
    }
}
