//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
//https://www.wongwonggoods.com/arduino/arduino-i2c-lcd/
// 19-A5-SCL
// 18-A4-SDA
// 13-SCL 
////https://www.factoryforward.com/16x2-lcd-display-i2c-pcf8574-library/
// 16×2 LCD Display with I2C | PCF8574 Library

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!");
   lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
   lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");
  delay(5000);
  //lcd.noBacklight();
}


void loop()
{
}
