cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico-control C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

if (TARGET tinyusb_device)
        add_executable(pico-control
                
        )

        target_sources(pico-control PUBLIC
                ${CMAKE_CURRENT_LIST_DIR}/main.c
                ${CMAKE_CURRENT_LIST_DIR}/pwm.c
                ${CMAKE_CURRENT_LIST_DIR}/adc.c
                ${CMAKE_CURRENT_LIST_DIR}/uart.c
        )

        target_include_directories(pico-control PUBLIC
                ${CMAKE_CURRENT_LIST_DIR}
        )       
        # pull in common dependencies and additional pwm hardware support
        target_link_libraries(pico-control pico_stdlib hardware_pwm hardware_gpio hardware_adc)
        # create map/bin/hex file etc.
        pico_add_extra_outputs(pico-control)
        pico_enable_stdio_usb(pico-control 1)
        pico_enable_stdio_uart(pico-control 0) # prevent printf from recursive loading
elseif(PICO_ON_DEVICE)
        message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()