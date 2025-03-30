
#include "../Aplication/inc/button.h"

// Global button state variables.
ButtonState button_a_state = {BUTTON_A, 0, true, 0, 0};
ButtonState button_b_state = {BUTTON_B, 0, true, 0, 0};
ButtonState button_sw_state = {BUTTON_SW, 0, true, 0, 0};

volatile uint32_t last_press_time = 0;
volatile uint32_t last_release_time = 0;
volatile uint32_t first_release_time = 0;
volatile ButtonEvent event = IDLE;
volatile uint8_t click_counter = 0;

int test_Button = 0;

/* Returns the ButtonState pointer for a given button name. */
/**
 * @brief Retrieves the state of a specified button.
 *
 * This function takes the name of a button as input and returns a pointer
 * to the corresponding ButtonState structure. If the button name does not
 * match any known button, the function returns NULL.
 *
 * @param button_name A string representing the name of the button. 
 *                    Valid values are:
 *                    - "BUTTON_A"
 *                    - "BUTTON_B"
 *                    - "BUTTON_SW"
 *
 * @return A pointer to the ButtonState structure corresponding to the
 *         specified button, or NULL if the button name is invalid.
 */
ButtonState* get_button_state(char* button_name) {
    if (strcmp(button_name, "BUTTON_A") == 0) {
        return &button_a_state;
    }
    else if (strcmp(button_name, "BUTTON_B") == 0) {
        return &button_b_state;
    }
    else if (strcmp(button_name, "BUTTON_SW") == 0) {
        return &button_sw_state;
    }
    return NULL;
}

/* Initializes a button's GPIO (input with pull-up). */
void init_button(char* button_name) {
    ButtonState* button_state = get_button_state(button_name);
    if (button_state != NULL) {
        // Inicializa o GPIO do botão
        gpio_init(button_state->gpio);
        gpio_set_dir(button_state->gpio, GPIO_IN);
        gpio_pull_up(button_state->gpio);

        // Define o estado inicial
        button_state->last_state = gpio_get(button_state->gpio);
        button_state->last_time = to_ms_since_boot(get_absolute_time());
    }
}

/* Reads button state, applies debounce, and returns the stable state. */
bool debounce_button(ButtonState* button_state) {
    bool current_state = gpio_get(button_state->gpio);              // Read current state.
    uint32_t current_time = to_ms_since_boot(get_absolute_time());  // Get current time.

    // If the state has changed, update the last change time.
    if (current_state != button_state->last_state) {
        button_state->last_time = current_time;
    }

    // If the state is stable for longer than the debounce period, update the stable state.
    if ((current_time - button_state->last_time) > DEBOUNCE) {
        if (current_state != button_state->last_state) {
            button_state->last_state = current_state;  // Update stable state.
            return current_state;                      // Return new stable state.
        }
    }
    return button_state->last_state;  // Return current stable state.
}

/* Checks for a long press by timing the duration of the button press. */
bool long_press_button(ButtonState* button_state, uint32_t duration_ms) {
    // Inverted logic due to pull-up (pressed = false).
    bool button_state_value = !gpio_get(button_state->gpio);

    if (button_state_value) {
        if (button_state->press_time == 0) {
            // Record the time when the button is first pressed.
            button_state->press_time = to_ms_since_boot(get_absolute_time());
        } else if (to_ms_since_boot(get_absolute_time()) - button_state->press_time > duration_ms && 
                   !button_state->long_press_detected) {
            // Mark long press when duration threshold is exceeded.
            button_state->long_press_detected = true;
            return true;
        }
    } else {
        // Reset timing and detection flag when button is released.
        button_state->press_time = 0;
        button_state->long_press_detected = false;
    }
    return false;
}

/* Detects a double click by checking if two presses occur within a set interval. */
bool double_click_button(ButtonState* button_state, uint32_t interval_ms) {
    // Use inverted logic as button press gives false.
    bool button_state_value = !gpio_get(button_state->gpio);

    if (button_state_value) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        // If the time since the last click is within the double-click interval, confirm a double click.
        if ((current_time - button_state->last_click_time) <= interval_ms) {
            button_state->last_click_time = 0; // Reset click time.
            return true;
        }
        // Store current click time.
        button_state->last_click_time = current_time;
    }
    return false;
}

/* Configures a GPIO for button interrupt handling. */
void init_button_IT(uint gpio) {
    gpio_init(gpio);                                  // Initialize GPIO pin.
    gpio_set_dir(gpio, GPIO_IN);                        // Set as input.
    gpio_pull_up(gpio);                                 // Enable pull-up resistor.
    // Enable both falling and rising edge interrupts with callback.
    gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, handle_button_IT);
}

/* Interrupt handler for processing button press and release events. */
void handle_button_IT(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());  // Get current time.

    if (events & GPIO_IRQ_EDGE_FALL) { // Falling edge: button press.
        last_press_time = current_time;  // Record press time.
        if (last_press_time - last_release_time <= DEBOUNCE) {
            return; // Ignore if within debounce period.
        }
    } else if (events & GPIO_IRQ_EDGE_RISE) {
        current_time = to_ms_since_boot(get_absolute_time());// Rising edge: button release.
        uint32_t press_duration = current_time - last_press_time; // Calculate press duration.
        if (press_duration <= DEBOUNCE) {
            return; // Ignore if press too short.
        } else if (press_duration >= LONG_PRESS_TIME_MS) {
            event = LONG_PRESS;  // Set event to long press if duration exceeds threshold.
            click_counter = 0;
        } else {
            click_counter++;                             // Increment click counter for potential double click.
            first_release_time = last_release_time;        // Record time of first release.
            last_release_time = current_time;              // Update last release time.
        }
    }
}

/* Processes button events and calls the provided event handler. */
void process_button_state(void (*event_handler)(ButtonEvent)) {
    uint32_t btwn_edge_time = last_press_time - first_release_time; // Time between press and first release.
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    // Check for double click event.
    if (click_counter == 2 && (btwn_edge_time > DEBOUNCE) && (btwn_edge_time <= DOUBLE_CLICK_TIME_MS)) {
         event = DOUBLE_CLICK;
         click_counter = 0;
    } else if (click_counter == 1 && current_time - last_release_time > DOUBLE_CLICK_TIME_MS) {
         // Consider it a single click if no second press occurs in time.
         event = SINGLE_CLICK;
         click_counter = 0;
    }
    
    // If an event is set, call the event handler and reset the event.
    if (event != IDLE) {
        event_handler(event);
    }
}

/* Toggles an LED on a given GPIO a specified number of times with delays. */
void blink_led(uint gpio, uint32_t delay_ms, int times) {
    for (int i = 0; i < times; i++) {
        gpio_put(gpio, true);    // Turn LED on.
        sleep_ms(delay_ms);      // Wait for the delay.
        gpio_put(gpio, false);   // Turn LED off.
        sleep_ms(delay_ms);      // Wait for the delay.
    }
}

/* Example event handler that blinks different LEDs based on the button event. */
void event_function(ButtonEvent event) {

    if(test_Button) {
        if (event == SINGLE_CLICK) {
            blink_led(12, 100, 5);  // Blink LED on GPIO 12 for a single click.
        } else if (event == DOUBLE_CLICK) {
            blink_led(11, 100, 5);  // Blink LED on GPIO 11 for a double click.
        } else if (event == LONG_PRESS) {
            blink_led(13, 100, 5);  // Blink LED on GPIO 13 for a long press.
        }else{
            return;
        }
    }
}