#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/multicore.h"

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

int main() {
    stdio_init_all();

    // Launch LED blinking on core 1
    multicore_launch_core1(blink_led);

    adc_init();

    // Select internal temperature sensor (ADC channel 4)
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    const float conversion_factor = 3.3f / (1 << 12);

    while (1) {
        uint16_t raw = adc_read();
        float voltage = raw * conversion_factor;

        // Convert voltage to temperature in °C (from datasheet)
        float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;

        printf("Raw ADC: %d, Voltage: %.4f V, Temperature: %.2f °C\n", raw, voltage, temperature);
        sleep_ms(1000);
    }
}
