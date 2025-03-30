
/********************************|includes|*****************************************/
#include "../Aplication/inc/jogo_da_velha.h"
#include "../Aplication/inc/ssd_1306_font.h"
#include "../Aplication/inc/config.h"

char board[3][3];
int sel_row = 1, sel_col = 1;
int blink_state = 0;

int get_joystick_direction(JoystickState *js){

    Joystick_Read(js);
    Joystick_Read(js);

    Joystick_LowPassFilter(js->x_raw, js->x_filtered);
    Joystick_LowPassFilter(js->y_raw, js->y_filtered);

    if (js->x_raw < ADC_LOWER_THRESHOLD_X)
        return JOY_LEFT;
    if (js->x_raw > ADC_UPPER_THRESHOLD_X)
        return JOY_RIGHT;
    if (js->y_raw < ADC_LOWER_THRESHOLD_Y)
        return JOY_DOWN;
    if (js->y_raw > ADC_UPPER_THRESHOLD_Y)
        return JOY_UP;

    if (_read_button_A())
        return JOY_SELECT;

    return JOY_NONE;
}

// Desenha o menu na tela SSD_1306
void draw_menu(void){
    ssd_1306_fill(white);
    ssd_1306_set_cursor(20, 5);
    ssd_1306_write_string("JOGO DA VELHA", Font_7x10, black);
    ssd_1306_draw_line(0, 20, 163, 20, black); // Linha decorativa
    ssd_1306_set_cursor(20, 27);
    ssd_1306_write_string("1 Jogador", Font_7x10, black);
    ssd_1306_set_cursor(20, 40);
    ssd_1306_write_string("2 Jogador", Font_7x10, black);
    ssd_1306_up_date_screen();
}

// Função de delay (usada sleep_ms da Pico SDK)
void delay_ms(int ms){
    sleep_ms(ms);
}

// Menu de seleção usando switch-case e os botões A e B
int menu_selection(void){
    int option = 1; // Inicia com opção 1 selecionada
    bool confirmed = false;
    draw_menu();
    while (!confirmed)
    {

        // Destaque a opção selecionada usando inversão de retângulo
        if (option == 1)
        {
            ssd_1306_inverte_retangulo(18, 25, 100, 35);
        }
        else
        {
            ssd_1306_inverte_retangulo(18, 38, 100, 48);
        }
        ssd_1306_up_date_screen();

        delay_ms(200); // Debounce e tempo para visualização

        // Lê o estado dos botões e gera um valor:
        // btn_state: 1 (apenas A), 2 (apenas B), 3 (ambos pressionados), 0 (nenhum)
        int btn_state = (_read_button_A() ? 1 : 0) + (_read_button_B() ? 2 : 0);

        switch (btn_state)
        {
        case 1: // Apenas botão A pressionado: seleciona opção 1
            option = 1;
            break;
        case 2: // Apenas botão B pressionado: seleciona opção 2
            option = 2;
            break;
        case 3: // Ambos pressionados: confirma a seleção
            confirmed = true;
            break;
        default:
            // Nenhum botão pressionado: nenhuma ação
            break;
        }
    }
    return option;
}

void clear_board(void){
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
}

void draw_board(void){
    ssd_1306_draw_line(VERT_LINE1, 0, VERT_LINE1, SCREEN_HEIGHT, white);
    ssd_1306_draw_line(VERT_LINE2, 0, VERT_LINE2, SCREEN_HEIGHT, white);
    ssd_1306_draw_line(0, HORZ_LINE1, SCREEN_WIDTH, HORZ_LINE1, white);
    ssd_1306_draw_line(0, HORZ_LINE2, SCREEN_WIDTH, HORZ_LINE2, white);
}

void get_cell_center(int row, int col, int *center_x, int *center_y){
    int x_start, x_end, y_start, y_end;

    if (col == 0)
    {
        x_start = 0;
        x_end = VERT_LINE1;
    }
    else if (col == 1)
    {
        x_start = VERT_LINE1;
        x_end = VERT_LINE2;
    }
    else
    {
        x_start = VERT_LINE2;
        x_end = SCREEN_WIDTH;
    }

    if (row == 0)
    {
        y_start = 0;
        y_end = HORZ_LINE1;
    }
    else if (row == 1)
    {
        y_start = HORZ_LINE1;
        y_end = HORZ_LINE2;
    }
    else
    {
        y_start = HORZ_LINE2;
        y_end = SCREEN_HEIGHT;
    }

    *center_x = (x_start + x_end) / 2;
    *center_y = (y_start + y_end) / 2;
}

