/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/gpio.h"
 #include "hardware/adc.h"
 #include "pico/multicore.h"
 #include "pico/bootrom.h"
 
 #define LED_PIN 25

void blink_led() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
    }
}

 int main() {
     stdio_init_all();
     
    // Launch LED blinking on core 1
    multicore_launch_core1(blink_led);
     
     printf("ADC Example, measuring GPIO26\n");
     adc_init();
 
     // Make sure GPIO is high-impedance, no pullups etc
     adc_gpio_init(26);
     // Select ADC input 0 (GPIO26)
     adc_select_input(0);
 
     // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
     const float conversion_factor = 3.3f / (1 << 12);
     
    while(1) {
         uint16_t result = adc_read();
         printf("Raw value: 0x%03x, Raw ADC value: %d, voltage: %f V\n", result, result, result * conversion_factor);
         sleep_ms(500);
     }

     reset_usb_boot(0, 0);
 }