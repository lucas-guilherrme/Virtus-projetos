/******************************************************************************
 * @file    examples.h
 * @brief   Demonstration header file with test examples of the libraries.
 *
 * @authors 
 * @date    March 2025
 * @version 1.0.0
 *
 * @note    This file contains simple functions that demonstrate how to use
 *          the About LED Matrix, Push-Buttons and Joystick Libraries.
 ******************************************************************************/

#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <stdio.h>
#include "pico/stdlib.h"


#include "inc/joystick.h"
#include "inc/ledMatrix.h"
#include "inc/button.h" 

#define ADC_UPPER_THRESHOLD_X 2900 	// Upper limit of the ADC on the X axis right
#define ADC_LOWER_THRESHOLD_X 1300  // Lower limit of the ADC on the X axis left
#define ADC_UPPER_THRESHOLD_Y 2800 	// Upper limit of the ADC on the Y axis up
#define ADC_LOWER_THRESHOLD_Y 1300 	// Lower limit of the ADC on the Y axis down

/***************************** Joystick Examples *****************************/

/**
 * @brief Tests the joystick reading and applies all configured filters.
 *
 * This function reads the raw values from the joystick, applies any
 * configured filters to the readings, and then prints both the raw
 * and filtered values to the console. A delay is introduced to allow
 * for stable output.
 *
 * @param js Pointer to a JoystickState structure that holds the raw
 *           and filtered joystick values.
 *
 * @note The function assumes that the joystick reading and filtering
 *       functions (`Joystick_Read` and `Joystick_ApplyFilters`) are
 *       properly implemented and configured.
 * @note The output is printed in the format:
 *       "Bruto: X=<raw_x>, Y=<raw_y> || Filtrado: X=<filtered_x>, Y=<filtered_y>"
 * @note A delay of 50 milliseconds is applied after printing the values.
 */
/* Tests the joystick reading and applies all filters. */
void Test_joystick_Complete(JoystickState *js){
    
    // Read the joystick values
    Joystick_Read(js);
        
    // Apply all configured filters
    Joystick_ApplyFilters(js);

    printf("Bruto: X=%d, Y=%d || Filtrado: X=%d, Y=%d\n",
        js->x_raw, js->y_raw,
        js->x_filtered, js->y_filtered);
    sleep_ms(50);
}


/* Tests only the dead zone filter for the joystick. */
void Test_joystick_DeadZone(JoystickState *js) {

    // Read the joystick values
    Joystick_Read(js);

    // Apply only the dead zone filter
    js->x_filtered = Joystick_ApplyDeadZone(js->x_raw);
    js->y_filtered = Joystick_ApplyDeadZone(js->y_raw);

    printf("Bruto: X=%d, Y=%d || Filtrado: X=%d, Y=%d  || DEADZONE=%d\n",
        js->x_raw, js->y_raw,
        js->x_filtered, js->y_filtered, DEADZONE_THRESHOLD);
    sleep_ms(50);
}


/* Tests only the low-pass filter for the joystick. */
void Test_joystick_LowpassFilter(JoystickState *js) {

    // Read the joystick values
    Joystick_Read(js);

    // Apply only the low-pass filter
    js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->prev_x);
    js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->prev_y);

    printf("Bruto: X=%d, Y=%d || Filtrado: X=%d, Y=%d  || ALPHA=%.1f\n",
        js->x_raw, js->y_raw,
        js->x_filtered, js->y_filtered, LOWPASS_ALPHA);

    // Update previous values
    js->prev_x = js->x_filtered;
    js->prev_y = js->y_filtered;
    sleep_ms(50);
}

/**************************** LED Matrix Examples ****************************/

/* Lights each LED sequentially across the matrix ("wipe" effect). */
void Test_LedMatrix_Wipe(void) {
    // Clear the matrix first.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(100);

    // Iterate through each coordinate in the 5x5 matrix.
    for (int index = 0; index < 25; index++) {
        // Compute x and y from the linear index.
        int x = index % 5;
        int y = index / 5;
        
        // Set the current LED to red at maximum intensity.
        LedMatrix_SetPixel(x, y, LED_WEAK, LED_OFF, LED_OFF);
        LedMatrix_Update();
        sleep_ms(200);
    }
    
    sleep_ms(500);
    // Clear the matrix after the wipe.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(200);
}

/* Animates a 2x2 square moving across the LED matrix. */
void Test_LedMatrix_MovingSquare(void) {
    // Traverse rows and columns to animate the square.
    // The square will move from top-left (0,0) to bottom-right (3,3).
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            LedMatrix_Clear();
            // Fill a 2x2 rectangle starting at (x,y) with a blend of intensities.
            LedMatrix_FillRectangle(x, y, x+2, y+1, LED_WEAK, LED_STRONG, LED_WEAK);
            LedMatrix_Update();
            sleep_ms(300);
        }
    }
    
    // Clear the matrix after the animation.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(200);
}

