#ifndef CONFIG_H
#define CONFIG_H

/***********************************|includes|****************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#include "hardware/pio.h"
#include "ws2818b.pio.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "math.h"
#include "ssd_1306.h"
#include "ssd_1306_font.h"
#include "joystick.h"
#include "ledMatrix.h"

/***********************************|defines|*****************/
// Botao
#define BUTTON_A 5   
#define BUTTON_B 6   
#define DEBOUNCING 100 // Tempo de debouncing em milissegundos
#define BUTTON_MODE_IT  0

// Joystick
#define ADC_PIN_X   26
#define ADC_PIN_Y   27
#define BUTTON_JOYSTICK 22
#define ADC_CHANEL_1 1
#define ADC_CHANEL_0 0

#define AVERAGE_VALUE 2048 

#define MOVING_AVG_WINDOW 5    // Size of the moving average window
#define DEADZONE_THRESHOLD 100 // Dead zone
#define LOWPASS_ALPHA 0.2      // Low-pass filter coefficient

// Matrix LED
#define WS2812                           7 
#define LED_SIZE                        25
#define DELAY                           300
#define FREQUENCIA                      800000.f  

#define ADC_UPPER_THRESHOLD_X 2900 	// Upper limit of the ADC on the X axis right
#define ADC_LOWER_THRESHOLD_X 1300  // Lower limit of the ADC on the X axis left
#define ADC_UPPER_THRESHOLD_Y 2800 	// Upper limit of the ADC on the Y axis up
#define ADC_LOWER_THRESHOLD_Y 1300 	// Lower limit of the ADC on the Y axis down


/********************************|defines|******************************************/
// Definições da tela e do tabuleiro
#define SCREEN_WIDTH 127
#define SCREEN_HEIGHT 63

// Posições para desenhar o grid do jogo (valores experimentais)
#define VERT_LINE1 42
#define VERT_LINE2 85
#define HORZ_LINE1 22
#define HORZ_LINE2 45

// Definições dos pinos dos botões para navegação do menu
#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6

// Definição do botão do joystick (para confirmação)
#define BUTTON_JOYSTICK 22

// Constantes de direção do joystick (retornadas pela função get_joystick_direction)
#define JOY_NONE   0
#define JOY_UP     1
#define JOY_DOWN   2
#define JOY_LEFT   3
#define JOY_RIGHT  4
#define JOY_SELECT 5

#define DEBAUCING 50

#endif // CONFIG_H