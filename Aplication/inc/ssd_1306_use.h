#ifndef SSD_1306_USE_H
#define SSD_1306_USE_H

/**********************************************|includes|******************************/
#include "config.h"

void oled_show_menu(JoystickState *js, int *selected_game);
void main_menu(JoystickState *js);
void JogoDaVelha_Start(JoystickState *js);


#endif /*SSD_1306_USE_H*/