/**
 * @brief Animates a spiral effect on the LED matrix.
 */
void Test_LedMatrix_Spiral(void) {
    // Ordem espiral para [x,y] na matriz 5x5.
    int spiral_order[25][2] = {
        {0,0}, {1,0}, {2,0}, {3,0}, {4,0},
        {4,1}, {4,2}, {4,3}, {4,4},
        {3,4}, {2,4}, {1,4}, {0,4},
        {0,3}, {0,2}, {0,1},
        {1,1}, {2,1}, {3,1},
        {3,2}, {3,3},
        {2,3}, {1,3},
        {1,2}, {2,2}
    };
    
    // Limpa a matriz.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(200);
    
    // Acende LEDs na ordem espiral.
    for (int i = 0; i < 25; i++) {
        int x = spiral_order[i][0];
        int y = spiral_order[i][1];
        LedMatrix_SetPixel(x, y, LED_WEAK, LED_OFF, LED_WEAK);  // exemplo: roxo
        LedMatrix_Update();
        sleep_ms(150);
    }
    sleep_ms(500);

    // Turn off the LEDs one by one using the same spiral order.
    for (int i = 24; i > 0; i--) {
        int x = spiral_order[i][0];
        int y = spiral_order[i][1];
        LedMatrix_SetPixel(x, y, LED_OFF, LED_OFF, LED_OFF);  // Turn off the LED.
        LedMatrix_Update();
        sleep_ms(150);
    }
    
    sleep_ms(200);
}


/*************************** Push-Buttons Examples ***************************/

/*********************** Tests integrating all the libs **********************/


/* Moves an LED around the matrix based on joystick input. */
void Test_joystick_LedMatrixControl(JoystickState *js) {
    static int x = 2;
    static int y = 2;
    static bool ledOn[5][5] = {false};  // Tracks which LEDs remain lit

    // Read and filter joystick values
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
    if (event == SINGLE_CLICK) {
        ledOn[y][x] = true;  // Keep this LED lit permanently
        sleep_ms(200);       // Debounce
    }

    // Clear the matrix
    LedMatrix_Clear();

    // Turn on any previously lit LEDs
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (ledOn[row][col]) {
                LedMatrix_SetPixel(col, row, LED_WEAK, LED_OFF, LED_OFF);
            }
        }
    }

    // Light the current position
    LedMatrix_SetPixel(x, y, LED_WEAK, LED_OFF, LED_OFF);

    // Update the LEDs and wait
    LedMatrix_Update();
    sleep_ms(60);
}

/* Move um LED com o joystick. Quando o botão é pressionado, muda a cor do LED. */
void Test_joystick_LedMatrixColorToggle(JoystickState *js) {
    static int x = 2, y = 2;
    static uint8_t colorIndex = 0;
    // Lista de cores de exemplo
    const led_intensity_t colors[3][3] = {
        {LED_WEAK, LED_OFF, LED_OFF   },    // Red
        {LED_OFF, LED_WEAK, LED_OFF   },    // Green
        {LED_OFF, LED_OFF, LED_WEAK}        // Blue
    };

    // Read and filter joystick values
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
    if (event == SINGLE_CLICK) {
        colorIndex = (colorIndex + 1) % 3;
        sleep_ms(200); // Debounce
    }

    // Clear the matrix and set the new pixel
    LedMatrix_Clear();
    LedMatrix_SetPixel(
        x, y,
        colors[colorIndex][0],
        colors[colorIndex][1],
        colors[colorIndex][2]
    );

    // Update the LEDs and wait
    LedMatrix_Update();
    sleep_ms(50);
}

/************************************ Logs ***********************************/

void log_values(JoystickState *js) {

    int x_MovingAvg = 0;
    int y_MovingAvg = 0;
    int x_DeadZone = 0;
    int y_DeadZone = 0;

    Joystick_Read(js);
    js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);
    js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);
    x_MovingAvg = Joystick_MovingAverageFilter(js->x_raw, js->x_buffer, MOVING_AVG_WINDOW);
    y_MovingAvg = Joystick_MovingAverageFilter(js->y_raw, js->y_buffer, MOVING_AVG_WINDOW);
    x_DeadZone = Joystick_ApplyDeadZone(js->x_raw);
    y_DeadZone = Joystick_ApplyDeadZone(js->y_raw);

    printf("%d,%d,%d,%d\n", js->y_raw, js->y_filtered, y_MovingAvg, y_DeadZone);
    //printf("%d,%d,%d,%d\n", js->x_raw, js->x_filtered, x_MovingAvg, x_DeadZone);

    sleep_ms(1);
}


#endif /*EXAMPLES_H*/