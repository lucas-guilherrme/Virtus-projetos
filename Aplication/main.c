#include "inc/ssd_1306.h"
#include "inc/ssd_1306_conf.h"
#include "inc/ssd_1306_font.h"
#include "inc/_button.h"
#include "inc/joystick.h"
#include "inc/jogo_da_velha.h"
#include "inc/led_sequence_game.h"
#include "inc/snake_game.h"

#define MENU_OPTIONS 3

const char *game_names[MENU_OPTIONS] = {
    "Jogo da Velha",
    "Snake",
    "Sequencia Correta"
};

int show_menu(JoystickState *js, const char *game_names[]);
void start_countdown();

int main() {
    stdio_init_all();
    ssd_1306_init();
    button_init();

    LedMatrix_Init(&ws2812_config); // Initializes the LED Matrix

    JoystickState js = {0};
    Joystick_Init(&js);

    int game = show_menu(&js, game_names);

    // Start the countdown before launching the game
    start_countdown();

    switch (game) {
        case 0: {
            play_tic_tac_toe();
        } break;
        case 1: {
            play_led_sequence_game(&js);
        } break;
        case 2:{
            int dificuldade = show_menu(&js,(const char *[]){"Facil", "Medio", "Dificil"});
            play_snake_game(&js, dificuldade);
        }break;
    }

    return 0;
}

int show_menu(JoystickState *js, const char *game_names[]) {
    int selected_option = 0;
    bool button_pressed = false;
    while (1) {
        // Clear the screen
        ssd_1306_fill(black);

        // Display the menu options
        for (int i = 0; i < MENU_OPTIONS; i++) {
            if (i == selected_option) {
                // Highlight the selected option
                ssd_1306_write_string("> ", Font_6x8, white);
            } else {
                ssd_1306_write_string("  ", Font_6x8, white);
            }
            ssd_1306_write_string(game_names[i], Font_6x8, white);
            ssd_1306_set_cursor(0, (i + 1) * 10); // Move to the next line
        }

        // Update the screen
        ssd_1306_up_date_screen();

        // Read joystick values
        Joystick_Read(js);
        Joystick_ApplyFilters(js);

        // Navigate the menu
        if (js->y_filtered < (AVERAGE_VALUE - DEADZONE_THRESHOLD)) {
            // Move up
            if (selected_option > 0) {
                selected_option--;
                sleep_ms(200); // Delay to avoid rapid movements
            }
        } else if (js->y_filtered > (AVERAGE_VALUE + DEADZONE_THRESHOLD)) {
            // Move down
            if (selected_option < MENU_OPTIONS - 1) {
                selected_option++;
                sleep_ms(200); // Delay to avoid rapid movements
            }
        }

        // Check if the button was pressed
        button_pressed = _joystick_read_button();
        if (button_pressed) {
            // Return the selected option
            return selected_option;
        }
    }
}

void start_countdown() {
    // Display a 10-second countdown on the OLED screen
    for (int i = 10; i > 0; i--) {
        ssd_1306_fill(black); // Clear the screen
        ssd_1306_set_cursor(0, 20);
        ssd_1306_write_string("Starting in:", Font_6x8, white);
        ssd_1306_set_cursor(50, 30);

        char countdown[3];
        snprintf(countdown, sizeof(countdown), "%d", i); // Convert the countdown number to a string
        ssd_1306_write_string(countdown, Font_6x8, white);

        ssd_1306_up_date_screen(); // Update the screen
        sleep_ms(1000); // Wait for 1 second
    }

    // Clear the screen after the countdown
    ssd_1306_fill(black);
    ssd_1306_up_date_screen();
}