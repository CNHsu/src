#include <PDM.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.printf(" arduino nano 33 ble: PDM test\n\r");
  pinMode(13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf(" arduino nano 33 ble: PDM test\n\r");
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500); 
}
