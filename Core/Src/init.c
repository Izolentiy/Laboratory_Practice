#include "init.h"

void GPIO_Ini() {
    SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN | RCC_AHBENR_GPIOAEN);

    // настройка пинов светодиодов PE 8-15
    SET_BIT(GPIOE->MODER, 0x5555UL << 16);
    SET_BIT(GPIOE->OSPEEDR, 0xFFFFUL << 16);
}

void HAL_RCC_Ini() {}

void RCC_Ini() {
    // очистка регистров, битов
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80UL);
    CLEAR_REG(RCC->CFGR);
    while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET);
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET);
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET);
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
    
    // включаем внешний источник таковой частоты
    // на плате DiscoveryKit HSE OSC имеет частоту 8 МГц
    // включаем CSS для отслеживания нарушений работы в HSE
    SET_BIT(RCC->CR, RCC_CR_HSEON);
    while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET);
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
    SET_BIT(RCC->CR, RCC_CR_CSSON);

    // настройка блока PLL
    // устанавливаем HSE как источник для PLL
    // устанавливаем входной предделитель 2
    // устанавливаем множитель PLLMUL = 16
    // включаем PLL блок
    SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC_HSE_PREDIV);
    MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PREDIV, RCC_CFGR2_PREDIV_DIV2);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLMUL, RCC_CFGR_PLLMUL16);
    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY));

    // устанавливаем PLL для системного тактирования
    // AHB предделитель = 1
    // APB1 предделитель = 4
    // APB2 предделитель = 2
    // настройка задержки внутренней памяти
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2);
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2); // !
}

void ITR_Ini() {
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
    
    MODIFY_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI0, SYSCFG_EXTICR1_EXTI0_PA);
    SET_BIT(EXTI->IMR, EXTI_IMR_MR0);
    SET_BIT(EXTI->RTSR, EXTI_IMR_MR0);
    SET_BIT(EXTI->FTSR, EXTI_IMR_MR0);
    NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void SysTick_Ini() {
    // выключаем счетчик
    // разрешаем прерываение по стистемному таймеру
    // тактирование из шины AHB без деления 64 МГц
    // значение с которого начинается счёт, эквивалентное 1 кГц
    // очистка поля и включение счётчика, только после настройки всего
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
    MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, 63999 << SysTick_LOAD_RELOAD_Pos);
    MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk, 63999 << SysTick_VAL_CURRENT_Pos);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}