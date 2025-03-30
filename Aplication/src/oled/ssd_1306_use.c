/*******************************| includes |***************************/
#include "../Aplication/inc/ssd_1306_use.h"

/***********************************|funtction |**********************/

void oled_show_menu(JoystickState *js, int *selected_game) {
    static int current_page = 0;    // Página atual do menu (0 ou 1)
    static int cursor_position = 0; // Posição do cursor (0 ou 1)

    while (true) {
        // Limpa a tela do OLED
        ssd_1306_fill(black);

        // Página 0
        if (current_page == 0) {
            ssd_1306_set_cursor(10, 10);
            ssd_1306_write_string("1. Jogo da Velha", Font_6x8, white);
            ssd_1306_set_cursor(10, 30);
            ssd_1306_write_string("2. Jogo Sequencia", Font_6x8, white);
        }

        // Página 1 (sensação de continuidade)
        if (current_page == 1) {
            ssd_1306_write_string("3. Configuracoes", Font_6x8, white);
            ssd_1306_write_string("4. Sobre", Font_6x8, white);
        }

        // Atualiza o display
        ssd_1306_up_date_screen();

        // Lê o estado do joystick
        Joystick_Read(js);
        js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);
        js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);

        // Movimenta o cursor verticalmente
        if (js->y_filtered > ADC_UPPER_THRESHOLD_Y && cursor_position < 1) {
            cursor_position++;
        } else if (js->y_filtered < ADC_LOWER_THRESHOLD_Y && cursor_position > 0) {
            cursor_position--;
        }

        // Troca de página horizontalmente
        if (js->x_filtered > ADC_UPPER_THRESHOLD_X) {
            current_page = (current_page + 1) % 2; // Alterna entre página 0 e 1
        } else if (js->x_filtered < ADC_LOWER_THRESHOLD_X) {
            current_page = (current_page - 1 + 2) % 2; // Alterna entre página 1 e 0
        }

        // Verifica se o botão do joystick foi pressionado
        if (_joystick_read_button()) {
            if (current_page == 0) {
                *selected_game = cursor_position + 1; // Escolhe entre Jogo 1 ou 2
            } else if (current_page == 1) {
                *selected_game = cursor_position + 3; // Escolhe entre Configurações ou Sobre
            }
            return; // Sai do menu
        }

        sleep_ms(100); // Pequeno atraso para suavizar o movimento
    }
}

void main_menu(JoystickState *js) {
    int selected_game = 0;

    while (true) {
        // Mostra o menu no OLED e espera pela escolha do usuário
        oled_show_menu(js, &selected_game);

        // Executa o jogo ou ação selecionada
        switch (selected_game) {
            case 1:
                // Jogo da Velha
                printf("Iniciando Jogo da Velha...\n");
                JogoDaVelha_Start(js); // Função que inicia o Jogo da Velha
                break;
            case 2:
                // Jogo da Sequência Correta
                printf("Iniciando Jogo da Sequência Correta...\n");
                //led_matrix_challenge_game(js); // Função do jogo da sequência correta
                break;
            case 3:
                // Configurações (placeholder)
                printf("Abrindo Configurações...\n");
                break;
            case 4:
                // Sobre (placeholder)
                printf("Mostrando informações sobre o jogo...\n");
                break;
            default:
                break;
        }
    }
}

void JogoDaVelha_Start(JoystickState *js) {
    // Implementação do Jogo da Velha
    printf("Jogo da Velha em desenvolvimento...\n");
    sleep_ms(2000); // Placeholder para o jogo
}

