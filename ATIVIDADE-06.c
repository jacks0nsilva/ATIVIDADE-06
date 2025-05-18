#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "libs/definicoes.h"
#include "libs/joystick_task.h"
#include "libs/ledsrgb_task.h"

QueueHandle_t xJoystickQueue;

/*
void vJoystickTask(void *params)
{
    JoystickData joystickData;
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);
    adc_init();

    while (true)
    {
        adc_select_input(0); // GPIO 26 - ADC 0
        joystickData.y_value = adc_read();

        adc_select_input(1); // GPIO 27 - ADC 1
        joystickData.x_value = adc_read();

        xQueueSend(xJoystickQueue, &joystickData, 0);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay de 100ms
    }
}

void vReadingTask(void *params)
{
   JoystickData joydata;
   while (true)
   {
       if (xQueueReceive(xJoystickQueue, &joydata, portMAX_DELAY) == pdTRUE)
       {
           printf("X: %d, Y: %d\n", joydata.x_value, joydata.y_value);
       }
       vTaskDelay(pdMS_TO_TICKS(100)); // Atualiza a cada 100ms
   }
}
*/


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
    sleep_ms(2000); // Aguarda 2 segundos para inicializar o USB
    xJoystickQueue = xQueueCreate(10, sizeof(JoystickData));

    xTaskCreate(vJoystickTask, "Joystick Task", 256, NULL,1, NULL);
    xTaskCreate(vLedsRGBTask, "LEDsRGB Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();
    panic_unsupported();
}
