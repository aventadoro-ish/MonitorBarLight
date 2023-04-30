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

enum PWMModes {
    CONSTANT,
    FADE_ABS,
    FADE_REL,
    FIRE
};

typedef struct PWM_Channel_Settings_s {
    uint8_t pwm_pin;
    uint8_t pwm_user_value;
    uint8_t pwm_value;
    PWMModes pwm_mode;
    uint32_t last_update_time;
    bool is_fade_asc;
    bool is_selected;
} PWM_Channel_Settings_t;

bool operator==(const PWM_Channel_Settings_s& lhs, const PWM_Channel_Settings_s& rhs) {
    bool eq_value = lhs.pwm_value == rhs.pwm_value;
    bool eq_mode  = lhs.pwm_mode == rhs.pwm_mode;
    bool eq_sel = lhs.is_selected == rhs.is_selected; 
    bool eq_pin = lhs.pwm_pin == rhs.pwm_pin; 

    return eq_value && eq_mode && eq_sel && eq_pin;
}


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

uint8_t correctGamma(uint8_t value) {
    return pgm_read_byte(&gamma8[value]);
}

void handle_pwm_channel(PWM_Channel_Settings_t *s) {
    if (s->is_selected) {
        if ((uint32_t)(millis() - s->last_update_time) > PWM_SELECTED_BLINK_T_MS) {
            
            s->pwm_value = s->pwm_value > PWM_BLINK_MIN ? PWM_BLINK_MIN : PWM_BLINK_MAX;
            analogWrite(s->pwm_pin, correctGamma(s->pwm_value));
            
            s->last_update_time = millis();

            
        }
        return;
    }

    // constant mode
    if (s->pwm_mode == CONSTANT) {
        analogWrite(s->pwm_pin, correctGamma(s->pwm_user_value));
        return;
    }

    // fade abs mode
    if (s->pwm_mode == FADE_ABS) {
        if ((uint32_t)(millis() - s->last_update_time) > PWM_FADE_STEP_PERIOD_MS) {
            incDecValue(s->pwm_value, s->is_fade_asc, 1, PWM_MAX_VALUE, PWM_MIN_VALUE);
            analogWrite(s->pwm_pin, correctGamma(s->pwm_value));
            s->last_update_time = millis();
        }
        return;
    }

    // fade rel mode
    if (s->pwm_mode == FADE_REL) {
        if ((uint32_t)(millis() - s->last_update_time) > PWM_FADE_STEP_PERIOD_MS) {
            incDecValue(s->pwm_value, s->is_fade_asc, 1, s->pwm_user_value, PWM_MIN_VALUE);
            analogWrite(s->pwm_pin, correctGamma(s->pwm_value));
            s->last_update_time = millis();
        }
        return;
    }


}


// ==== setup() ====================================================
void setup() {

    #ifdef DEBUG_VERSION
        Serial.begin(9600);
    #endif

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(PWM_PIN_0, OUTPUT);
    pinMode(PWM_PIN_1, OUTPUT);

    I2C.begin();  // initialize I2C lib

    DPRINTLN("Monitor Bar Light v1");

}


// ==== loop() =====================================================
void loop() {
    static MenuState menu_state = MENU_WAIT_FOR_INPUT;
    
    // TODO: init from EEPROM
    static PWM_Channel_Settings_t pwm_channels[] = {
        //          user_value      last_upd_time
        //          |  value        |         is_fade_asc  
        // pin      |  |  mode      |         |     is_selected
        {PWM_PIN_0, 0, 0, CONSTANT, millis(), true, false},
        {PWM_PIN_1, 0, 0, CONSTANT, millis(), true, false}
    };
    static uint8_t pwm_channel_select = 0;

    static uint32_t menu_timeout_start = 0;
    static uint32_t menu_some_time = 0;
    

    btn.tick();
    char debug_str[100];

    handle_pwm_channel(&pwm_channels[0]);
    handle_pwm_channel(&pwm_channels[1]);
    

    if (btn.state() == 0 &&
        (uint32_t)(millis() - menu_timeout_start) > MENU_TIMEOUT_MS &&
        menu_state != MENU_WAIT_FOR_INPUT) {

        pwm_channels[pwm_channel_select].is_selected = false;
        
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
        pwm_channels[pwm_channel_select].is_selected = false;
        pwm_channel_select = (pwm_channel_select + 1) % n_pwm_channels;
        pwm_channels[pwm_channel_select].is_selected = true;

        sprintf(debug_str, "pwm idx = %u", pwm_channel_select);        
        DPRINTLN(debug_str);
        
        menu_timeout_start = millis();
    }

    if (menu_state == MENU_CHANNEL_SELECT && btn.hold()) {
        pwm_channels[pwm_channel_select].is_selected = false;
        menu_state = MENU_VALUE_SELECT;
        DPRINTLN("MENU_VALUE_SELECT");
    }

    if (menu_state == MENU_VALUE_SELECT && btn.hold()) {
        if ((uint32_t)(millis() - pwm_channels[pwm_channel_select].last_update_time) > 10) {
            static bool isIncreasing = true;
            incDecValue((pwm_channels[pwm_channel_select].pwm_user_value),
                        isIncreasing, 1, PWM_MAX_VALUE, PWM_MIN_VALUE);
            
            sprintf(debug_str, "val[%u] = %u", pwm_channel_select,
                    pwm_channels[pwm_channel_select].pwm_user_value);
            DPRINTLN(debug_str);
            pwm_channels[pwm_channel_select].last_update_time = millis();
        }

        menu_timeout_start = millis();
    }

    if (menu_state == MENU_VALUE_SELECT && btn.hasClicks(1)) {
        DPRINTLN("NO menu");
        menu_state = MENU_WAIT_FOR_INPUT;
    }

    if (menu_state == MENU_CHANNEL_SELECT && btn.hasClicks(2)) {
        pwm_channels[pwm_channel_select].is_selected = false;
        menu_state = MENU_MODE_SELECT;
        DPRINTLN("MODE SELECT");
    }

    if (menu_state == MENU_MODE_SELECT && btn.click()) {
        pwm_channels[pwm_channel_select].pwm_mode =
            (pwm_channels[pwm_channel_select].pwm_mode + 1) % n_pwm_modes;
        sprintf(debug_str, "mode[%u] = %d", pwm_channel_select,
                pwm_channels[pwm_channel_select].pwm_mode);
        DPRINTLN(debug_str);

        menu_timeout_start = millis();
    }

    // if (btn.hasClicks(1)) lcd_print_screen("1 clicks");
    // if (btn.hasClicks(2)) lcd_print_screen("2 clicks");
    // if (btn.held()) lcd_print_screen("held");
}