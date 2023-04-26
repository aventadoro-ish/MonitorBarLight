#include "lcd.h"

void lcd_send_byte(uint8_t value, boolean isData) {
  /* 
   * send upper nibble, then lower
   * toggle LCD_EN line
   */

  byte upperNibble = value & 0xF0;
  byte lowerNibble = (value << 4) & 0xF0;
  byte registerSelet = isData ? LCD_RS : 0;
  registerSelet |= 1 << 3; // backlight permanently enabled
  
  TinyWireM.beginTransmission(LCD_I2C_ADDRESS);
  // send upper nibble first
  TinyWireM.write(upperNibble | registerSelet);          // setup data
  TinyWireM.write(upperNibble | registerSelet | LCD_EN); // toggle EN line
  TinyWireM.write(upperNibble | registerSelet);          // 
  
  delay(1);
  
  // then send lower nibble
  TinyWireM.write(lowerNibble | registerSelet);          // setup data
  TinyWireM.write(lowerNibble | registerSelet | LCD_EN); // toggle EN line
  TinyWireM.write(lowerNibble | registerSelet);
  
  TinyWireM.endTransmission();
}

void lcd_init() {
  // delay(15); // power-on dealy
  lcd_send_byte(0b00000010, false); // use nibble method to control lcd
  delay(20);
  
  lcd_send_byte(0b00101000, false); // init lcd (function set instruction)
  lcd_send_byte(0b00000101, false); // enable display
  lcd_send_byte(0b00000001, false); // clear display
  lcd_send_byte(0b00000110, false); // left to right
}

void lcd_print_str(char* str) {
  while (*str != 0) {
    lcd_send_byte(*str, true);
    str++;
  }
}

void lcd_print_str(const char* str) {
  while (*str != 0) {
    lcd_send_byte(*str, true);
    str++;
  }
}


void lcd_print_screen(char* str) {
  lcd_clear();
  lcd_cursor_home();
  lcd_print_str(str); 
}

void lcd_print_screen(const char* str) {
  lcd_clear();
  lcd_cursor_home();
  lcd_print_str(str); 
}

void lcd_clear() {
  lcd_send_byte(0b00000001, false); // clear display
  delay(5);
}

void lcd_cursor_home() {
  lcd_send_byte(0b00000010, false); // home cursor prob?
  delay(5);
}

