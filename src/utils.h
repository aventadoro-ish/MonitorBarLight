#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdint.h>
// I2C TinyWire
#include "Arduino.h"
#include "TinyWireM.h"                  // I2C Master lib for ATTinys which use USI


/* ==========================================================
 *                         Macros
 */

#define digitalToggle(pin) digitalWrite(pin, ~digitalRead(pin))

// Button
#define BUTTON_PIN 3 // PB3
#define BUTTON_DEBOUNCE_MS  50
#define BUTTON_TIMEOUT_MS   1000

#define MENU_TIMEOUT_MS 5000

#define PWM_PIN_0 4
#define PWM_PIN_1 1

#define PWM_MIN_VALUE 0
#define PWM_MAX_VALUE 255


#endif