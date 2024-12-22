#include "it_handlers.h"

void SysTick_Handler(void) {
    SysTick_Interrupt();
}

void EXTI0_IRQHandler(void) {
    SET_BIT(EXTI->PR, EXTI_PR_PR0);
    Button_Interrupt();
}
