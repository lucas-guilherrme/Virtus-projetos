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

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

/*******************************************| Definicoes | *********************/
#define ADC_PIN_X   26
#define ADC_PIN_Y   27

#define BUTTON_JOYSTICK 22

#define ADC_CHANEL_1 1
#define ADC_CHANEL_0 0

#define AVERAGE_VALUE 2048 

#define MOVING_AVG_WINDOW 5    // Size of the moving average window
#define DEADZONE_THRESHOLD 100 // Dead zone
#define LOWPASS_ALPHA 0.2      // Low-pass filter coefficient

/*********************************| struct |************************************/
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
} JoystickState;


/****************************|function|****************************************/

/**
 * @brief Initializes the joystick module.
 */
void Joystick_Init(JoystickState *js);

/**
 * @brief Reads raw joystick values into the given state structure.
 * @param js Pointer to JoystickState structure.
 */
void Joystick_Read(JoystickState *js);

/**
 * @brief Applies filters (moving average and low-pass) to the joystick readings.
 * @param js Pointer to JoystickState structure.
 */
void Joystick_ApplyFilters(JoystickState *js);

/**
 * @brief Applies a dead-zone filter to a given axis value.
 * @param value Axis value.
 * @return Filtered value.
 */
int16_t Joystick_ApplyDeadZone(int16_t value);

/**
 * @brief Applies a moving average filter to a new sample.
 * @param new_sample New axis sample.
 * @param buffer Buffer for moving average.
 * @param size Size of the buffer.
 * @return Filtered value.
 */
int16_t Joystick_MovingAverageFilter(int16_t new_sample, int16_t *buffer, uint8_t size);

/**
 * @brief Applies a low-pass filter to a new sample.
 * @param new_sample New axis sample.
 * @param prev_value Previous filtered value.
 * @return Filtered value.
 */
int16_t Joystick_LowPassFilter(int16_t new_sample, int16_t prev_value);

/**
 * @brief Applies a median filter to values in the buffer.
 * @param buffer Buffer of samples.
 * @param size Size of the buffer.
 * @return Median value.
 */
int16_t Joystick_MedianFilter(int16_t *buffer, uint8_t size);

bool _joystick_read_button(void);

#endif // JOYSTICK_H