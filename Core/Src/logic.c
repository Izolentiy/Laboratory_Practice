#include "logic.h"

int last_turned_led = -1; // индекс диода для включения
int controlled_led = 0;  // индекс управляемого диода
unsigned int *led_tick[6];
uint16_t *led_period[6];
uint8_t index_in_range = 0;  // индекс текущего диапазона частот
uint8_t index_of_range = 0;  // индекс частоты в диапазоне
uint8_t flag = 0;
unsigned int led_tick_map[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};
uint16_t led_period_map[3][3] = {
    {3333, 1000, 714},
    {2500,  833, 588},
    {2000,  625, 526}
};

void B1_short_press() {
    if (++index_of_range == 3) index_of_range = 0;
    *led_period[controlled_led] = led_period_map[index_in_range][index_of_range];
    *led_tick[controlled_led] = led_tick_map[index_in_range][index_of_range];
}
void B2_short_press() {
    if (++index_in_range == 3) index_in_range = 0;
    *led_period[controlled_led] = led_period_map[index_in_range][index_of_range];
    *led_tick[controlled_led] = led_tick_map[index_in_range][index_of_range];
}

void B1_long_press() {
    if (++last_turned_led == 6) {
        last_turned_led = -1;
        CLEAR_BIT(GPIOC->ODR, 0b111111 << 7);
    } else if (last_turned_led >= 0) {
        SET_BIT(GPIOC->ODR, 1 << (7 + last_turned_led));
    }
}
void B2_long_press() {
    if (++controlled_led == 6) controlled_led = 0;
}

void init_periods_and_ticks() {
    for (int i = 0; i < 6; ++i) {
        led_tick[i] = &(led_tick_map[0][0]);
        led_period[i] = &(led_period_map[0][0]);
    }
}

void manage_leds() {
    for (int k = 0; k <= last_turned_led; ++k) {
        if (k <= last_turned_led && k >= 0) {
            uint32_t period = *led_period[k];
            if (*led_tick[k] >= period / 2) {
                flag = 1;
                SET_BIT(GPIOC->ODR, 1 << (7 + k));
                if (*led_tick[k] >= period) *led_tick[k] = 0;
            } else {
                flag = 0;
                CLEAR_BIT(GPIOC->ODR, 1 << (7 + k));
            }
        }
    }
}
