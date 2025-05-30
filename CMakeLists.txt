cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)
set(FREERTOS_KERNEL_PATH "C:/FreeRTOS-Kernel")
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(ATIVIDADE-06 C CXX ASM) 
pico_sdk_init()


include_directories(${CMAKE_SOURCE_DIR}/libs/include) # Adiciona o diretório 'libs' à lista global de diretórios de headers 

add_executable(${PROJECT_NAME}  
        ATIVIDADE-06.c # Código principal em C
        libs/src/joystick_task.c # Código da task do joystick
        libs/src/ledsrgb_task.c # Código da task dos leds rgb
        libs/src/display_task.c # Código da task do display
        libs/src/ssd1306.c # Código da biblioteca do display
        libs/src/buzzer_task.c # Código da task do buzzer
        libs/src/matriz_task.c # Código da matriz de leds
        )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})  # Adiciona o diretório raiz como include privado apenas para o target atual

target_link_libraries(${PROJECT_NAME} 
        pico_stdlib
        FreeRTOS-Kernel 
        FreeRTOS-Kernel-Heap4
        hardware_adc
        hardware_pwm
        hardware_i2c
        hardware_pio
        hardware_clocks
        )

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_generate_pio_header(ATIVIDADE-06 ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)
pico_add_extra_outputs(${PROJECT_NAME})