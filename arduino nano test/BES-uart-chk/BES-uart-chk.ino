//----------------------------------------------
// BES uart check
// arduino with 3 serial port
//  Serial: host
// BSerial: BES 921600 , need \n\r and space
// ASerial: 9600, need not \n\r
// UART termainal set no \n\r
//------------------------------------------------
#include  <SoftwareSerial.h>  
SoftwareSerial BSerial(10, 11); // RX | TX , BES 921600 
//SoftwareSerial BTSerial2(8, 9); // RX | TX
SoftwareSerial ASerial(5, 6); // RX | TX, Arduino 9600

  
int i=0;
int LED=13;


char ch;
int renew;
int bb;

void setup() {
  // put your setup code here, to run once:
 // pinMode(LED,OUTPUT);
  Serial.begin(38400);
  Serial.write("BES and Arduino\n\r");
 BSerial.begin(921600); //HC-05 BT default AT command mode speed
 ASerial.begin(38400); //HC-42 default 9600, '@' for arduino command prefix
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
     ASerial.print(a.substring(1));
    }
    else
    {
    if(a.length()>0 && renew==1)
     {
       a += "\n\r "; // 2019/5/28 very import, need space after \n\r,  "\n\r "
       BSerial.print(a); // out to BES never use!!!
       renew=0;
     }
    }
 if(a[0]=='@')
   {
    ASerial.listen();
    delay(2000);      // need wait for slave response  
     while(ASerial.available())  // read BT
       {
         Serial.write(ASerial.read()); // out to UART
       }
   }
  else
   { 
   BSerial.listen();
    bb=BSerial.available();
     while(bb)  // read slave arduino
       { 
         //delay(10);
         Serial.println(bb);
         ch=BSerial.read();
         if(ch=='\n') break;
         b += ch;
         bb=BSerial.available();
       }       
        // Serial.write(BTSerial2.read()); // out to UART
       if(bb)  Serial.println(b);    
    } 

 // delay(1000); // wait for printing 

}


 void led_blink() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  }
