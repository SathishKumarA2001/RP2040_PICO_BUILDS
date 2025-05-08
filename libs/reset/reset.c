#include <stdio.h> 
#include "pico/stdlib.h"
#include "pico/bootrom.h"

void process_serial_input() {
    int c = getchar_timeout_us(0); // Non-blocking serial read
    if (c == 'R' || c == 'r') {
        printf("\n\nResetting Pico...\n");
        reset_usb_boot(0, 0);
    }
    else if (c == PICO_ERROR_TIMEOUT) {
        return;
    }
    else {
        putchar(c);
    }
}