//----------------------------------------------
// BT- HC-42 test
// 2019/5/23
// by CNHsu
//
//------------------------------------------------
#include  <SoftwareSerial.h>  
SoftwareSerial BTSerial(10, 11); // RX | TX  
int i=0;
int LED=13;


char ch;


void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  Serial.begin(38400);
  Serial.write("BT HC-42 test code\n\r");
 BTSerial.begin(9600); //BT default AT command mode speed
  //BTSerial.begin(115200); //AT+UART change to this
}

void loop() {
  // put your main code here, to run repeatedly:
String a;
 
  if(BTSerial.available())  // read BT
  {
    Serial.write(BTSerial.read()); // out to UART
  }

  if(Serial.available())       // read UART  
  {
   a=Serial.readString();
  }
    BTSerial.print(a); // out to BT
  //delay(1000); // wait for printing 

}


 void led_blink() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  }
