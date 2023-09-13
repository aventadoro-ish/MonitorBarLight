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
// DO NOT REASSIGN PWM PINS for ATtiny85!
// Only PB1 and PB4 can be used for the implemented analogWriteATtiny(..)
// 12 bit dual channel DAC code from: http://www.technoblogy.com/show?1NGL
#define PWM_PIN_0 0   // Pin 4
#define PWM_PIN_1 1   // Pin 1

#define PWM_MIN_VALUE 3
#define PWM_MAX_VALUE 255

#define PWM_FADE_STEP_PERIOD_MS 2

const uint16_t PROGMEM gamma[] = {
  0,    0,    0,    0,    1,    1,    2,    3,    4,    6,    8,   10,   13,   16,   19,   24,
  28,   33,   39,   46,   53,   60,   69,   78,   88,   98,  110,  122,  135,  149,  164,  179,
  196,  214,  232,  252,  273,  295,  317,  341,  366,  393,  420,  449,  478,  510,  542,  575,
  610,  647,  684,  723,  764,  806,  849,  894,  940,  988, 1037, 1088, 1140, 1194, 1250, 1307,
  1366, 1427, 1489, 1553, 1619, 1686, 1756, 1827, 1900, 1975, 2051, 2130, 2210, 2293, 2377, 2463,
  2552, 2642, 2734, 2829, 2925, 3024, 3124, 3227, 3332, 3439, 3548, 3660, 3774, 3890, 4008, 4128,
  4251, 4376, 4504, 4634, 4766, 4901, 5038, 5177, 5319, 5464, 5611, 5760, 5912, 6067, 6224, 6384,
  6546, 6711, 6879, 7049, 7222, 7397, 7576, 7757, 7941, 8128, 8317, 8509, 8704, 8902, 9103, 9307,
  9514, 9723, 9936, 10151, 10370, 10591, 10816, 11043, 11274, 11507, 11744, 11984, 12227, 12473, 12722, 12975,
  13230, 13489, 13751, 14017, 14285, 14557, 14833, 15111, 15393, 15678, 15967, 16259, 16554, 16853, 17155, 17461,
  17770, 18083, 18399, 18719, 19042, 19369, 19700, 20034, 20372, 20713, 21058, 21407, 21759, 22115, 22475, 22838,
  23206, 23577, 23952, 24330, 24713, 25099, 25489, 25884, 26282, 26683, 27089, 27499, 27913, 28330, 28752, 29178,
  29608, 30041, 30479, 30921, 31367, 31818, 32272, 32730, 33193, 33660, 34131, 34606, 35085, 35569, 36057, 36549,
  37046, 37547, 38052, 38561, 39075, 39593, 40116, 40643, 41175, 41711, 42251, 42796, 43346, 43899, 44458, 45021,
  45588, 46161, 46737, 47319, 47905, 48495, 49091, 49691, 50295, 50905, 51519, 52138, 52761, 53390, 54023, 54661,
  55303, 55951, 56604, 57261, 57923, 58590, 59262, 59939, 60621, 61308, 62000, 62697, 63399, 64106, 64818, 65535
};

#elif defined(__AVR_ATmega328P__)
#include "Wire.h"
#define I2C Wire

#define BUTTON_PIN 4  // PB3
#define PWM_PIN_0 5
#define PWM_PIN_1 6

#define PWM_MIN_VALUE 27
#define PWM_MAX_VALUE 255

#define PWM_FADE_STEP_PERIOD_MS 10

// https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
const uint8_t PROGMEM gamma[] = {
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

#define PWM_BLINK_MIN 50
#define PWM_BLINK_MAX 200

#define PWM_SELECTED_BLINK_T_MS 250


#endif