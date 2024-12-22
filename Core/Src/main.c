#include "init.h"
#include "it_handlers.h"
#include "logic.h"

int main(void) {
    GPIO_Ini();
    RCC_Ini();
    SysTick_Ini();
    ITR_Ini();

    while (1) {}
}