#include "libs/include/display_task.h"
#include "libs/include/ssd1306.h"
#include "hardware/i2c.h"
#include "libs/include/definicoes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void vDisplayTask(void *params)
{
    JoystickData joystickData;
    ssd1306_t ssd;

    uint16_t volume_agua = 0;
    uint16_t volume_chuva = 0;
    char volume_agua_str[6];
    char volume_chuva_str[6];

    // Inicialização da comunicação I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização do display e configuração
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADRESS, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
   
    ssd1306_send_data(&ssd);

    while(true)
    {
        if(xQueueReceive(xJoystickQueue, &joystickData, portMAX_DELAY)== pdTRUE)
        {
            volume_chuva = (abs(joystickData.y_value - 2048) * 100) / 2048; // Normaliza para 0-100
            volume_agua = (abs(joystickData.x_value - 2048) * 100) / 2048; // Normaliza para 0-100

            ssd1306_fill(&ssd, false);
            ssd1306_rect(&ssd,0,0, WIDTH, HEIGHT, true, false); // Desenha o contorno do display

            ssd1306_draw_string(&ssd, "VIGILANCIA", 24, 3); // Desenha o título
            ssd1306_draw_string(&ssd, "DE", 56, 12); // Desenha o subtítulo
            ssd1306_draw_string(&ssd, "CHEIA", 44, 20); // Desenha o subtítulo

            ssd1306_hline(&ssd, 1, 126, 28, true); // Desenha a linha horizontal

            // Desenha o valor do eixo X (nível de água)
            ssd1306_draw_string(&ssd,"V da Chuva:", 3, 34); 
            sprintf(volume_chuva_str, "%d" "%s", volume_chuva, "%");
            ssd1306_draw_string(&ssd, volume_chuva_str, 97, 34); 

            ssd1306_hline(&ssd, 1, 126, 46, true); // Desenha a linha horizontal

            // Desenha o valor do eixo Y (nível de volume da chuva)
            ssd1306_draw_string(&ssd, "V da Agua:", 3, 52);
            sprintf(volume_agua_str, "%d" "%s", volume_agua, "%");
            ssd1306_draw_string(&ssd, volume_agua_str, 89, 52);

            ssd1306_send_data(&ssd);
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Atualiza a cada 100ms
    }
}