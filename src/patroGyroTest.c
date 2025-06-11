/**
 * @file patroGyroTest.c
 * @brief Implementation of project-specific LED feedback based on roll and pitch.
 *
 * This file implements utility functions for updating LED states based on roll and pitch values.
 *
 * @author Luis Felipe Patrocinio (https://github.com/luisfpatrocinio)
 */
#include "patroGyroTest.h"
#include "gyro.h"

/**
 * @brief Updates the LEDs based on roll and pitch values.
 *
 * This function turns off all LEDs and then lights up specific LEDs based on the
 * provided roll and pitch values, simulating a dice face or special conditions.
 *
 * @param roll The roll value mapped to an integer (e.g., -11, -5, 0, 5, 11).
 * @param pitch The pitch value mapped to an integer (e.g., -11, -5, 0, 5, 11).
 */
void updateLedsByRollAndPitch(int roll, int pitch)
{
    setAllLedsBrightness(0); // Desliga todos os LEDs

    if (roll == -MAX_ROLL)
    {
        setLedBrightness(LED_RED_PIN, 255);
    }

    if (roll == MAX_ROLL)
    {
        setLedBrightness(LED_RED_PIN, 255);
    }

    if (pitch == -MAX_ROLL)
    {
        setLedBrightness(LED_GREEN_PIN, 255);
    }

    if (pitch == MAX_PITCH)
    {
        setLedBrightness(LED_GREEN_PIN, 255);
    }

    if (pitch == 0 && roll == 0)
    {
        setLedBrightness(LED_BLUE_PIN, 255); // Liga o LED azul se ambos forem zero
    }

    if (pitch == -MAX_PITCH+1 || pitch == MAX_PITCH-1 || roll == -MAX_ROLL+1 || roll == MAX_ROLL-1)
    {
        setLedBrightness(LED_BLUE_PIN, 255);
        setLedBrightness(LED_RED_PIN, 255);
        setLedBrightness(LED_GREEN_PIN, 255);
    }
}