void draw_x(int cell_row, int cell_col){
    int x_start, x_end, y_start, y_end;
    int margin = 5;

    if(cell_col == 0){
        x_start = 0;
        x_end = VERT_LINE1;
    }
    else if (cell_col == 1){
        x_start = VERT_LINE1;
        x_end = VERT_LINE2;
    }
    else{
        x_start = VERT_LINE2;
        x_end = SCREEN_WIDTH;
    }

    if(cell_row == 0){
        y_start = 0;
        y_end = HORZ_LINE1;
    }
    else if (cell_row == 1){
        y_start = HORZ_LINE1;
        y_end = HORZ_LINE2;
    }
    else{
        y_start = HORZ_LINE2;
        y_end = SCREEN_HEIGHT;
    }

    ssd_1306_draw_line(x_start + margin, y_start + margin, x_end - margin, y_end - margin, white);
    ssd_1306_draw_line(x_start + margin, y_end - margin, x_end - margin, y_start + margin, white);
}

void draw_o(int cell_row, int cell_col){
    int center_x, center_y;
    get_cell_center(cell_row, cell_col, &center_x, &center_y);
    int radius = 9; // raio
    ssd_1306_draw_circle(center_x, center_y, radius, white);
}

void draw_moves(void){
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(board[i][j] == 'X')
                draw_x(i, j);
            else if(board[i][j] == 'O')
                draw_o(i, j);
}

void update_board_screen(int current_player){
    ssd_1306_fill(black);
    draw_board();
    draw_moves();
    if(board[sel_row][sel_col] == ' '){
        if(blink_state){
            if(current_player == 0)
                draw_x(sel_row, sel_col);
            else
                draw_o(sel_row, sel_col);
        }
    }
    ssd_1306_up_date_screen();
}

int check_win(void){
    for (int i = 0; i < 3; i++){
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return 1;
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return 1;
    }
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return 1;
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return 1;
    return 0;
}

int board_full(void){
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

void wait_for_start(void){
    // Aguarda até que o botão esteja pressionado (lógica invertida: retorna 0 quando pressionado)
    while (gpio_get(BUTTON_JOYSTICK)){
        delay_ms(50);
    }
    // Aguarda o botão ser liberado para evitar múltiplas confirmações
    while (!gpio_get(BUTTON_JOYSTICK)){
        delay_ms(50);
    }
}

/* Função para desenhar um risco sobre os três pontos do vencedor */
void draw_winning_line(int start_row, int start_col, int end_row, int end_col) {
    int start_x, start_y, end_x, end_y;

    // Calcula o centro da célula inicial e final
    get_cell_center(start_row, start_col, &start_x, &start_y);
    get_cell_center(end_row, end_col, &end_x, &end_y);

    // Desenha a linha conectando os pontos inicial e final
    ssd_1306_draw_line(start_x, start_y, end_x, end_y, white);

    // Atualiza a tela para exibir a linha
    ssd_1306_up_date_screen();
    sleep_ms(2000);
}

int check_win_and_draw_line(void) {
    for (int i = 0; i < 3; i++) {
        // Verifica vitória em linhas
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2]) {
            draw_winning_line(i, 0, i, 2); // Linha horizontal
            return 1;
        }

        // Verifica vitória em colunas
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i]) {
            draw_winning_line(0, i, 2, i); // Linha vertical
            return 1;
        }
    }

    // Verifica vitória na diagonal principal
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        draw_winning_line(0, 0, 2, 2); // Diagonal principal
        return 1;
    }

    // Verifica vitória na diagonal secundária
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        draw_winning_line(0, 2, 2, 0); // Diagonal secundária
        return 1;
    }

    return 0; // Nenhuma vitória encontrada
}

