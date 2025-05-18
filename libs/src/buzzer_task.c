#include "libs/include/buzzer_task.h"
#include "libs/include/definicoes.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"

void vBuzzerTask(void *params)
{
    JoystickData joystickData;
    uint16_t volume_agua = 0;
    uint16_t volume_chuva = 0;


    gpio_set_function(BUZZER_A, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_A);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (5000 * 4096));  // Frequência do PWM
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(BUZZER_A, 0);  // Inicializa com o PWM desligado

    while(true)
    {
        if(xQueueReceive(xJoystickQueue, &joystickData, portMAX_DELAY) == pdTRUE)
        {
            volume_agua = (joystickData.x_value * 100) / 4095; // Normaliza para 0-100
            volume_chuva = (joystickData.y_value * 100) / 4095; // Normaliza para 0-100
            
            if(volume_agua >= 70 || volume_chuva >= 80)
            {
                for (int i = 0; i < 3; i++)  // 3 bipes
                {
                    // Tom 1
                    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (1000 * 4096)); // 1 kHz
                    pwm_init(slice_num, &config, true);
                    pwm_set_gpio_level(BUZZER_A, 3072); 
                    vTaskDelay(pdMS_TO_TICKS(150));

                    // Silêncio
                    pwm_set_gpio_level(BUZZER_A, 0);
                    vTaskDelay(pdMS_TO_TICKS(100));

                    // Tom 2
                    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (1500 * 4096)); // 1.5 kHz
                    pwm_init(slice_num, &config, true);
                    pwm_set_gpio_level(BUZZER_A, 3072); 
                    vTaskDelay(pdMS_TO_TICKS(150));

                    // Silêncio
                    pwm_set_gpio_level(BUZZER_A, 0);
                    vTaskDelay(pdMS_TO_TICKS(100));
                }
            }
            else
            {
                pwm_set_gpio_level(BUZZER_A, 0); // Desliga o PWM se o valor for zero
            }
        }
    }
}