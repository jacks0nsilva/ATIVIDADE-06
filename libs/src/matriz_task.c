#include "libs/include/definicoes.h"
#include "libs/include/matriz_task.h"
#include "libs/include/pio_config.h"
#include <stdlib.h>

void vrx_leds(uint16_t vrx_value)
{
    
    if(vrx_value < 20)
    {
        int disabled_leds[5] = {4, 5, 14, 15, 24}; // Array para armazenar as cores dos LEDs
        npSetManyLEDs(disabled_leds, 5, 0, 0, 0); // Apaga os leds
    } else if (vrx_value >= 20 && vrx_value < 40) 
    {
        int disabled_leds[4] = {5, 14, 15, 24}; // Array para armazenar os outros LEDs
        npSetLED(4, 0, 0, 101); // Azul
        npSetManyLEDs(disabled_leds, 4, 0, 0, 0); // Apaga os outros LEDs
    } else if (vrx_value >= 40 && vrx_value < 60)
    {
        int disabled_leds[3] = {14, 15, 24}; 
        int active_leds[2] = {4, 5}; 
        npSetManyLEDs(disabled_leds, 3, 0, 0, 0); // Apaga os outros LEDs
        npSetManyLEDs(active_leds, 2, 0, 0, 101); 
    } else if (vrx_value >= 60 && vrx_value < 80){
        int disabled_leds[2] = {15, 24};
        int active_leds[3] = {4, 5, 14};
        npSetManyLEDs(disabled_leds, 2, 0, 0, 0); // Apaga os outros LEDs
        npSetManyLEDs(active_leds, 3, 0, 0, 101); // Azul
    } else if(vrx_value >= 80 && vrx_value < 98){
        int disabled_leds[1] = {24};
        int active_leds[4] = {4, 5, 14, 15};
        npSetManyLEDs(disabled_leds, 1, 0, 0, 0); // Apaga os outros LEDs
        npSetManyLEDs(active_leds, 4, 0, 0, 101); // Azul
    } else if (vrx_value >= 99){
        int active_leds[5] = {4, 5, 14, 15, 24};
        npSetManyLEDs(active_leds, 5, 0, 0, 101); // Azul
    }
}

void vry_leds(uint16_t vry_value)
{
    
    if(vry_value < 20)
    {
        int disabled_leds[5] = {0,9,10,19,20}; // Array para armazenar as cores dos LEDs
        npSetManyLEDs(disabled_leds, 5, 0, 0, 0); // Apaga os leds
    } else if (vry_value >= 20 && vry_value < 40) 
    {
        int disabled_leds[4] = {9,10,19,20}; // Array para armazenar os outros LEDs
        npSetLED(0, 0, 0, 101); // Azul
        npSetManyLEDs(disabled_leds, 4, 0, 0, 0); // Apaga os outros LEDs
    } else if (vry_value >= 40 && vry_value < 60)
    {
        int disabled_leds[3] = {10,19,20}; 
        int active_leds[2] = {0,9}; 
        npSetManyLEDs(disabled_leds, 3, 0, 0, 0); // Apaga os outros LEDs
        npSetManyLEDs(active_leds, 2, 0, 0, 101); 
    } else if (vry_value >= 60 && vry_value < 80){
        int disabled_leds[2] = {19,20};
        int active_leds[3] = {0,9,10};
        npSetManyLEDs(disabled_leds, 2, 0, 0, 0); // Apaga os outros LEDs
        npSetManyLEDs(active_leds, 3, 0, 0, 101); // Azul
    } else if(vry_value >= 80 && vry_value < 98){
        int disabled_leds[1] = {20};
        int active_leds[4] = {0,9,10,19};
        npSetManyLEDs(disabled_leds, 1, 0, 0, 0); // Apaga os outros LEDs
        npSetManyLEDs(active_leds, 4, 0, 0, 101); // Azul
    } else if (vry_value >= 99){
        int active_leds[5] = {0,9,10,19,20};
        npSetManyLEDs(active_leds, 5, 0, 0, 101); // Azul
    }
}


void vMatrizTask(void *params){
    JoystickData joystickData;
    
    uint16_t volume_agua = 0;
    uint16_t volume_chuva = 0;

    npInit(MATRIZ_LEDS); // Inicializa a matriz de LEDs
    npClear(); // Limpa a matriz de LEDs
    npWrite(); // Envia os dados para a matriz de LEDs

    while (true){
        if(xQueueReceive(xJoystickQueue, &joystickData, portMAX_DELAY) == pdTRUE){
            volume_chuva = (abs(joystickData.y_value - 2048) * 100) / 2048; // Normaliza para 0-100
            volume_agua = (abs(joystickData.x_value - 2048) * 100) / 2048; // Normaliza para 0-100
            vrx_leds(volume_agua);
            vry_leds(volume_chuva);

            npWrite(); // Envia os dados para a matriz de LEDs  
            vTaskDelay(pdMS_TO_TICKS(100)); 

            if(volume_agua >= 70 || volume_chuva >= 80)
            {
                int active_leds[9]= {6,7,8,11,12,13,16,17,18};
                npSetManyLEDs(active_leds, 9, 101, 0, 0); // Vermelho
                npWrite(); // Envia os dados para a matriz de LEDs
                vTaskDelay(pdMS_TO_TICKS(200));
                npSetManyLEDs(active_leds, 9, 0, 0, 0); // Apaga os LEDs
                npWrite(); // Envia os dados para a matriz de LEDs
                vTaskDelay(pdMS_TO_TICKS(200));
            }
        }
    }

}
