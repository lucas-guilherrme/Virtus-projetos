#ifndef JOGO_DA_VELHA_H
#define JOGO_DA_VELHA_H

/********************************|includes|*****************************************/
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#include "../Libs/display_oled/inc/ssd_1306.h"
#include "../Libs/display_oled/inc/ssd_1306_font.h"
#include "../Libs/joystick/inc/joystick.h"

/********************************|defines|******************************************/
// Definições da tela e do tabuleiro
#define SCREEN_WIDTH 127
#define SCREEN_HEIGHT 63

// Posições para desenhar o grid do jogo (valores experimentais)
#define VERT_LINE1 42
#define VERT_LINE2 85
#define HORZ_LINE1 22
#define HORZ_LINE2 45

// Cores (supondo que white e black estejam definidos na biblioteca)
#define white 1
#define black 0

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
/********************************|fuctions|*****************************************/

bool read_button_A(void);  
bool read_button_B(void);  

// Protótipo para leitura dos eixos do joystick
int get_joystick_direction(void);
void delay_ms(int ms);

void draw_menu(void);
int menu_selection(void);
void clear_board(void);
void draw_board(void);
void get_cell_center(int row, int col, int *center_x, int *center_y);
void drawX(int cell_row, int cell_col);
void drawO(int cell_row, int cell_col);
void draw_moves(void);
void update_board_screen(int current_player);
void wait_for_start(void);
int check_win(void);
int board_full(void);

#endif /*JOGO_DA_VELHA_H*/