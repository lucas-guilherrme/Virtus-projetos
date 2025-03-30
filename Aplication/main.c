
#include "inc/ssd_1306.h"
#include "inc/ssd_1306_conf.h"
#include "inc/ssd_1306_font.h"
#include "inc/_button.h"
#include "inc/joystick.h"
#include "inc/jogo_da_velha.h"

#define MENU_OPTIONS 3

const char *game_names[MENU_OPTIONS] = {
    "Jogo da Velha",
    "Snake",
    "Sequencia Correta"
};

int show_menu(JoystickState *js, const char *game_names[]);

int main(){

    stdio_init_all();
    ssd_1306_init();
    button_init();

    JoystickState js = {0};
    Joystick_Init(&js);

    ssd_1306_fill(black);
    ssd_1306_draw_circle(40, 40, 20, white);
    ssd_1306_up_date_screen();

    int game = show_menu(&js, game_names);

    switch(game) {
        case 0:{
            play_tic_tac_toe();
        }break;
        case 2:{
        }break;
    }
    
    return 0;
}

int show_menu(JoystickState *js, const char *game_names[]) {
    int selected_option = 0;
    bool button_pressed = false;
    while (1) {
        // Limpa a tela
        ssd_1306_fill(black);

        // Exibe as opções do menu
        for (int i = 0; i < MENU_OPTIONS; i++) {
            if (i == selected_option) {
                // Destaca a opção selecionada
                ssd_1306_write_string("> ", Font_6x8, white);
            } else {
                ssd_1306_write_string("  ", Font_6x8, white);
            }
            ssd_1306_write_string(game_names[i], Font_6x8, white);
            ssd_1306_set_cursor(0, (i + 1) * 10); // Move para a próxima linha
        }

        // Atualiza a tela
        ssd_1306_up_date_screen();

        // Lê os valores do joystick
        Joystick_Read(js);
        Joystick_ApplyFilters(js);

        // Navega pelo menu
        if (js->y_filtered < (AVERAGE_VALUE - DEADZONE_THRESHOLD)) {
            // Move para cima
            if (selected_option > 0) {
                selected_option--;
                sleep_ms(200); // Delay para evitar múltiplos movimentos rápidos
            }
        } else if (js->y_filtered > (AVERAGE_VALUE + DEADZONE_THRESHOLD)) {
            // Move para baixo
            if (selected_option < MENU_OPTIONS - 1) {
                selected_option++;
                sleep_ms(200); // Delay para evitar múltiplos movimentos rápidos
            }
        }

        // Verifica se o botão foi pressionado
        button_pressed = _joystick_read_button();
        if (button_pressed) {
            // Retorna a opção selecionada
            return selected_option;
        }
    }
}