//####################################################################
// for arduino atm2560, 2019/09/17
//#################################################################### 
//2019/3/5 arduino nano v3 for BES test tool
// 2019/3/12 revised some io definition
// 2019/3/27 modified V/I output value after real meeter check
// 2019/4/29 Q1,Q3 modify from nmos to pmos.... reverse uA_SET
// 2019/5/28 include HC-05 BT module
// 2019/7/26 _BT_3 and _BT_2 only different is BT_3 has reset value before measure
// 2019/8/2 uart to 115200
// $$$  2019/8/19 try atm2560
// uart terminal should set "no NL&CR", HC-05 should set "at+role=1" 
// HC-05 module need pre-setting
// AT+ROLE=1
// AT+INQM=1,5,48
// AT+CLASS=240404
// 2019/6/6 update command set for HC-05
// control HC-05 PIO2-8 & PIO11
// "PIO=2,1" // port-2 hight,
// "PIO=7,0" // port-7 low
// will MPIO show settting status
// $$ EEPROM: atm2560 EEPROM size 4KB
// 20190910: BES FW set uart to 115200
// Serial: 0 (RX) and 1 (TX); Serial 1: 19 (RX) and 18 (TX); Serial 2: 17 (RX) and 16 (TX); Serial 3: 15(RX) and 14 (TX)
//
//  by CNHsu
//-----------------------------------------------------------
#include  <SoftwareSerial.h>  // atm256 use hw serial & software 
#include <EEPROM.h>
#include <PWM.h>
#include <arduinoFFT.h>
//#include <Stepper.h>
#include <Servo.h>

#define STEPS 100
///////////////////////////////////////////////////////////////////////////
// mega2560 softwareserial only following can be used for RX: 
// 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, 
// A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69).

SoftwareSerial Serial4(50, 48); // RX | TX, atm256 don't6 use software serial
SoftwareSerial Serial5(51, 49); // RX | TX, atm256 don't6 use software serial

///////////////////////////////////////////////////////////////////////////
// stepper
// int1-4:    (int1, int3, int2, int4)
// total 4096 steps  (gear ratio=64, 5.625deg, total 4096 steps)
//Stepper motor(STEPS, 31, 35, 33, 37);
///////////////////////////////////////////////////////////////////////////
// servo
Servo servo[16];
int up[16],down[16],sit[16]; // define servo up/dow/sit position
int deg;
char spos;
int step_speed;
int step_step;



int LED13=13; // LED on board
int pwm=11; // pwm 1k
int pwm2=9; //pwm2

int reset1=44; // nano reset 'low' reset
int reset2=45;
int reset3=46;
int reset4=47;

int fftinputL=A9; // A9 for fft-L
int fftinputR=A10; // A10 for fft-R

int val=0;
char *endptr;
char aRssi[10];
int cnt;

int eeaddr =0;
byte eev;

int pcbch; // pcb channel 0-3 mapping to serial2,3,4,5
unsigned long TIME_OUT=7000;
boolean newData= false;
const byte numChars =64;
char receivedChars[numChars];
//#######################################
// PWM
int32_t frequency =1000; //freq. Hz
int32_t frequency2 =500; //
//########################################
// fft
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
const uint16_t samples = 64; //This value MUST ALWAYS be a power of 2
const double signalFrequency = 1000;
const double samplingFrequency = 9600; // arduino 16Mhz ADC sample rate
//const double  amplitude = 1;
/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03



