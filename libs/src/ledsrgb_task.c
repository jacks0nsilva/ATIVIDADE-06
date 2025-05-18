#include <stdio.h>
#include "libs/include/ledsrgb_task.h"
#include "libs/include/definicoes.h"
#include "pico/stdlib.h"
#include <stdlib.h>

void vLedsRGBTask(void *params){
    JoystickData joystickData;

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    uint16_t volume_agua = 0;
    uint16_t volume_chuva = 0;

    while(true)
    {
        if(xQueueReceive(xJoystickQueue, &joystickData, portMAX_DELAY)== pdTRUE)
        {
            volume_chuva = (abs(joystickData.y_value - 2048) * 100) / 2048; // Normaliza para 0-100
            volume_agua = (abs(joystickData.x_value - 2048) * 100) / 2048; // Normaliza para 0-100
           
            // Caso o volume de água ou chuva seja maior que 70 ou 80, acende o LED vermelho como forma de alerta, caso contrário, acende o LED verde
            if(volume_agua >= 70 || volume_chuva >= 80)
            {
                gpio_put(LED_GREEN, 0);
                gpio_put(LED_RED, 1);
                printf("Volume da água: %d, Volume da chuva: %d\n", volume_agua, volume_chuva);
            } else{
                gpio_put(LED_GREEN, 1);
                gpio_put(LED_RED, 0);
                printf("Volume da água: %d, Volume da chuva: %d\n", volume_agua, volume_chuva);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Atualiza a cada 100ms
    }
}