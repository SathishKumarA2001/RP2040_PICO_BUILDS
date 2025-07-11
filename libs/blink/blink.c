#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "blink.h"

#define LED_PIN 25

void blink_led() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(1000);
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);
    }
}