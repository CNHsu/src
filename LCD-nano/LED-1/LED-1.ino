/*
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
 * YWROBOT
 *Compatible with the Arduino IDE 1.0
 *Library version:1.1
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int ledPin =  12;      // the number of the LED pin
int ledState = LOW;
int long interval=1000; 
unsigned long previousMillis = 0;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//int show = -1;
void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  String instr="";
  // when characters arrive over the serial port...
   unsigned long currentMillis = millis();
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      instr=Serial.readString();
      lcd.print(instr);
      switch(instr[0]){
        case '1': 
                  lcd.print("DEC");
                  interval=interval-100;
                  lcd.print(interval);
                  break;
        case '2': 
                  lcd.print("INC");
                  interval=interval+100;
                  lcd.print(interval);
                  break;
      }
      //lcd.print("end");
       Serial.println(interval);
    }
  }
  if(interval <0) interval=0;
 // Serial.println(interval);
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
  }
  digitalWrite(ledPin, ledState);
}

/*
void loop()
{
  if (show == 0) {
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print("Hello LCD");
    delay(1000);

    lcd.setBacklight(0);
    delay(400);
    lcd.setBacklight(255);

  } else if (show == 1) {
    lcd.clear();
    lcd.print("Cursor On");
    lcd.cursor();

  } else if (show == 2) {
    lcd.clear();
    lcd.print("Cursor Blink");
    lcd.blink();

  } else if (show == 3) {
    lcd.clear();
    lcd.print("Cursor OFF");
    lcd.noBlink();
    lcd.noCursor();

  } else if (show == 4) {
    lcd.clear();
    lcd.print("Display Off");
    lcd.noDisplay();

  } else if (show == 5) {
    lcd.clear();
    lcd.print("Display On");
    lcd.display();

  } else if (show == 7) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** first line.");
    lcd.setCursor(0, 1);
    lcd.print("*** second line.");

  } else if (show == 8) {
    lcd.scrollDisplayLeft();
  } else if (show == 9) {
    lcd.scrollDisplayLeft();
  } else if (show == 10) {
    lcd.scrollDisplayLeft();
  } else if (show == 11) {
    lcd.scrollDisplayRight();

  } else if (show == 12) {
    lcd.clear();
    lcd.print("write-");

  } else if (show > 12) {
    lcd.print(show - 13);
  } // if

  delay(1400);
  show = (show + 1) % 16;
} // loop()
*/
