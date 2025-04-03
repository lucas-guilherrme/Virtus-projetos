#include "../Aplication/inc/snake_game.h"

// Estrutura para representar a posição da cobrinha
typedef struct {
    int x;
    int y;
} SnakeSegment;

// Função principal do jogo da cobrinha
void play_snake_game(JoystickState *js, int difficulty) {
    // Configura a velocidade com base na dificuldade
    int speed;
    switch (difficulty) {
        case 0: speed = SNAKE_SPEED_EASY; break;
        case 1: speed = SNAKE_SPEED_MEDIUM; break;
        case 2: speed = SNAKE_SPEED_HARD; break;
        default: speed = SNAKE_SPEED_EASY; break;
    }

    // Inicializa a cobrinha
    SnakeSegment snake[100]; // Máximo de 100 segmentos
    int snake_length = 3;    // Comprimento inicial da cobrinha
    snake[0] = (SnakeSegment){8, 4}; // Cabeça da cobrinha no centro
    snake[1] = (SnakeSegment){7, 4};
    snake[2] = (SnakeSegment){6, 4};

    // Inicializa o fruto
    SnakeSegment fruit = {rand() % 16, rand() % 8}; // Coordenadas do fruto (16x8 grid no OLED)

    // Direção inicial da cobrinha
    int dx = 1, dy = 0; // Movendo para a direita inicialmente

    while (true) {
        // Atualiza o OLED com as instruções
        ssd_1306_fill(black);
        ssd_1306_set_cursor(0, 0);
        ssd_1306_write_string("Cobrinha!", Font_6x8, white);

        // Lê o estado do joystick
        Joystick_Read(js);
        js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);
        js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);

        // Atualiza a direção com base no joystick
        if (js->x_filtered > ADC_UPPER_THRESHOLD_X && dx == 0) {
            dx = 1; dy = 0; // Direita
        } else if (js->x_filtered < ADC_LOWER_THRESHOLD_X && dx == 0) {
            dx = -1; dy = 0; // Esquerda
        } else if (js->y_filtered > ADC_UPPER_THRESHOLD_Y && dy == 0) {
            dx = 0; dy = 1; // Baixo
        } else if (js->y_filtered < ADC_LOWER_THRESHOLD_Y && dy == 0) {
            dx = 0; dy = -1; // Cima
        }

        // Move a cobrinha
        for (int i = snake_length - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0].x += dx;
        snake[0].y += dy;

        // Verifica colisão com as bordas
        if (snake[0].x < 0 || snake[0].x >= 16 || snake[0].y < 0 || snake[0].y >= 8) {
            // Game over
            ssd_1306_fill(black);
            ssd_1306_set_cursor(0, 20);
            ssd_1306_write_string("Game Over!", Font_6x8, white);
            ssd_1306_up_date_screen();
            sleep_ms(2000);
            return;
        }

        // Verifica colisão com o próprio corpo
        for (int i = 1; i < snake_length; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                // Reduz o tamanho da cobrinha
                snake_length--;
                break;
            }
        }

        // Verifica se a cobrinha comeu o fruto
        if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
            snake_length++; // Aumenta o tamanho da cobrinha
            fruit.x = rand() % 16; // Gera um novo fruto
            fruit.y = rand() % 8;
        }

        // Desenha a cobrinha no OLED
        for (int i = 0; i < snake_length; i++) {
            ssd_1306_set_cursor(snake[i].x * 6, snake[i].y * 8); // Cada célula é 6x8 pixels
            ssd_1306_write_string("O", Font_6x8, white); // Segmento da cobrinha
        }

        // Desenha o fruto no OLED
        ssd_1306_set_cursor(fruit.x * 6, fruit.y * 8);
        ssd_1306_write_string("X", Font_6x8, white); // Fruto

        // Atualiza o display
        ssd_1306_up_date_screen();

        // Aguarda antes do próximo movimento
        sleep_ms(speed);
    }
}