
        cmake_minimum_required(VERSION 3.13)

        set(PICO_SDK_PATH ./../../pico-sdk) # Set this config for your pico-sdk path

        include(pico_sdk_import.cmake)

        project(main C CXX ASM)

        set(CMAKE_C_STANDARD 11)
        set(CMAKE_CXX_STANDARD 17)

        pico_sdk_init()
        add_executable(main
            main.c
            ../libs/blink/blink.c
            ../libs/reset/reset.c
        )

        target_include_directories(main PRIVATE
            ../libs/blink              # Tell compiler where to find headers
            ../libs/reset
        )

        pico_enable_stdio_usb(main 1)
        pico_enable_stdio_uart(main 0)
        pico_add_extra_outputs(main)

        target_link_libraries(main pico_stdlib hardware_adc pico_multicore)
