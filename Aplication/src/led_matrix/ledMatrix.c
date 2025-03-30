/**********************************|includes|********************************************** */
#include "../Aplication/inc/ledMatrix.h"

// Global buffer for storing colors of 25 LEDs (5x5)
static uint32_t led_matrix[25] = {0};

PIO pio_ws = pio0;       // PIO instance for WS2812 LED control
uint sm_ws = 0;          // State machine for WS2812 LED control

ws2812_config_t ws2812_config = {.pin = WS2812};


/* Initializes WS2812 LED matrix and configures PIO. */
void LedMatrix_Init(ws2812_config_t *ws2812_config) {
    // Load WS2818b program into the PIO and claim a state machine.
    uint offset = pio_add_program(pio_ws, &ws2818b_program);

    sm_ws = pio_claim_unused_sm(pio_ws, true);

    // Initialize the state machine with the chosen PIO program.
    ws2818b_program_init(pio_ws, sm_ws, offset, ws2812_config->pin, FREQUENCIA);

    // Clear the LED buffer by setting all elements to zero.
    LedMatrix_Clear();

    // Update the physical LEDs with the cleared buffer.
    LedMatrix_Update();
}

/***********************************| Cometarios |*************************************/

/* Sets the same color for all LEDs in the matrix buffer. */
void LedMatrix_Fill(led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    
    // Build the color value once.
    uint32_t color = build_color(red, green, blue);
    // Set every LED in the buffer to this color.
    LedMatrix_Clear();

}


/* Fills a quadrant of the LED matrix with a specified color. */
void LedMatrix_FillRectangle(int x1, int y1, int x2, int y2, led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    // Build the color value once.
    uint32_t color = build_color(red, green, blue);
    
    // Clamp the provided coordinates to the valid range [0, 4].
    x1 = clamp(x1, 0, 4);
    y1 = clamp(y1, 0, 4);
    x2 = clamp(x2, 0, 4);
    y2 = clamp(y2, 0, 4);
    
    // Ensure x1/y1 is the top-left and x2/y2 is the bottom-right.
    if(x1 > x2) { int temp = x1; x1 = x2; x2 = temp; }
    if(y1 > y2) { int temp = y1; y1 = y2; y2 = temp; }
    
    // Loop through the specified rows.
    for (int y = y1; y <= y2; y++) {

        // Loop through the specified columns.
        for (int x = x1; x <= x2; x++) {
            int index;
            // Compute the buffer index applying snake wiring.
            if (y % 2 == 0) {
                index = y * 5 + (4 - x); // Reverse order for even rows.
            }
            else {
                index = y * 5 + x;       // Normal order for odd rows.
            }
            // Set the LED color in the buffer.
            led_matrix[index] = color;
        }
    }
}


/*Funcao optimizada*/
void led_matrix_fill_rectangle(int x_1, int y_1, int x_2, int y_2, led_intensity_t red, led_intensity_t green, led_intensity_t blue){
    
    uint32_t color = build_color(red, green, blue);

    x_1 = clamp(x_1, 0, 4);
    y_1 = clamp(y_1, 0, 4);
    y_2 = clamp(y_2, 0, 4);
    x_2 = clamp(x_2, 0, 4);

    if(x_1 > x_2){
        int temp = x_1;
        x_1 = x_2;
        x_2 = temp;
    }

    if(y_1 > y_2){
        int temp = y_1;
        y_1 = y_2;
        y_2 = temp;
    }

    for(int y = y_1; y <= y_2; y++){
        int base_index = y * 5;
        for(int x = x_1; x <= x_2; x++){
            int index = (y % 2 == 0) ? base_index + (4 - x)  : base_index + x;
            led_matrix[index] = color;
        }
    }
}


/* Sets the color of an individual LED at (x,y) in the matrix buffer. */
void LedMatrix_SetPixel(int x, int y, led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    // Clamp x and y to range [0,4].
    x = clamp(x, 0, 4);
    y = clamp(y, 0, 4);
    int index;
    // Compute buffer index using snake wiring.
    if (y % 2 == 0) {
        index = y * 5 + (4 - x); // Even rows reversed.
    } else {
        index = y * 5 + x;       // Odd rows direct.
    }
    // Store the color.
    led_matrix[index] = build_color(red, green, blue);
}


