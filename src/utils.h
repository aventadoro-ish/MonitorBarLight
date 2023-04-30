#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

#include "Arduino.h"

// https://forum.arduino.cc/t/how-to-debug-code-in-arduino-ide/209670/9
#ifdef DEBUG_VERSION  // Macros are usually in all capital letters.
#define DPRINT(...) Serial.print(__VA_ARGS__)  // DPRINT is a macro, debug print
#define DPRINTLN(...) \
    Serial.println(   \
        __VA_ARGS__)  // DPRINTLN is a macro, debug print with new line
#else
#define DPRINT(...)    // now defines a blank line
#define DPRINTLN(...)  // now defines a blank line
#endif

#if defined(__AVR_ATtiny85__)
// Code for ATtiny85 Trinket 5V board
#include "TinyWireM.h"  // I2C Master lib for ATTinys which use USI
#define I2C TinyWireM

#define BUTTON_PIN 3  // PB3
#define PWM_PIN_0 4
#define PWM_PIN_1 1

#elif defined(__AVR_ATmega328P__)
#include "Wire.h"
#define I2C Wire

#define BUTTON_PIN 4  // PB3
#define PWM_PIN_0 5
#define PWM_PIN_1 6

#elif !defined(__AVR_ATtiny85__) && !defined(__AVR_ATmega328P__)
#error Wrong chip, it must be a ATtiny85 or ATmega328P
#endif

/* ==========================================================
 *                         Macros
 */

#define digitalToggle(pin) digitalWrite(pin, ~digitalRead(pin))
#define incDecValue(value, is_acs, step, max, min) {    \
    if (is_acs) {                                       \
        if (++value == max) {                           \
                is_acs = false;                         \
        }                                               \
    } else {                                            \
        if (--value == min) {                           \
                is_acs = true;                          \
        }                                               \
    }                                                   \
}

/* ==========================================================
 *                         Settings
 */
// Button
#define BUTTON_DEBOUNCE_MS 50
#define BUTTON_TIMEOUT_MS 1000

#define MENU_TIMEOUT_MS 5000

#define PWM_MIN_VALUE 0
#define PWM_MAX_VALUE 255
#define PWM_BLINK_MIN 50
#define PWM_BLINK_MAX 200

#define PWM_FADE_STEP_PERIOD_MS 10
#define PWM_SELECTED_BLINK_T_MS 250

// https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

#endif