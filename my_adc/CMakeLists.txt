cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "/Users/sathishkumar/Documents/2_PICO/pico-sdk")

include(pico_sdk_import.cmake)

project(my_blink C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
add_executable(main
  main.c
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
pico_add_extra_outputs(main)

target_link_libraries(main pico_stdlib hardware_adc pico_multicore)