#ifndef DEFINICOES_H
#define DEFINICOES_H

#include "FreeRTOS.h"
#include "queue.h"

#define VRX_PIN 27 // Eixo X do Joystick (nível de água)
#define VRY_PIN 26 // Eixo Y do Joystick (nível de volume da chuva)
#define BUTTON_B 6 // Botão B
#define LED_GREEN 11 // Led Verde
#define LED_RED 13 // Led Vermelho
#define I2C_PORT i2c1 // Porta I2C
#define I2C_SDA 14 // Pino SDA
#define I2C_SCL 15 // Pino SCL
#define ADRESS 0x3c // Endereço do display
#define BUZZER_A 21 // Pino do Buzzer
#define MATRIZ_LEDS 7 // Pino da Matriz de LEDs


typedef struct {
    int16_t x_value; // Eixo X do Joystick (nível de água)
    int16_t y_value; // Eixo Y do Joystick (nível de volume da chuva)
} JoystickData;

extern QueueHandle_t xJoystickQueue; //  Fila para comunicação entre tarefas

#endif