void setup() {
  // put your setup code here, to run once:
  //PWM
  InitTimersSafe();
  SetPinFrequencySafe(pwm, frequency);
   SetPinFrequencySafe(pwm2, frequency2);
  
  Serial.begin(115200); // to PC
  Serial.write("3S PWR/BT tester atm2560 v5\n\r");
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
//  pinMode(5,OUTPUT);
//  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  //pinMode(10,INPUT); // for fft input
  pinMode(11,OUTPUT);
  //pinMode(12,INPUT);
  pinMode(13,OUTPUT); //
  Serial1.begin(38400); //HC-05 BT default AT command mode speed, 19Rx, 18Tx
  Serial2.begin(57600); // to nano pcba1
  Serial3.begin(57600); // to nano pcba2
  Serial4.begin(57600); // to nano pcba3
  Serial5.begin(57600); // to nano pcba4

//  pinMode(44,OUTPUT);
//  pinMode(45,OUTPUT);
//  pinMode(46,OUTPUT);
//  pinMode(47,OUTPUT);
// define servo
  for (cnt=0; cnt < 16; cnt++){
    servo[cnt].attach(cnt+22); // servo start from D22 to D37
    servo[cnt].write(90); // reset all servo to default degree
  }
 for (cnt=0; cnt < 16; cnt++){ // set servo default position value
      up[cnt]=90;
      down[cnt]=10;
      sit[cnt]=90;
  }

  reset_nano(); // initial some IO setting
  pcbch=1; // default pcbch=0
  step_speed=90; // default speed
  step_step=2048; // default


}

