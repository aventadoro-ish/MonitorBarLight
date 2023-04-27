#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdint.h>
// I2C TinyWire
#include "Arduino.h"



// #ifdef __AVR_ATtiny85__
// // Code for ATtiny85
//         #include "TinyWireM.h"                  // I2C Master lib for ATTinys which use USI
//         #define I2C TinyWireM
// #endif


// #ifndef __AVR_ATmega328P__
//         #include "Wire.h"
//         #define I2C Wire
// #endif



#if defined (__AVR_ATtiny85__)
// Code for ATtiny85
        #include "TinyWireM.h"                  // I2C Master lib for ATTinys which use USI
        #define I2C TinyWireM

#elif defined (__AVR_ATmega328P__) // false //defined(ARDUINO_AVR_NANO)       
        #define BOARD "Nano"

        #define SDA 18
        #define SCL 19

        #include "Wire.h"
        #define I2C Wire
        // #error Wrong chip, it must be a ATtiny85 or ATmega328P

// #else
// #error "Unknown board"

#endif

// #if !defined (__AVR_ATtiny85__) && !defined (__AVR_ATmega328P__)
//   #error Wrong chip, it must be a ATtiny85 or ATmega328P
// #endif



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