/* Sends the LED matrix buffer to the physical LEDs via PIO FIFO. */
void LedMatrix_Update(void) {
    // Loop through the buffer.
    for (int i = 0; i < LED_SIZE; i++) {
        uint32_t c = led_matrix[i];
        uint8_t g = (c >> 16) & 0xFF;  // Extract green.
        uint8_t r = (c >>  8) & 0xFF;  // Extract red.
        uint8_t b = c & 0xFF;          // Extract blue.
        // Transmit color components in GRB order.
        pio_sm_put_blocking(pio_ws, sm_ws, g);
        pio_sm_put_blocking(pio_ws, sm_ws, r);
        pio_sm_put_blocking(pio_ws, sm_ws, b);
    }
    // Delay for latching.
    sleep_us(DELAY);
}


/* Clears the LED matrix buffer by setting all elements to zero and updates the LEDs. */
void LedMatrix_Clear(void) {
    // Loop through each LED in the matrix and set its value to 0.
    for(int i = 0; i < LED_SIZE; i++ ){
        led_matrix[i] = 0;
    }
}


/* Clamps a value between a minimum and maximum. */
static int clamp(int val, int min, int max) {
    if (val < min) 
        return min;                    // Return lower bound if value is too low.
    if (val > max) 
        return max;                  // Return upper bound if value is too high.
    return val;                      // Otherwise, return the original value.
}


/* Builds a color in 0xGRB format, clamping intensity values to [0, 4]. */
static uint32_t build_color(led_intensity_t red, led_intensity_t green, led_intensity_t blue) {

    // Clamp each intensity value to ensure it is between 0 and 4.
    red   = clamp(red, 0, 4);
    green = clamp(green, 0, 4);
    blue  = clamp(blue, 0, 4);
    // Combine channels into one 32-bit value; green is the most significant byte.
    return ((uint32_t)green << 16) | ((uint32_t)red << 8) | ((uint32_t)blue);
}


void _led_matrix_moving(JoystickState *js) {

    static bool led_on[5][5] = {false};  // Tracks which LEDs remain lit
    
    static int x = 2;
    static int y = 2;
    
    Joystick_Read(js);

    js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);
    js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);

    // Adjust X position
    if (js->x_filtered > ADC_UPPER_THRESHOLD_X && x < 4) {
        x++;
    } else if (js->x_filtered < ADC_LOWER_THRESHOLD_X && x > 0) {
        x--;
    }

    // Adjust Y position
    if (js->y_filtered > ADC_UPPER_THRESHOLD_Y && y < 4) {
        y++;
    } else if (js->y_filtered < ADC_LOWER_THRESHOLD_Y && y > 0) {
        y--;
    }

    // Check if button is pressed (pull-up assumed, pressed reads 0)
    if (_joystick_read_button()) {
        led_on[y][x] = true;  // Keep this LED lit permanently
        LedMatrix_SetPixel(x, y, LED_OFF, LED_WEAK, LED_OFF);
        LedMatrix_Update();
        sleep_ms(200);       // Debounce
    }

    // Clear the matrix
    LedMatrix_Clear();

    // Turn on any previously lit LEDs
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (led_on[row][col]) {
                LedMatrix_SetPixel(col, row, LED_OFF, LED_WEAK, LED_OFF);
            }
        }
    }

    // Light the current position
    LedMatrix_SetPixel(x, y, LED_WEAK, LED_WEAK, LED_WEAK);

    // Update the LEDs and wait
    LedMatrix_Update();
    sleep_ms(60);

}

