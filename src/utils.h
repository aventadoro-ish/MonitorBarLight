#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdint.h>
#include "Arduino.h"

#ifdef DEBUG_VERSION
        #define DEBUG true
#else
        #define DEBUG false
#endif

// https://forum.arduino.cc/t/how-to-debug-code-in-arduino-ide/209670/9
#ifdef DEBUG    //Macros are usually in all capital letters.
   #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
   #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
   #define DPRINT(...)     //now defines a blank line
   #define DPRINTLN(...)   //now defines a blank line
#endif


#if defined (__AVR_ATtiny85__)
        // Code for ATtiny85 Trinket 5V board
        #include "TinyWireM.h"                  // I2C Master lib for ATTinys which use USI
        #define I2C TinyWireM

        #define BUTTON_PIN 3 // PB3
        #define PWM_PIN_0 4
        #define PWM_PIN_1 1

#elif defined (__AVR_ATmega328P__)
        #include "Wire.h"
        #define I2C Wire

        #define BUTTON_PIN 4 // PB3
        #define PWM_PIN_0 5
        #define PWM_PIN_1 6

#elif !defined (__AVR_ATtiny85__) && !defined (__AVR_ATmega328P__)
  #error Wrong chip, it must be a ATtiny85 or ATmega328P
#endif



/* ==========================================================
 *                         Macros
 */

#define digitalToggle(pin) digitalWrite(pin, ~digitalRead(pin))


/* ==========================================================
 *                         Settings
 */
// Button
#define BUTTON_DEBOUNCE_MS  50
#define BUTTON_TIMEOUT_MS   1000

#define MENU_TIMEOUT_MS 5000

#define PWM_MIN_VALUE 0
#define PWM_MAX_VALUE 255


#endif