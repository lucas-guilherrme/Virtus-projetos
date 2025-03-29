/**
 * @file main.c
 * @brief Main application file for Raspberry Pi Pico project integrating joystick, LED matrix, and push buttons.
 *
 * This file demonstrates the initialization and usage of various peripherals, including:
 * - Joystick for input
 * - LED matrix for visual output
 * - Push buttons with interrupt handling
 * 
 * The code includes examples for testing individual components and integrating them together.
 * Uncomment specific test functions to run the desired examples.
 *
 * @note Ensure the correct GPIO pins are connected for the LED RGB and buttons as defined in the code.
 */

#include <stdio.h>
#include "pico/stdlib.h"


#include "inc/examples.h"
/*
#include "examples.h"
#include "../Libs/button/inc/button.h"
#include "../Libs/led_matrix/inc/ledMatrix.h"
#include "inc/aplication.h"*/

#include "../Libs/joystick_lib_test/inc/joystick.h"

//#include "../inc/jogo_da_velha.h"

#include "../Libs/button/inc/_button.h"

/**************************************| Variaveis globais |************************/

/**************************************| prototipo das funcoes |*********************/
//void teste_button_evento( ButtonEvent evento);


/**
 * @brief Main function of the application.
 *
 * Initializes the UART, joystick, LED matrix, and push buttons. Provides examples for:
 * - Testing push button interrupts
 * - Testing joystick functionality (complete, dead zone, low-pass filter)
 * - Testing LED matrix animations (wipe, moving square, spiral)
 * - Integrating joystick control with LED matrix
 *
 * @return int Returns 0 on successful execution.
 */
/*
int main(){

    stdio_init_all();               // Initializes the UART for printing
    /*************************************|Inicializar o botoes|***************************
    init_button("BUTTON_B"); 
    init_button("BUTTON_A"); 

    init_button_IT(BUTTON_A);
    init_button_IT(BUTTON_B);

    /*************************************|Inicializar a matrix|***************************
    LedMatrix_Init(&ws2812_config); // Initializes the LED Matrix

    /************************************| Inicializacao do joystick|*********************

    JoystickState joystick = {0};         // Declare a joystick state variable
    Joystick_Init(&joystick);            // Initialize the joystick

    while (1) {

    // --------------------- PushButton's Example --------------------- //

        //process_button_state(teste_button_evento);
        //sleep_ms(100);

    // ---------------------- Joystick's Example ---------------------- //
        
        //Test_joystick_Complete(&joystick);
        //Test_joystick_DeadZone(&joystick);
        //Test_joystick_LowpassFilter(&joystick);

    // --------------------- LedMatrix's Example ---------------------- //

        //Test_LedMatrix_Wipe();
        //Test_LedMatrix_MovingSquare();
        //Test_LedMatrix_Spiral();

    // ---------------- Tests integrating all the libs ---------------- //
        //Test_joystick_LedMatrixControl(&joystick);
        //Test_joystick_LedMatrixColorToggle(&joystick);

    // ----------------------------- LOG ------------------------------ //

        //log_values(&joystick);

    }

    return 0;
}
*/

int main(){

    stdio_init_all();
    button_init();

    JoystickState joystick = {0};         // Declare a joystick state variable
    Joystick_Init(&joystick);            // Initialize the joystick

    int contador = 0;

    for(;;){
        if(_read_button_A()){
            contador += 1;
            printf("Contador A: %d\n", contador);
        }

        if(_read_button_B()){
            contador -= 1;
            printf("Contador B: %d\n", contador);
        }
        sleep_ms(100);

        Joystick_Read(&joystick);
        printf("Cordenada x %d\nCordenada y %d\n", joystick.x_raw, joystick.y_raw);
        sleep_ms(500);

        if(_joystick_read_button()){
            printf("Botao pressionado\n");
        }
        sleep_ms(100);
    }

    return 0;
}
/*
void teste_button_evento(ButtonEvent evento){
    int conter = 0;
    if(evento == SINGLE_CLICK){
        if(gpio_get(BUTTON_A) == 0){
            conter = conter + 1;;
            printf("Contador: %d\n", conter);
        }
        else if(gpio_get(BUTTON_B) == 0){
            conter -=1;
            printf("Contador: %d\n", conter);
        }
        
    }
}
    */