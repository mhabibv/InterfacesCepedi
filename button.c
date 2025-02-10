#include "button.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "led.h"
#define BUTTON_A 5
#define BUTTON_B 6

void button_a_irq_handler(uint gpio, uint32_t events) {
    toggle_green_led();
}

void button_b_irq_handler(uint gpio, uint32_t events) {
    toggle_blue_led();
}

void init_buttons() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_a_irq_handler);
    
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_b_irq_handler);
}
