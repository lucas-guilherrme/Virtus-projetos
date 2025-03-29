/******************************************************************************
 * @file    button.h
 * @brief   Header file defining constants, structures, and function prototypes
 *          for handling button operations including debounce, double click, 
 *          and long press detection on the Raspberry Pi Pico W.
 * @authors 
 * @date    March 2025
 * @version 1.0.0
 *
 * @note    This file provides an interface for initializing buttons, reading
 *          their states with proper debounce handling, detecting long presses, 
 *          and double click events. It also includes a helper function to blink 
 *          an LED based on button events.
 ******************************************************************************/

 #ifndef BUTTONS_LIB_H
 #define BUTTONS_LIB_H
 
/*******************************|include|*************************************/
 #include "pico/stdlib.h"
 #include <string.h>
 #include <stdio.h>

/******************************|define|***************************************/
 /** @brief GPIO pin for button A */
 #define BUTTON_A 5
 
 /** @brief GPIO pin for button B */
 #define BUTTON_B 6

 /** @brief GPIO pin for button SW */
 #define BUTTON_SW 22
 
 /** @brief Debounce delay in milliseconds */
 #define DEBOUNCE 50
 
 /** @brief Maximum allowed interval in milliseconds for a double click */
 #define DOUBLE_CLICK_TIME_MS 500
 
 /** @brief Minimum press duration in milliseconds to detect a long press */
 #define LONG_PRESS_TIME_MS 1000

 /******************************|struct|***************************************/

 /**
  * @brief Structure for storing the state of a button.
  *
  * This structure holds the current and last states of a button, along with
  * timing information used to implement debounce, long press, and double click detection.
  */
 typedef struct {
     uint gpio;                   ///< GPIO pin number assigned to the button
     uint32_t last_time;          ///< Timestamp of the last state change
     bool last_state;             ///< Last stable state of the button (true = released, false = pressed)
     uint32_t press_time;         ///< Timestamp when the button was first pressed
     uint32_t last_click_time;    ///< Timestamp of the last click event
     bool long_press_detected;    ///< Flag indicating if a long press has been detected
} ButtonState;
 
/*********************************| Variaveis globais |**************************/
 extern int test_Button;    // Test Variable

/*********************************| Enumeracao |*********************************/
 /**
  * @brief Enumeration for different types of button events.
  *
  * The enumeration defines possible button events detected:
  *  - IDLE: No event detected.
  *  - SINGLE_CLICK: A single click event.
  *  - DOUBLE_CLICK: A double click event.
  *  - LONG_PRESS: A long press event.
  */
 typedef enum {
     IDLE,
     SINGLE_CLICK,
     DOUBLE_CLICK,
     LONG_PRESS
 } ButtonEvent;
 
 extern volatile ButtonEvent event;
 /**
  * @brief Retrieves the button state structure based on the button name.
  *
  * This function returns a pointer to the ButtonState corresponding to the given button name.
  *
  * @param button_name Name of the button ("BUTTON_A" or "BUTTON_B").
  * @return Pointer to the corresponding ButtonState structure, or NULL if not found.
  */
 ButtonState* get_button_state(char* button_name);
 
 /**
  * @brief Initializes the specified button.
  *
  * This function initializes the GPIO for the specified button using its name and configures it as an input
  * with an internal pull-up resistor.
  *
  * @param button_name Name of the button ("BUTTON_A" or "BUTTON_B").
  */
 void init_button(char* button_name);
 
 /**
  * @brief Reads the button state with debounce.
  *
  * This function reads the current state of the button and applies a debounce algorithm. If the state change 
  * is stable for longer than the DEBOUNCE delay, it updates the button state.
  *
  * @param button_state Pointer to the ButtonState structure.
  * @return The stable state of the button (true = released, false = pressed).
  */
 bool debounce_button(ButtonState* button_state);
 
 /**
  * @brief Detects a long press on the button.
  *
  * This function checks if the button has been pressed continuously for a duration exceeding the specified 
  * duration in milliseconds. It resets the timing when the button is released.
  *
  * @param button_state Pointer to the ButtonState structure.
  * @param duration_ms Duration in milliseconds required to consider the press as long.
  * @return True if a long press is detected, otherwise false.
  */
 bool long_press_button(ButtonState* button_state, uint32_t duration_ms);
 
 /**
  * @brief Detects a double click event on the button.
  *
  * This function checks if two consecutive button presses occur within the specified interval.
  *
  * @param button_state Pointer to the ButtonState structure.
  * @param interval_ms Maximum interval in milliseconds between clicks for a double click.
  * @return True if a double click is detected, otherwise false.
  */
 bool double_click_button(ButtonState* button_state, uint32_t interval_ms);
 
 /**
  * @brief Initializes the button GPIO for interrupt handling.
  *
  * This function sets up the specified GPIO for both rising and falling edge interrupts and assigns
  * a callback function to handle these events.
  *
  * @param gpio GPIO pin number for the button.
  */
 void init_button_IT(uint gpio);
 
 /**
  * @brief Interrupt handler for button events.
  *
  * This callback function processes GPIO interrupts for button press (falling edge) and release 
  * (rising edge) and updates global timing and event variables accordingly.
  *
  * @param gpio GPIO pin number where the event occurred.
  * @param events Event flags indicating the type of edge (falling or rising).
  */
 void handle_button_IT(uint gpio, uint32_t events);
 
 /**
  * @brief Processes the current button state and triggers appropriate events.
  *
  * This function reviews the button press and release timings to determine whether a 
  * single click, double click, or long press event has occurred, and calls the provided event 
  * handler function with the detected event.
  *
  * @param event_handler Pointer to the event handler function that processes ButtonEvent.
  */
 void process_button_state(void (*event_handler)(ButtonEvent));
 
 /**
  * @brief Blinks an LED as a visual indicator for button events.
  *
  * This helper function toggles the LED state on the specified GPIO pin for a given number of times.
  *
  * @param gpio GPIO pin number where the LED is connected.
  * @param delay_ms Delay in milliseconds between toggling the LED.
  * @param times Number of times to blink the LED.
  */
 void blink_led(uint gpio, uint32_t delay_ms, int times);
 
 /**
  * @brief Example event function handling button events.
  *
  * This function provides a simple implementation of an event handler that reacts to different
  * button events by blinking LEDs on specified GPIO pins.
  *
  * @param event ButtonEvent value indicating the detected event (SINGLE_CLICK, DOUBLE_CLICK, or LONG_PRESS).
  */
 void event_function(ButtonEvent event);

 
 #endif // BUTTONS_LIB_H