/* Função para perguntar se deseja reiniciar o jogo */
bool ask_restart(){
    int selected_option = 0; // 0 = Sim, 1 = Não
    while (1){
        ssd_1306_fill(black);
        ssd_1306_set_cursor(10, 10);
        ssd_1306_write_string("Reiniciar?", Font_6x8, white);
        ssd_1306_set_cursor(10, 30);
        ssd_1306_write_string(selected_option == 0 ? "> Sim" : "  Sim", Font_6x8, white);
        ssd_1306_set_cursor(10, 40);
        ssd_1306_write_string(selected_option == 1 ? "> Nao" : "  Nao", Font_6x8, white);
        ssd_1306_up_date_screen();

        JoystickState js;
        int joy = get_joystick_direction(&js);
        if (joy == JOY_UP || joy == JOY_DOWN){
            selected_option = 1 - selected_option; // Alterna entre 0 e 1
            sleep_ms(200);                         // Debounce
        }
        else if (joy == JOY_SELECT)
        {
            return selected_option == 0; // Retorna true para "Sim", false para "Não"
        }
    }
}

void play_tic_tac_toe() {
    menu_selection();
    sleep_ms(1000);
    int current_player = 0; // 0 para 'X', 1 para 'O'
    int joy;

    clear_board();
    sel_row = 1;
    sel_col = 1;

    while (1) {
        blink_state = !blink_state;
        update_board_screen(current_player);
        sleep_ms(300);
        JoystickState js;
        joy = get_joystick_direction(&js);
        switch (joy) {
        case JOY_RIGHT:
            if (sel_col < 2)
                sel_col++;
            break;
        case JOY_LEFT:
            if (sel_col > 0)
                sel_col--;
            break;
        case JOY_UP:
            if (sel_row > 0)
                sel_row--;
            break;
        case JOY_DOWN:
            if (sel_row < 2)
                sel_row++;
            break;
        case JOY_SELECT:
            if (board[sel_row][sel_col] == ' ') {
                board[sel_row][sel_col] = (current_player == 0) ? 'X' : 'O';

                // Verifica se houve vitória
                if (check_win_and_draw_line()) {
                    update_board_screen(current_player);
                    sleep_ms(1000); // Pausa para exibir o resultado

                    // Exibe mensagem de fim de jogo e pergunta se deseja reiniciar
                    ssd_1306_fill(black);
                    ssd_1306_set_cursor(10, 10);
                    ssd_1306_write_string("Fim de Jogo!", Font_6x8, white);
                    ssd_1306_set_cursor(10, 30);
                    ssd_1306_write_string("Reiniciar?", Font_6x8, white);
                    ssd_1306_set_cursor(10, 40);
                    ssd_1306_write_string("> Sim", Font_6x8, white);
                    ssd_1306_up_date_screen();

                    if (ask_restart()) {
                        clear_board();
                        sel_row = 1;
                        sel_col = 1;
                        current_player = 0;
                        continue; // Reinicia o jogo
                    } else {
                        return; // Sai do jogo e volta ao menu
                    }
                }

                // Verifica se o tabuleiro está cheio (empate)
                if (board_full()) {
                    update_board_screen(current_player);
                    sleep_ms(1000); // Pausa para exibir o empate

                    // Exibe mensagem de empate e pergunta se deseja reiniciar
                    ssd_1306_fill(black);
                    ssd_1306_set_cursor(10, 10);
                    ssd_1306_write_string("Empate!", Font_6x8, white);
                    ssd_1306_set_cursor(10, 30);
                    ssd_1306_write_string("Reiniciar?", Font_6x8, white);
                    ssd_1306_set_cursor(10, 40);
                    ssd_1306_write_string("> Sim", Font_6x8, white);
                    ssd_1306_up_date_screen();

                    if (ask_restart()) {
                        clear_board();
                        sel_row = 1;
                        sel_col = 1;
                        current_player = 0;
                        continue; // Reinicia o jogo
                    } else {
                        return; // Sai do jogo e volta ao menu
                    }
                }

                // Passa a vez para o outro jogador
                current_player = 1 - current_player;
            }
            break;
        default:
            break;
        }
    }
}