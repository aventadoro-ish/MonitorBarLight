#ifndef _LCD_H_
#define _LCD_H_

#include "utils.h"

// LCD definifitons
#define LCD_I2C_ADDRESS 0x27
#define LCD_RS 0b00000001  // 0 -> instruction, 1 -> data
#define LCD_RW 0b00000010  // 0 -> write, 1 -> read
#define LCD_EN 0b00000100  // on rising edge 


void lcd_send_byte(uint8_t value, boolean isData);
void lcd_init();

void lcd_print_str(char* str);
void lcd_print_str(const char* str);    // for passing string literals

void lcd_clear();
void lcd_cursor_home();


void lcd_print_screen(char* str);
void lcd_print_screen(const char* str);


#endif