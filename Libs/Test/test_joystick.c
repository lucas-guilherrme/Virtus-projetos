#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "src/unity.h"
#include "../inc/joystick.h"

// Mock definitions for ADC and constants
#define ADC_PIN_X 26
#define ADC_PIN_Y 27
#define ADC_CHANEL_1 1
#define ADC_CHANEL_0 0
#define AVERAGE_VALUE 2048
#define DEADZONE_THRESHOLD 100
#define MOVING_AVG_WINDOW 5
#define LOWPASS_ALPHA 0.2

// Mock functions for ADC
void adc_init() {}
void adc_gpio_init(unsigned int pin) {}
void adc_select_input(unsigned int input) {}
uint16_t adc_read() {
    static uint16_t mock_value = 2048;
    return mock_value; // Return a constant value for testing
}

/**
 * @file test_joystick.c
 * @brief Unit test for the Joystick_Init function.
 *
 * This file contains a test function to verify the initialization of a joystick
 * state structure. The test ensures that the raw and filtered values for both
 * the x and y axes are correctly initialized to the same values.
 */

/**
 * @brief Test the Joystick_Init function.
 *
 * This test initializes a JoystickState structure using the Joystick_Init function
 * and verifies that the raw and filtered values for both the x and y axes are equal
 * after initialization.
 *
 * @note This test uses the TEST_ASSERT_EQUAL macro to perform the assertions.
 */
// Test for Joystick_Init
void test_Joystick_Init(void) {
    JoystickState js = {0};
    Joystick_Init(&js);

    TEST_ASSERT_EQUAL(js.x_raw, js.x_filtered);
    TEST_ASSERT_EQUAL(js.y_raw, js.y_filtered);
}

// Test for Joystick_ApplyDeadZone
void test_Joystick_ApplyDeadZone(void) {
    int16_t value = 2048;
    TEST_ASSERT_EQUAL(2048, Joystick_ApplyDeadZone(value));

    value = 2100;
    TEST_ASSERT_EQUAL(2100, Joystick_ApplyDeadZone(value));

    value = 2000;
    TEST_ASSERT_EQUAL(2000, Joystick_ApplyDeadZone(value));
}

// Test for Joystick_MovingAverageFilter
void test_Joystick_MovingAverageFilter(void) {
    int16_t buffer[MOVING_AVG_WINDOW] = {0};
    int16_t result = Joystick_MovingAverageFilter(2048, buffer, MOVING_AVG_WINDOW);

    TEST_ASSERT_EQUAL(2048 / MOVING_AVG_WINDOW, result);
}

// Test for Joystick_LowPassFilter
void test_Joystick_LowPassFilter(void) {
    int16_t new_sample = 2100;
    int16_t prev_value = 2000;
    int16_t result = Joystick_LowPassFilter(new_sample, prev_value);

    TEST_ASSERT_EQUAL((int16_t)(LOWPASS_ALPHA * new_sample + (1 - LOWPASS_ALPHA) * prev_value), result);
}

// Test for Joystick_MedianFilter
void test_Joystick_MedianFilter(void) {
    int16_t buffer[5] = {2000, 2100, 2048, 2200, 1900};
    int16_t result = Joystick_MedianFilter(buffer, 5);

    TEST_ASSERT_EQUAL(2048, result);
}

// Test for Joystick_ApplyFilters
void test_Joystick_ApplyFilters(void) {
    JoystickState js = {0};
    js.x_raw = 2100;
    js.y_raw = 2000;

    Joystick_ApplyFilters(&js);

    TEST_ASSERT_NOT_EQUAL(0, js.x_filtered); // Ensure filters are applied
    TEST_ASSERT_NOT_EQUAL(0, js.y_filtered);
}

// Additional test for edge cases in Joystick_ApplyDeadZone
void test_Joystick_ApplyDeadZone_EdgeCases(void) {
    int16_t value = AVERAGE_VALUE - DEADZONE_THRESHOLD;
    TEST_ASSERT_EQUAL(AVERAGE_VALUE, Joystick_ApplyDeadZone(value));

    value = AVERAGE_VALUE + DEADZONE_THRESHOLD;
    TEST_ASSERT_EQUAL(AVERAGE_VALUE, Joystick_ApplyDeadZone(value));

    value = AVERAGE_VALUE - DEADZONE_THRESHOLD - 1;
    TEST_ASSERT_EQUAL(value, Joystick_ApplyDeadZone(value));

    value = AVERAGE_VALUE + DEADZONE_THRESHOLD + 1;
    TEST_ASSERT_EQUAL(value, Joystick_ApplyDeadZone(value));
}

// Additional test for Joystick_MovingAverageFilter with varying inputs
void test_Joystick_MovingAverageFilter_VaryingInputs(void) {
    int16_t buffer[MOVING_AVG_WINDOW] = {0};
    Joystick_MovingAverageFilter(1000, buffer, MOVING_AVG_WINDOW);
    Joystick_MovingAverageFilter(2000, buffer, MOVING_AVG_WINDOW);
    Joystick_MovingAverageFilter(3000, buffer, MOVING_AVG_WINDOW);
    Joystick_MovingAverageFilter(4000, buffer, MOVING_AVG_WINDOW);
    int16_t result = Joystick_MovingAverageFilter(5000, buffer, MOVING_AVG_WINDOW);

    TEST_ASSERT_EQUAL((1000 + 2000 + 3000 + 4000 + 5000) / MOVING_AVG_WINDOW, result);
}

// Unity setup and teardown functions
void setUp(void) {}
void tearDown(void) {}

// Main function to run all tests
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_Joystick_Init);
    RUN_TEST(test_Joystick_ApplyDeadZone);
    RUN_TEST(test_Joystick_MovingAverageFilter);
    RUN_TEST(test_Joystick_LowPassFilter);
    RUN_TEST(test_Joystick_MedianFilter);
    RUN_TEST(test_Joystick_ApplyFilters);
    RUN_TEST(test_Joystick_ApplyDeadZone_EdgeCases);
    RUN_TEST(test_Joystick_MovingAverageFilter_VaryingInputs);

    return UNITY_END();
}



