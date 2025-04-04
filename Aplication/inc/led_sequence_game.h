#ifndef APLICATION_H
#define APLICATION_H

/**************************| include |*******************************/
#include "config.h"
/**************************| Define |********************************/

#define MAX_X_LED_MATRIX     4
#define MAX_Y_LED_MATRIX     4
#define MIN_X_LED_MATRIX     0
#define MIN_Y_LED_MATRIX     0

/**************************| Funcion |*******************************/

void led_matrix_moving(int x, int y, led_intensity_t red, led_intensity_t green, led_intensity_t blue);
void led_matrix_nuber(int numero);

void play_led_sequence_game(JoystickState *js);

#endif ///APLICATION_H