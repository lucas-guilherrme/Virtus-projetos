/******************************************************************************
 * @file    joystick.h
 * @brief   Header providing general constants, a structure for joystick data,
 *          and function prototypes for handling joystick operations.
 * @authors 
 * @date    March 2025
 * @version 1.0.0
 *
 * @note    This file contains a set of values, a data structure to store raw
 *          and processed readings, and functions for initializing, reading,
 *          and filtering joystick data in various ways.
 ******************************************************************************/

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "config.h"
/*********************************| struct |************************************/
#define MOVING_AVG_WINDOW 5

/**
 * @brief Holds the raw and filtered joystick data and buffers.
 */
typedef struct {
    int16_t x_raw;                         ///< Raw X-axis value
    int16_t y_raw;                         ///< Raw Y-axis value
    int16_t x_filtered;                    ///< Filtered X-axis value
    int16_t y_filtered;                    ///< Filtered Y-axis value
    int16_t x_buffer[MOVING_AVG_WINDOW];   ///< X-axis moving average buffer
    int16_t y_buffer[MOVING_AVG_WINDOW];   ///< Y-axis moving average buffer
    int16_t prev_x;                        ///< Previous filtered X-axis value
    int16_t prev_y;                        ///< Previous filtered Y-axis value
}JoystickState;


/****************************|function|****************************************/

void Joystick_Init(JoystickState *js);
void Joystick_Read(JoystickState *js);
void Joystick_ApplyFilters(JoystickState *js);
int16_t Joystick_ApplyDeadZone(int16_t value);
int16_t Joystick_MovingAverageFilter(int16_t new_sample, int16_t *buffer, uint8_t size);
int16_t Joystick_LowPassFilter(int16_t new_sample, int16_t prev_value);
int16_t Joystick_MedianFilter(int16_t *buffer, uint8_t size);
bool _joystick_read_button(void);

#endif // JOYSTICK_H