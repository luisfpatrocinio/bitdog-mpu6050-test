/**
 * @file patroGyroTest.h
 * @brief Project-specific utility functions for LED feedback based on roll and pitch.
 *
 * This header declares functions for updating LED states based on roll and pitch values.
 *
 * @author Luis Felipe Patrocinio (https://github.com/luisfpatrocinio)
 */
#ifndef PATRO_GYRO_TEST_H
#define PATRO_GYRO_TEST_H

#include "led.h" ///< For LED control functions and pin definitions

/**
 * @brief Updates the LEDs based on roll and pitch values.
 *
 * This function turns off all LEDs and then lights up specific LEDs based on the
 * provided roll and pitch values, simulating a dice face or special conditions.
 *
 * @param roll The roll value mapped to an integer (e.g., -11, -5, 0, 5, 11).
 * @param pitch The pitch value mapped to an integer (e.g., -11, -5, 0, 5, 11).
 */
void updateLedsByRollAndPitch(int roll, int pitch);

#endif // PATRO_GYRO_TEST_H