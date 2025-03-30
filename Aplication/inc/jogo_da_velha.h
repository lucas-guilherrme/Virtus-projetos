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

#include "../Aplication/inc/ssd_1306.h"
#include "../Aplication/inc/ssd_1306_font.h"
#include "../Aplication/inc/joystick.h"

#include "config.h"

/********************************|fuctions|*****************************************/
// Protótipo para leitura dos eixos do joystick
int get_joystick_direction(JoystickState *js);
void delay_ms(int ms);

void draw_menu(void);
int menu_selection(void);
void clear_board(void);
void draw_board(void);
void get_cell_center(int row, int col, int *center_x, int *center_y);
void draw_x(int cell_row, int cell_col);
void draw_o(int cell_row, int cell_col);
void draw_moves(void);
void update_board_screen(int current_player);
void wait_for_start(void);
int check_win(void);
int board_full(void);

void play_tic_tac_toe(void);
bool ask_restart(void);
void draw_winning_line(int start_row, int start_col, int end_row, int end_col);
int check_win_and_draw_line(void);
#endif /*JOGO_DA_VELHA_H*/