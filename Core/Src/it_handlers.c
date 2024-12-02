#include "it_handlers.h"
#include "logic.h"

#define BTN_DELAY_FILTER 75
#define LONG_PRESS_DURATION 2000

extern unsigned int *led_tick[6];
uint32_t B1_tick_count = 0, B2_tick_count = 0;
uint8_t B1_pressed = 0, B2_pressed = 0;

// Button 1
void EXTI0_IRQHandler(void) {
    SET_BIT(EXTI->PR, EXTI_PR_PR0);
    B1_pressed = READ_BIT(GPIOC->IDR, GPIO_IDR_ID0);
    if (!B1_pressed && B1_tick_count >= BTN_DELAY_FILTER) {
        // кнопку отпустили
        if (B1_tick_count >= LONG_PRESS_DURATION) B1_long_press();
        else B1_short_press();
    } else if (B1_pressed) {
        B1_tick_count = 0; // запускаем счётчик
    }
}
// Button 2
void EXTI3_IRQHandler(void) {
    SET_BIT(EXTI->PR, EXTI_PR_PR3);
    B2_pressed = READ_BIT(GPIOC->IDR, GPIO_IDR_ID3);
    if (!B2_pressed && B2_tick_count >= BTN_DELAY_FILTER) {
        // кнопку отпустили
        if (B2_tick_count >= LONG_PRESS_DURATION) B2_long_press();
        else B2_short_press();
    } else if (B2_pressed) {
        B2_tick_count = 0;
    }
}

// одновременное мигание для диодов с одинаковой частотой.
// обработка дребезга, доработать.
// вынести логику в отдельный файл.

void SysTick_Handler(void) {
    ++B1_tick_count;
    ++B2_tick_count;
    
    (*(led_tick[0]))++;
    (*(led_tick[1]))++;
    (*(led_tick[2]))++;
    (*(led_tick[3]))++;
    (*(led_tick[4]))++;
    (*(led_tick[5]))++;
}