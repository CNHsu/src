//----------------------------------------------
// BT- HC-42 test
// 2019/5/23
// by CNHsu
// arduino with 3 serial port
//  Serial: host
// BTSerial: HC-05 38400 , need \n\r and space
// BTSerial2:HC-42 9600, need not \n\r
// UART termainal set no \n\r
//------------------------------------------------
#include  <SoftwareSerial.h>  
SoftwareSerial BTSerial(10, 11); // RX | TX (BT 10-TX, 11-RX)
//SoftwareSerial BTSerial2(8, 9); // RX | TX
SoftwareSerial BTSerial2(5, 6); // RX | TX

  
int i=0;
int LED=13;


char ch;
int renew;

void setup() {
  // put your setup code here, to run once:
 // pinMode(LED,OUTPUT);
  Serial.begin(9600);
  Serial.write("BT HC-05 need NF&CR\n\r");
  Serial.write("BT HC-42 @ \n\r");
 BTSerial.begin(38400); //HC-05 BT default AT command mode speed
 BTSerial2.begin(9600); //HC-42 default 9600
  //BTSerial.begin(115200); //AT+UART change to this
  renew=0;
}

void loop() {
  // put your main code here, to run repeatedly:
String a,b;

  if(Serial.available())       // read UART  
  {
   a=Serial.readString();
   renew=1;
  }
    if(a[0]== '@') 
    {
     BTSerial2.print(a.substring(1));
    }
    else
    {
    if(a.length()>0 && renew==1)
     {
       a += "\n\r "; // 2019/5/28 very import, need space after \n\r,  "\n\r "
       BTSerial.print(a); // out to BT,, HC-05 need NL&NR
       renew=0;
     }
    }
 if(a[0]=='@')
   {
    BTSerial2.listen();
    delay(20);              //2019/5/28 very import, wait for available
    Serial.write("*@");
     while(BTSerial2.available())  // read BT
       { 
         delay(10);
         ch=BTSerial2.read();
         if(ch=='13') break;
         b += ch;
       }
        // Serial.write(BTSerial2.read()); // out to UART
        Serial.println(b);    
   }
  else
   { 
    BTSerial.listen();
     if(BTSerial.available())  // read BT
       {
         Serial.write(BTSerial.read()); // out to UART
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
