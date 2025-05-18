#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "libs/include/definicoes.h"
#include "libs/include/joystick_task.h"
#include "libs/include/ledsrgb_task.h"
#include "libs/include/display_task.h"
#include "libs/include/buzzer_task.h"
#include "libs/include/matriz_task.h"

QueueHandle_t xJoystickQueue;


// Modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}

int main()
{

    // Ativa BOOTSEL via botão
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    stdio_init_all();
    sleep_ms(1000); // Aguarda 1 segundo para inicializar o USB
    xJoystickQueue = xQueueCreate(10, sizeof(JoystickData));

    xTaskCreate(vJoystickTask, "Joystick Task", 256, NULL,1, NULL);
    xTaskCreate(vLedsRGBTask, "LEDsRGB Task", 256, NULL, 2, NULL);
    xTaskCreate(vDisplayTask, "Display Task", 256, NULL, 2, NULL);
    xTaskCreate(vBuzzerTask, "Buzzer Task", 256, NULL, 2, NULL);
    xTaskCreate(vMatrizTask, "Matriz Task", 512, NULL, 2, NULL);
    vTaskStartScheduler();
    panic_unsupported();
}