void led_matrix_game(JoystickState *js) {
    static int target_x = 0, target_y = 0; // Coordenadas do ponto aleatório
    static int cursor_x = 2, cursor_y = 2; // Coordenadas do cursor (inicializado no centro)

    // Inicializa o primeiro ponto aleatório
    if (target_x == 0 && target_y == 0) {
        target_x = rand() % 5;
        target_y = rand() % 5;
    }

    while (true) {
        // Lê o estado do joystick
        Joystick_Read(js);
        js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);
        js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);

        // Ajusta a posição do cursor com base no movimento do joystick
        if (js->x_filtered > ADC_UPPER_THRESHOLD_X && cursor_x < 4) {
            cursor_x++;
        } else if (js->x_filtered < ADC_LOWER_THRESHOLD_X && cursor_x > 0) {
            cursor_x--;
        }

        if (js->y_filtered > ADC_UPPER_THRESHOLD_Y && cursor_y < 4) {
            cursor_y++;
        } else if (js->y_filtered < ADC_LOWER_THRESHOLD_Y && cursor_y > 0) {
            cursor_y--;
        }

        // Verifica se o botão do joystick foi pressionado
        if (_joystick_read_button()) {
            // Se o cursor estiver no ponto aleatório, gera um novo ponto
            if (cursor_x == target_x && cursor_y == target_y) {
                target_x = rand() % 5;
                target_y = rand() % 5;
            }
        }

        // Limpa a matriz de LEDs
        LedMatrix_Clear();

        // Acende o ponto aleatório com uma cor específica
        LedMatrix_SetPixel(target_x, target_y, LED_STRONG, LED_OFF, LED_OFF); // Vermelho

        // Acende o cursor com outra cor
        LedMatrix_SetPixel(cursor_x, cursor_y, LED_WEAK, LED_WEAK, LED_WEAK); // Branco fraco

        // Atualiza a matriz de LEDs
        LedMatrix_Update();
        sleep_ms(100); // Pequeno atraso para suavizar o movimento
    }
}

void led_matrix_challenge_game(JoystickState *js) {
    static int sequence[25][2]; // Armazena a sequência de LEDs (x, y)
    static int sequence_length = 0; // Comprimento atual da sequência
    static int cursor_x = 2, cursor_y = 2; // Coordenadas do cursor (inicializado no centro)
    int current_step = 0; // Passo atual na sequência

    // Inicializa o primeiro ponto aleatório
    if (sequence_length == 0) {
        sequence[0][0] = rand() % 5; // x
        sequence[0][1] = rand() % 5; // y
        sequence_length = 1;
    }

    while (true) {
        // Lê o estado do joystick
        Joystick_Read(js);
        js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);
        js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);

        // Ajusta a posição do cursor com base no movimento do joystick
        if (js->x_filtered > ADC_UPPER_THRESHOLD_X && cursor_x < 4) {
            cursor_x++;
        } else if (js->x_filtered < ADC_LOWER_THRESHOLD_X && cursor_x > 0) {
            cursor_x--;
        }

        if (js->y_filtered > ADC_UPPER_THRESHOLD_Y && cursor_y < 4) {
            cursor_y++;
        } else if (js->y_filtered < ADC_LOWER_THRESHOLD_Y && cursor_y > 0) {
            cursor_y--;
        }

        // Verifica se o botão do joystick foi pressionado
        if (_read_button_A()) {
            // Verifica se o cursor está no LED correto da sequência
            if (cursor_x == sequence[current_step][0] && cursor_y == sequence[current_step][1]) {
                current_step++; // Avança para o próximo passo da sequência

                // Se completou a sequência, adiciona um novo LED
                if (current_step == sequence_length) {
                    sequence_length++;
                    sequence[sequence_length - 1][0] = rand() % 5; // Novo x
                    sequence[sequence_length - 1][1] = rand() % 5; // Novo y
                    current_step = 0; // Reinicia para o próximo ciclo
                }
            } else {
                // Errou: acende toda a matriz em vermelho por 100 ms
                LedMatrix_Fill(LED_STRONG, LED_OFF, LED_OFF); // Vermelho forte
                LedMatrix_Update();
                sleep_ms(100);

                // Limpa a matriz e reinicia o jogo
                LedMatrix_Clear();
                LedMatrix_Update();
                sequence_length = 1;
                sequence[0][0] = rand() % 5;
                sequence[0][1] = rand() % 5;
                current_step = 0;
            }
        }

        // Limpa a matriz de LEDs
        LedMatrix_Clear();

        // Mostra a sequência atual
        for (int i = 0; i < sequence_length; i++) {
            LedMatrix_SetPixel(sequence[i][0], sequence[i][1], LED_STRONG, LED_OFF, LED_OFF); // Vermelho
        }

        // Mostra o cursor
        LedMatrix_SetPixel(cursor_x, cursor_y, LED_WEAK, LED_WEAK, LED_WEAK); // Branco fraco

        // Atualiza a matriz de LEDs
        LedMatrix_Update();
        sleep_ms(100); // Pequeno atraso para suavizar o movimento
    }
}