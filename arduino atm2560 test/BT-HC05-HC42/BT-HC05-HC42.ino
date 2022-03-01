//----------------------------------------------
// BT- HC-42 test
// 2019/9/4
// by CNHsu
// atm 2560 
//  Serial: host
// Serial1: HC-05 38400 , need \n\r and space
// Serial2:HC-42 9600, need not \n\r
// UART termainal set no \n\r
//------------------------------------------------
/* arduino atm256 use hw serial!!
#include  <SoftwareSerial.h>  
SoftwareSerial Serial1(10, 11); // RX | TX (BT 10-TX, 11-RX)
SoftwareSerial Serial2(5, 6); // RX | TX
*/
  
int i=0;
int LED=13;


char ch;
int renew;

void setup() {
  // put your setup code here, to run once:
  //pinMode(LED,OUTPUT);
  Serial.begin(9600);
  Serial.write("BT HC-05 need NF&CR\n\r");
  Serial.write("BT HC-42 @ \n\r");
 Serial1.begin(38400); //HC-05 BT default AT command mode speed
 Serial2.begin(9600); //HC-42 default 9600
  //Serial1.begin(115200); //AT+UART change to this
  renew=0;
}

void loop() {
  // put your main code here, to run repeatedly:
String a,b;
//led_blink();

  if(Serial.available())       // read UART  
  {
   a=Serial.readString();
   renew=1;
  }
    if(a[0]== '@') 
    {
     Serial2.print(a.substring(1));
    }
    else
    {
    if(a.length()>0 && renew==1)
     {
       a += "\n\r "; // 2019/5/28 very import, need space after \n\r,  "\n\r "
       Serial1.print(a); // out to BT,, HC-05 need NL&NR
       renew=0;
     }
    }
 if(a[0]=='@')
   {
//    Serial2.listen();
    delay(20);              //2019/5/28 very import, wait for available
    Serial.write("*@");
     while(Serial2.available())  // read BT
       { 
         delay(10);
         ch=Serial2.read();
         if(ch=='13') break;
         b += ch;
       }
        // Serial.write(Serial2.read()); // out to UART
        Serial.println(b);    
   }
  else
   { 
//    Serial1.listen();
     if(Serial1.available())  // read BT
       {
         Serial.write(Serial1.read()); // out to UART
       }
    } 

 // delay(1000); // wait for printing 

}


 void led_blink() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  }
