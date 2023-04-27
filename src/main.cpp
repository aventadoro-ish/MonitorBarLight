/*
 * Firmware for my monitor bar light
 */

#include <EncButton2.h>

#include "utils.h"
#include "lcd.h"

// Опциональные дефайн-настройки (показаны по умолчанию)
#define EB_DEB 50     // дебаунс кнопки, мс
#define EB_CLICK 100  // таймаут накликивания, мс

const uint8_t pwm_pins[] = {PWM_PIN_0, PWM_PIN_1};
const uint8_t n_pwm_channels = sizeof(pwm_pins) / sizeof(uint8_t);
const uint8_t n_pwm_modes = 4;  // TODO: change to enum PWMModes

enum ButtonState {
    BTN_NO_PRESS,
    BTN_SINGLE_PRESS_DETECT,
    BTN_SINGLE_PRESS_SINGLE_RELEASE
};

enum ButtonEvent {
    BTN_NO_EVENT,
    BTN_LONG_PRESS,
    BTN_SHORT_PRESS,
    BTN_DOUBLE_PRESS
};

enum MenuState {
    MENU_WAIT_FOR_INPUT,  // goes here after timeout
    MENU_CHANNEL_SELECT,  // goes here from MENU_WAIT_FOR_INPUT after a press
    MENU_VALUE_SELECT,
    MENU_MODE_SELECT,
};

EncButton2<EB_BTN> btn(INPUT_PULLUP, BUTTON_PIN);

// ==== functions ==================================================
void blink(uint8_t pin, uint8_t n, uint32_t period_ms) {
    for (int i = 0; i < n; i++) {
        digitalToggle(pin);
        delay(period_ms);
    }
}

void blink_forever(uint8_t pin, uint32_t period_ms) {
    for (;;) {
        digitalToggle(pin);
        delay(period_ms);
    }
}

// ==== setup() ====================================================
void setup() {

    #ifdef DEBUG
        Serial.begin(9600);
    #endif

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(PWM_PIN_0, OUTPUT);
    pinMode(PWM_PIN_1, OUTPUT);

    // blink(PWM_PIN_0, 10, 200);
    // blink(PWM_PIN_1, 10, 200);
    

    I2C.begin();  // initialize I2C lib

    DPRINTLN("Hello there!");

}



// ==== loop() =====================================================
static MenuState menu_state = MENU_WAIT_FOR_INPUT;

// TODO: init from EEPROM
static uint32_t pwm_values[n_pwm_channels] = {0, 0};
static uint8_t pwm_modes[n_pwm_channels] = {0, 0};
static uint8_t pwm_channel_select = 0;

static uint32_t menu_timeout_start = 0;

void loop() {

    // static uint32_t menu_increase_delay = 0;
    

    btn.tick();
    char debug_str[100];

    if (btn.state() == 0 &&
        (uint32_t)(millis() - menu_timeout_start) > MENU_TIMEOUT_MS &&
        menu_state != MENU_WAIT_FOR_INPUT) {
        
        DPRINTLN("NO menu");
        menu_state = MENU_WAIT_FOR_INPUT;
        // menu_timeout_start = millis();
    }

    if (menu_state == MENU_WAIT_FOR_INPUT && btn.press()) {
        
        DPRINTLN("MENU_CHANNEL_SELECT");
        // if (menu_state == MENU_WAIT_FOR_INPUT && btn.click()) {

        menu_state = MENU_CHANNEL_SELECT;
        menu_timeout_start = millis();
    }

    // if (menu_state == MENU_CHANNEL_SELECT && btn.click()) {
    if (menu_state == MENU_CHANNEL_SELECT && btn.hasClicks(1)) {

        pwm_channel_select = (pwm_channel_select + 1) % n_pwm_channels;

        sprintf(debug_str, "pwm idx = %u", pwm_channel_select);        
        DPRINTLN(debug_str);
        
        menu_timeout_start = millis();
    }

    if (menu_state == MENU_CHANNEL_SELECT && btn.hold()) {
        menu_state = MENU_VALUE_SELECT;
        DPRINTLN("MENU_VALUE_SELECT");
    }

    if (menu_state == MENU_VALUE_SELECT && btn.hold()) {
        static bool isIncreasing = true;
        if (isIncreasing) {
            if (++pwm_values[pwm_channel_select] == PWM_MAX_VALUE) {
                isIncreasing = false;
            }
        } else {
            if (--pwm_values[pwm_channel_select] == PWM_MIN_VALUE) {
                isIncreasing = true;
            }
        }
        

        sprintf(debug_str, "val[%u] = %lu", pwm_channel_select,
                pwm_values[pwm_channel_select]);
        DPRINTLN(debug_str);

        analogWrite(pwm_pins[pwm_channel_select],
                    pwm_values[pwm_channel_select]);
        delay(5);
        menu_timeout_start = millis();
    }

    if (menu_state == MENU_CHANNEL_SELECT && btn.hasClicks(2)) {
        menu_state = MENU_MODE_SELECT;
        DPRINTLN("MODE SELECT");
    }

    if (menu_state == MENU_MODE_SELECT && btn.click()) {
        pwm_modes[pwm_channel_select] =
            (pwm_modes[pwm_channel_select] + 1) % n_pwm_modes;
        sprintf(debug_str, "mode[%u] = %d", pwm_channel_select,
                pwm_modes[pwm_channel_select]);
        DPRINTLN(debug_str);

        menu_timeout_start = millis();
    }

    // if (btn.hasClicks(1)) lcd_print_screen("1 clicks");
    // if (btn.hasClicks(2)) lcd_print_screen("2 clicks");
    // if (btn.held()) lcd_print_screen("held");
}