/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/adc.h"
 #include "blink.h"
 #include "reset.h"
 #include "pico/multicore.h"
 
 
 int main() {
     stdio_init_all();
     printf("\nPico Joystick Monitor - Press 'R' to reset\n");
 
     multicore_launch_core1(blink_led);
 
     adc_init();
     adc_gpio_init(26); // X-axis (ADC0)
     adc_gpio_init(27); // Y-axis (ADC1)
 
     while (1) {
         // Check for serial input
         process_serial_input();
 
         // Read ADC values
         adc_select_input(0);
         uint adc_x_raw = adc_read();
         adc_select_input(1);
         uint adc_y_raw = adc_read();
 
         // Display the joystick position
         const uint bar_width = 40;
         const uint adc_max = (1 << 12) - 1;
         uint bar_x_pos = adc_x_raw * bar_width / adc_max;
         uint bar_y_pos = adc_y_raw * bar_width / adc_max;
 
         printf("\rX: [");
 
         for (uint i = 0; i < bar_width; ++i)
             putchar(i == bar_x_pos ? 'o' : ' ');
 
         printf("]  Y: [");
 
         for (uint i = 0; i < bar_width; ++i)
             putchar(i == bar_y_pos ? 'o' : ' ');
         
         printf("]  (X: %4d, Y: %4d)  Press 'R' to reset", adc_x_raw, adc_y_raw);
 
         sleep_ms(100); // Reduced delay for more responsive serial input
     }
 }