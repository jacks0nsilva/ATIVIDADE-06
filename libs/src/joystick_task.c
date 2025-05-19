#include "libs/include/joystick_task.h"
#include "libs/include/definicoes.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"


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