void loop() {
  // put your main code here, to run repeatedly:
 int k;
 char i,ch;
 String btrx,instr;
 String Rssi;
 char char_array[10];

 pwmWrite(pwm,128);
 pwmWrite(pwm2,128);
 
 /*
 instr="";
             if(Serial3.available()) {
                ch=Serial3.read();
                Serial2.write(ch);
               }       
 */
/*
eev = EEPROM.read(eeaddr);
Serial.print(" ");
Serial.print(eev,HEX);
i=eeaddr & 0x0f;
if( i == 0x08) Serial.println();


eeaddr = eeaddr+1;
if(eeaddr ==EEPROM.length()){
  eeaddr=0;
}
*/
  instr="";

 // /*
 while (!Serial.available()) {} // wait RX data input
 instr=Serial.readString(); 
// Serial.println(instr);  // debug message
// Serial.println(String(instr.length())); // debug message
 if(instr.length() == 3){ // 20190917 length include \n\r? 
 i=instr[0];
  switch (i){
   case 'Q': break;
   case '1': case '2':  case '3': case '4': case '5': case '6': case '7': case '8': case '9':
   case 'a': case 'b':  case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 's': case 'x':
   case 'z':Serial.print("call slave..");
             //digitalWrite(VBUS_EN,HIGH);             
             pcbwrite(i);
             break;
  /* case 'z':  Serial.print("check key...");
              for(k=0; k<5; k++) 
               {
               // delay(1000);
                Serial.print(k);               
                pcbwrite(i);      
               }
               break;   
   */            
   case 'n'  :Serial.print("**Reset nano 0.5sec\n\r");
              reset_nano();
              break;
   case 'u'  :Serial.print("**LED1-BT INIT\n\r");
              Serial.write("AT+RESET\n\r ");
              Serial1.print("AT+RESET\n\r ");
            delay(1000);
            while(Serial1.available())  // read BT wait 'ok'
               {                 
                 delay(10);
                  ch=Serial1.read();
                  if(ch=='\r') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART
                btrx= "";                              
              Serial.write("AT+INIT\n\r ");
              Serial1.print("AT+INIT\n\r ");
            delay(1000);
             while(Serial1.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=Serial1.read();
                  if(ch=='\r') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART   
                btrx= ""; 
              Serial.write("AT+state\n\r ");
              Serial1.print("AT+state\n\r ");
             delay(500);
             while(Serial1.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=Serial1.read();
                  if(ch=='\r') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART
                btrx= "";  
             while(Serial1.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=Serial1.read();
                  if(ch=='\r') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART
                btrx="";
             Serial.write("BT INQM\n\r ");
             Serial1.print("AT+INQM=1,1,3\n\r ");
             delay(500);
             while(Serial1.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=Serial1.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART
                btrx="";  
             Serial.write("BT class\n\r ");
             Serial1.print("AT+class=240404\n\r ");
             delay(500);
             while(Serial1.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=Serial1.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART   
             break ;  
   case 'v'  :Serial.print("**LED1-BT rx: scan\n\r"); // BT initial
             Serial.write("BT scan\n\r ");
             cnt=0;
             while(cnt < 10){ // !!!!!!! endless loop
              cnt++;
              btrx="";
              Rssi="";
             Serial1.print("AT+INQ\n\r ");
             delay(1000);
             while(Serial1.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=Serial1.read();
                  if(ch=='\r') break;
                  btrx += ch;                  
               }               
               // Serial.println(btrx); // out to UART
                val=btrx.length();
             //   Serial.println(String(val));
                if(val > 28) {
                  Serial.println(btrx); // out to UART
                   Rssi=btrx.substring(val-4,val);
                   Rssi.toCharArray(aRssi,10);                   
                   val= strtoul(aRssi,&endptr,16);
                   Serial.println(Rssi); // out to UART
                   Serial.println("Rssi="+String(val));
                }
                Serial.println("==>"+String(cnt));
               // delay(1000);           
             }
             Serial.println("---end scan ---\n\r");
             break ; 
   case 'w'  :Serial.print("**BT INQM\n\r"); // BT INq and search
              Serial.write("AT inqm\n\r ");
              Serial1.print("AT+INQM\n\r ");// inq one device time out 12sec
//              Serial1.listen();
             delay(200);
             while(Serial1.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=Serial1.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART       
             break ; 
            
  }
 }
 else {
   if (instr[0]=='p') {
    btrx="";
    btrx="AT+"+instr+"\n\r ";
    Serial.print("**HC-05 PIO\n\r"); // BT INq and search
    Serial.print(btrx);
    Serial1.print(btrx);// inq one device time out 12sec
    btrx="";
             delay(200);
             while(Serial1.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=Serial1.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART       
    Serial1.print("AT+MPIO\n\r ");// inq one device time out 12sec
    btrx="";
             delay(200);
             while(Serial1.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=Serial1.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART         
      }
      else if (instr[0]=='#') {
       i=instr[1];
       switch (i){
        case '1': Serial.println("select pcb1(serial2)");                
                      pcbch=1;
                      break;
        case '2': Serial.println("select pcb2(serial3)");
                      pcbch=2;
                      break;                        
        case '3': Serial.println("select pcb3(serial4)");
                      pcbch=3;
                      break;
        case '4': Serial.println("select pcb4(serial5)");
                      pcbch=4;
                      break;   
       }
      }
       else if (instr[0]=='%'){
          i=instr[1];
          btrx=instr.substring(2);
          switch(i){
            case 's': Serial.println("reset servo 'sit' "+btrx);
                      deg = btrx.toInt();
                      if(deg >=1 && deg <=175)
                      {
                       for(cnt=0;cnt<16;cnt++) {
                        sit[cnt]=deg;                      
                       }
                      }
                      break;
            case 'u': Serial.println("reset servo 'up' "+btrx);
                      deg = btrx.toInt();
                      if(deg >=1 && deg <=175)
                      {
                       for(cnt=0;cnt<16;cnt++) {
                        up[cnt]=deg;                      
                       }
                      }
                      break;
            case 'd': Serial.println("reset servo 'down' "+btrx);
                      deg = btrx.toInt();
                      if(deg >=1 && deg <=175)
                      {
                       for(cnt=0;cnt<16;cnt++) {
                        down[cnt]=deg;                      
                       }
                      }
                      break;
            case 'i': //Serial.println("reset servo 'down' "+btrx);
                      Rssi="";
                      if( btrx[0] == 's' || btrx[0] == 'u'|| btrx[0] == 'd') {
                        spos=btrx[0];
                        Rssi=btrx.substring(1,3);
                        cnt=Rssi.toInt();                        
                       // Serial.print(spos);
                       if(cnt >15) {Serial.println("servo# out of range[0-15]"); break;}
                       Rssi=btrx.substring(3);
                      deg = Rssi.toInt();
                      if(deg >=1 && deg <=175)
                      {
                         servo[cnt].write(deg);
                         switch(spos){
                          case 's': sit[cnt]=deg;
                                    Serial.print("sit ");
                                    break;
                          case 'u': up[cnt]=deg;
                                    Serial.print("up ");
                                    break;
                          case 'd': down[cnt]=deg;
                                    Serial.print("down ");
                                    break;                                                                                                                                             
                         }
                      Serial.print(" to servo[");
                      Serial.print(cnt);
                      Serial.print("] to ");
                      Serial.println(deg);
                      } else Serial.println("deg out of range[1-175]");
                      }
                      else Serial.println("set not s/u/d");                      
                                           
                      Serial.println("end");
                      break;                                                                   
            case 'r': //Serial.println("run servo "+btrx);
                      cnt = btrx.toInt();
                      if(cnt <=15){
                        servo[cnt].write(sit[cnt]); delay(100);
                        servo[cnt].write(up[cnt]); delay(100);
                        servo[cnt].write(down[cnt]); delay(100);
                        servo[cnt].write(sit[cnt]); delay(100);
                       Serial.print("servo "); Serial.print(cnt); Serial.println(" run");
                      }
                      else Serial.println("servo# out of range[0-15]");
                       // servo[cnt].write(sit[cnt]); delay(15);
                       // servo[cnt].write(down[cnt]); delay(15); 
                        Serial.println(sit[cnt]);
                        Serial.println(up[cnt]);
                        Serial.println(down[cnt]);                     
                      break;
             case 'k': // press k[0]-[3] (single preee), key     
                       Serial.println("press key: "+btrx);
                      deg = btrx.toInt();
                      if(deg >=0 && deg <=7) // 0-7
                      {
                       presskey(deg);
                      }
                      else Serial.println("key# out of range[0-7]");
                      break;
             case 'z': // press k[0]-[3] (single preee), key     
                       Serial.println("press and check key: "+btrx);
                      deg = btrx.toInt();
                      if(deg >=0 && deg <=7) // 0-7
                      {
                       pcbwriteonly('z');
                       presskey(deg);
                       pcbread('z');
                      }
                      else Serial.println("key# out of range[0-7]");
                      break;                      
 
          }
        
       
            
      }
 }

 
 //*/
}

///////////////////////////////////////////
// servo press key
// k=0,1,2,3 mapping to pcba key1,2,3,4 signl press
// k=4,5,6,7 mapping to pcba key1,2,3,4 and tri-press

void presskey(int key) {
  int k;
  int tri;

  if(key <= 3) { k=key+(pcbch-1)*4; tri=0;}
  else { k=(key-4)+(pcbch-1)*4; tri=1;}
  
 if(k <=15){
  if (tri ==0){ // single press
  servo[k].write(sit[k]); delay(100);
  servo[k].write(up[k]); delay(100);
  servo[k].write(down[k]); delay(100);
  servo[k].write(sit[k]);
  }
  else { // tri-press
  servo[k].write(sit[k]); delay(100);
  servo[k].write(up[k]); delay(100);
  servo[k].write(down[k]); delay(100);

  servo[k].write(up[k]); delay(100);
  servo[k].write(down[k]); delay(100);

  servo[k].write(up[k]); delay(100);
  servo[k].write(down[k]); delay(100);
  servo[k].write(sit[k]); 
  }
  Serial.print("servo "); Serial.print(k); Serial.println(" pressed");
 }
 else Serial.println("presskey servo# out of range[0-15]");
 Serial.println(sit[k]);
 Serial.println(up[k]);
 Serial.println(down[k]);
 
}


///////////////////////////////////////////
// write command and read value from nano
void pcbwrite(char c){
  static unsigned long startime =0;
  unsigned long now =millis();
  char rc;
  char endMarker='\n';
  static byte ndx=0;
   
  
  startime=now;
  //Serial.println("pcbread"+String(pcbch)+"_"+String(c));
 // Serial.println(now);
     switch(pcbch){
      case 1: Serial.println("pcb1:"+String(c));
              Serial2.write(c);
              Serial2.write("\n\r"); 
              while (!Serial2.available()) {now=millis(); if(now-startime > TIME_OUT) {Serial.println("serial"+String(pcbch)+"Time out");break;}}
              Serial.println(Serial2.readString());
              break;
      case 2: Serial.println("pcb2:"+String(c));
              Serial3.write(c);
              Serial3.write("\n\r");
              while (!Serial3.available()) {now=millis(); if(now-startime > TIME_OUT) {Serial.println("serial"+String(pcbch)+"Time out");break;}}
              Serial.print(Serial3.readString());
              break;
      case 3: Serial.println("pcb3:"+String(c));
              Serial4.listen(); // for multi serial need define the focus for serial read
              Serial4.write(c);
              Serial4.write("\n\r");              
              while (!Serial4.available()) {now=millis(); if(now-startime > TIME_OUT) {Serial.println("serial"+String(pcbch)+"Time out");break;}}                
              Serial.print(Serial4.readString());      
              break;
      case 4: Serial.println("pcb4:"+String(c));
              Serial5.listen();
              Serial5.write(c);
              Serial5.write("\n\r");
              while (!Serial5.available()) {now=millis(); if(now-startime > TIME_OUT) {Serial.println("serial"+String(pcbch)+"Time out");break;}}
              Serial.print(Serial5.readString());
              break; 
                                      
     }
     
}
///////////////////////////////////////////
// read read value from nano
void pcbread(char c){
  static unsigned long startime =0;
  unsigned long now =millis();
  char rc;
  char endMarker='\n';
  static byte ndx=0;
   
  
  startime=now;
  //Serial.println("pcbread"+String(pcbch)+"_"+String(c));
 // Serial.println(now);
     switch(pcbch){
      case 1: Serial.println("Rpcb1:");
              while (!Serial2.available()) {now=millis(); if(now-startime > TIME_OUT) {Serial.println("serial"+String(pcbch)+"Time out");break;}}
              Serial.println(Serial2.readString());
              break;
      case 2: Serial.println("pcb2:");
              while (!Serial3.available()) {now=millis(); if(now-startime > TIME_OUT) {Serial.println("serial"+String(pcbch)+"Time out");break;}}
              Serial.print(Serial3.readString());
              break;
      case 3: Serial.println("pcb3:");
              Serial4.listen(); // for multi serial need define the focus for serial read          
              while (!Serial4.available()) {now=millis(); if(now-startime > TIME_OUT) {Serial.println("serial"+String(pcbch)+"Time out");break;}}                
              Serial.print(Serial4.readString());      
              break;
      case 4: Serial.println("pcb4:");
              Serial5.listen();
              while (!Serial5.available()) {now=millis(); if(now-startime > TIME_OUT) {Serial.println("serial"+String(pcbch)+"Time out");break;}}
              Serial.print(Serial5.readString());
              break; 
                                      
     }
     
}
///////////////////////////////////////////
// write command and read value from nano
void pcbwriteonly(char c){
  static unsigned long startime =0;
  unsigned long now =millis();
  char rc;
  char endMarker='\n';
  static byte ndx=0;
   
  
  startime=now;
  //Serial.println("pcbread"+String(pcbch)+"_"+String(c));
 // Serial.println(now);
     switch(pcbch){
      case 1: Serial.println("pcb1:"+String(c));
              Serial2.write(c);
              Serial2.write("\n\r"); 
              break;
      case 2: Serial.println("pcb2:"+String(c));
              Serial3.write(c);
              Serial3.write("\n\r");
              break;
      case 3: Serial.println("pcb3:"+String(c));
              Serial4.listen(); // for multi serial need define the focus for serial read
              Serial4.write(c);
              Serial4.write("\n\r");                 
              break;
      case 4: Serial.println("pcb4:"+String(c));
              Serial5.listen();
              Serial5.write(c);
              Serial5.write("\n\r");
              break; 
                                      
     }
     
}
///////////////////////////////////////////
//
 void led_blink(int i) {
  int k;
  for (k=0;k<3;k++){
  digitalWrite(i, HIGH);
  delay(200);
  digitalWrite(i, LOW);
  delay(200);
  }
  }
///////////////////////////////////////////
//
void reset_nano(){
 digitalWrite(44,LOW);
 digitalWrite(45,LOW);
 digitalWrite(46,LOW);
 digitalWrite(47,LOW);
 delay(500);
 digitalWrite(44,HIGH);
 digitalWrite(45,HIGH);
 digitalWrite(46,HIGH);
 digitalWrite(47,HIGH);
 
}

///////////////////////////////////////////
//
/*
void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    // Print abscissa value 
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
  break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
  break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
  break;
    }
    if(scaleType!=SCL_PLOT)
    {    
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    }
    Serial.println(vData[i], 4);
  }
  Serial.println();
}  
*/
