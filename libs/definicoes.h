#ifndef DEFINICOES_H
#define DEFINICOES_H

#include "FreeRTOS.h"
#include "queue.h"

#define VRX_PIN 27 // Eixo X do Joystick
#define VRY_PIN 26 // Eixo Y do Joystick
#define BUTTON_B 6 // Botão B

typedef struct {
    int16_t x_value;
    int16_t y_value;
} JoystickData;

extern QueueHandle_t xJoystickQueue;

#endif