//####################################################################
// for arduino atm2560, 2019/12/19
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
//20191219: mega2560 to nano baud rate change to 38400
//        Auto key press(4 key) to check pcba uart log 
//        Paring mode key 
//        Pwr on key 
//        pwr key tri-press(BT address)
//  by Eason
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
int deg,i;
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
 int t=0;
int eeaddr =0;
byte eev;

int pcbch,retest; // pcb channel 0-3 mapping to serial2,3,4,5
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
  Serial.write("3S PWR/BT tester atm2560 BT7\n\r");
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
  Serial2.begin(38400); // to nano pcba1
  Serial3.begin(38400); // to nano pcba2
  Serial4.begin(38400); // to nano pcba3
  Serial5.begin(38400); // to nano pcba4

//  pinMode(44,OUTPUT);
//  pinMode(45,OUTPUT);
//  pinMode(46,OUTPUT);
//  pinMode(47,OUTPUT);
// define servo
  for (cnt=0; cnt < 16; cnt++){
    servo[cnt].detach(); // servo closed from D22 to D37
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
String btrx2,instr2,instr3,btrx4;  //for reback()
int k,h;
char j,v,key_check[15],bt_check[3];




void loop() {
  // put your main code here, to run repeatedly:

 
 String btrx,instr,servo_test,btrx2,instr2;
 String Rssi;
 char char_array[10];
 char i,ch,m,j,v;
 pwmWrite(pwm,128);
 pwmWrite(pwm2,128);
 


 delay(1);
 while (!Serial.available()) {} // wait RX data input (pc)
 instr=Serial.readString(); 
// Serial.println(instr);  // debug message
// Serial.println(String(instr.length())); // debug message


  
 if(instr.length() ==1){ // 20190917 length include \n\r? 
 i=instr[0];
  switch (i){
   case 'Q': break;
   case '1': case '2':  case '3': case '4': case '5': case '6': case '7': case '8': case '9':
   case 'a': case 'b':  case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 's': case 'x': case 'q':case 'j':
             Serial.print("call slave..");
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
                  Serial.print(ch);
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
             Serial1.print("AT+INQM=1,4,6\n\r ");
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
      else if (instr[0]=='#') {  //select pcba
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
                        servo[cnt].attach(cnt+22); // servo start
                        delay(5);
                        servo[cnt].write(sit[cnt]); delay(100);
                        servo[cnt].write(up[cnt]); delay(100);
                        servo[cnt].write(down[cnt]); delay(100);
                        servo[cnt].write(sit[cnt]); delay(100);
                        delay(5);
                        servo[cnt].detach(); // servo close
                       Serial.print("servo "); Serial.print(cnt); Serial.println(" run");
                      }
                      else Serial.println("servo# out of range[0-15]");
                       // servo[cnt].write(sit[cnt]); delay(15);
                       // servo[cnt].write(down[cnt]); delay(15); 
                        Serial.println(sit[cnt]);
                        Serial.println(up[cnt]);
                        Serial.println(down[cnt]);  
                        //delay(1000);
                        
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
                
             
 
          }
        
       
            
      }
      else if(instr[0]=='z'){        //send char to nano and test key enable/disable
         i=instr[1];
         m=instr[2];
         btrx4=instr.substring(2);
         switch(i){
                case '1'://pcba1 check 4 key (power on->anc-> v+ -> v-)
                         for (cnt=0; cnt < 5; cnt++){
                              servo[cnt].attach(cnt+22);}
                         delay(1);
                         pcbch=1;
                         servo_pcba_1();
                         delay(1);
                         for (cnt=0; cnt < 5; cnt++){
                              servo[cnt].detach();} 
                         break;
                case '2'://pcba2  check 4 key (power on->anc-> v+ -> v-)
                         for (cnt=4; cnt < 8; cnt++){
                              servo[cnt].attach(cnt+22);}
                         delay(1);
                         pcbch=2;
                         servo_pcba_2();
                         delay(1);
                         for (cnt=4; cnt < 8; cnt++){
                              servo[cnt].detach();} 
                         break;
                case '3'://pcba3  check 4 key (power on->anc-> v+ -> v-)
                         pcbch=3;
                         servo_pcba_3();
                         break;
                case '4'://pcba4  check 4 key (power on->anc-> v+ -> v-)
                         pcbch=4;
                         servo_pcba_4();
                         break; 
              /////////////////////////////////////////////////////////////
                case '5': //check pcba1 bt address(triple press)
                         pcbch=1;
                         BT_pcba_1();
                         break;
                case '6': //check pcba2 bt address(triple press)
                         pcbch=2;
                         BT_pcba_2();
                         break;
                case '7': //check pcba3 bt address(triple press)
                         pcbch=3;
                         BT_pcba_3();
                         break;
                case '8': //check pcba4 bt address(triple press)
                         pcbch=4;
                         BT_pcba_4();
                         break;
              ////////////////////pcba pairing mode(pwr key)/////////////////////////////
                 case '9':
                          switch(m){
                                    case '1':  //pcba1
                                            pwr_pairing_key_pcba_1();
                                            break;
                                    case '2':  //pcba2
                                            pwr_pairing_key_pcba_2();
                                            break;                            
                                    case '3':  //pcba3
                                            pwr_pairing_key_pcba_3();
                                            break;                            
                                    case '4':  //pcba4
                                            pwr_pairing_key_pcba_4();
                                            break;                            
                            }
                          break;
              /////////////////////pcba pwr on(pwr key)/////////////////////////
                 case 'a':
                          switch(m){
                                    case '1':  //pcba1
                                            pwr_on_key_pcba_1();
                                            break;
                                    case '2':  //pcba2
                                            pwr_on_key_pcba_2();
                                            break;                            
                                    case '3':  //pcba3
                                            pwr_on_key_pcba_3();
                                            break;                            
                                    case '4':  //pcba4
                                            pwr_on_key_pcba_4();
                                            break;                            
                            }
                          break;
              ///////////////////////auto all pcba 16key test//////////////////// 
                 case 'b':
                          auto_all_pcba_16_key_test();
                          break; 
              //////////////////////////////////////////////////////////////////
                case 't':  //delay time for check key log(nano)  d=100~300 tune
                        switch(pcbch){
                                     case 1:
                                             Serial2.println("zt"+btrx4);
                                             btrx4="";
                                             break;
                                     case 2:
                                             Serial3.println("zt"+btrx4);
                                             btrx4="";
                                             break;
                                     case 3:
                                             Serial4.listen();
                                             Serial4.println("zt"+btrx4);
                                             btrx4="";
                                             break;                                             
                                     case 4:
                                             Serial5.listen();
                                             Serial5.println("zt"+btrx4);
                                             btrx4="";
                                             break;                             
                             }  
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
             // reback();    // wait nano data input
              
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

void reback(){
  instr2="";
  btrx2="";
  switch(pcbch){
            case 1:
                   instr2=Serial2.readString(); //pcba1
                   Serial.print(instr2+"\n\r");
                   break;
            case 2:
                   instr2=Serial3.readString(); //pcba2
                   Serial.print(instr2+"\n\r");
                   break;
            case 3:
                   instr2=Serial4.readString(); //pcba3
                   Serial.print(instr2+"\n\r");
                   break;
            case 4:
                   instr2=Serial5.readString(); //pcba4
                   Serial.print(instr2+"\n\r");
                   break;
  }

          k=instr2[0];
          if (instr2[0]=='%'){
          j=instr2[1];
         
          btrx2=instr2.substring(2);
    
         cnt = btrx2.toInt();
            if(cnt <=15){
              Serial.print("servo "); Serial.print(cnt); Serial.println(" run");
              servo[cnt].write(sit[cnt]); delay(100);
              servo[cnt].write(up[cnt]); delay(100);
              servo[cnt].write(down[cnt]); 
              if(cnt==0||cnt==4||cnt==8||cnt==12){  //power key delay (hold on)
                delay(1700); //pwr on
                }
              else{
                delay(100);
                }  
              servo[cnt].write(sit[cnt]); delay(100);
              
              delay(1);
            switch(pcbch){
                      case 1:
                             while (!Serial2.available()){}
                             instr3="";
                             instr3=Serial2.readString(); //pcba1 check data
                             break;
                      case 2:
                             while (!Serial3.available()){}
                             instr3="";
                             instr3=Serial3.readString(); //pcba2 check data
                             break;
                      case 3:                            
                             Serial4.listen();
                             while (!Serial4.available()){}
                             instr3="";
                             instr3=Serial4.readString(); //pcba3 check data
                             break;
                      case 4:
                             Serial5.listen();
                             while (!Serial5.available()){}
                             instr3="";
                             instr3=Serial5.readString(); //pcba4 check data
                             break;
            }
                          
                          Serial.println(instr3+"\n\r");
               
             }
            
            else Serial.println("servo# out of range[0-15]");
             // servo[cnt].write(sit[cnt]); delay(15);
             // servo[cnt].write(down[cnt]); delay(15); 
              Serial.println(sit[cnt]);
              Serial.println(up[cnt]);
              Serial.println(down[cnt]);  
          }
          else{}
          
}
//////////////////////////////////////////
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
void servo_pcba_1(){              //pcba1 servo motor        
         static unsigned long startime =0;
         unsigned long now =millis();
         startime=now;
                     //servo0    for PWR
                      Serial.print("call slave..");
                      Serial.println("pcb1:z0");
                      Serial2.write("z0\n\r");
                      delay(1);
                      while (!Serial2.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();    
                    servo0_keep:
                       if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[0]='1';
                         }
                        else {    
                            while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                              Serial2.write("z0\n\r");
                              reback();
                              if(t<1){
                                t=t+1;
                                goto servo0_keep;
                                     }
                              else{
                                  if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                      Serial.println("The key is disable\n\r");
                                      key_check[0]='0';
                                      break;
                                        }
                                  else{
                                      instr3="";
                                      Serial.println("key ok \n\r");
                                      key_check[0]='1';
                                      break;
                                    }                          
                                  }
                              }
                        }
                     //servo1     for ANC Key
                      Serial.print("call slave..");
                      Serial.println("pcb1:z1");
                      Serial2.write("z1\n\r"); 
                      delay(1);
                      while (!Serial2.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();    
                    servo1_keep:  
                          if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[1]='1';
                         }   
                          else {                
                                while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial2.write("z1\n\r");
                                  reback();
                                  if(t<1){
                                    t=t+1;
                                    goto servo1_keep;
                                         }
                                  else{
                                      if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                        Serial.println("The key is disable\n\r");
                                        key_check[1]='0';
                                        break;
                                        }
                                     else{
                                        instr3="";
                                        Serial.println("key ok \n\r");
                                        key_check[1]='1';
                                        break;
                                         }
                                        }                          
                                      }
                                }
                          
                   //servo2        for V+
                      Serial.print("call slave..");
                      Serial.println("pcb1:z2");
                      Serial2.write("z2");
                      Serial2.write("\n\r");
                      delay(1);
                      while (!Serial2.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();
                    servo2_keep:
                          if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[2]='1';
                         }   
                           else { 
                                while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial2.write("z2\n\r");
                                  reback();
                                  if(t<1){
                                    t=t+1;
                                    goto servo2_keep;
                                         }
                                  else{
                                      if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                        Serial.println("The key is disable\n\r");
                                        key_check[2]='0';
                                        break;
                                        }  
                                     else{
                                        instr3="";
                                        Serial.println("key ok \n\r");
                                        key_check[2]='1';
                                        break;
                                         }
                                        }                          
                                      }
                                  }
                               
                    //servo3         for V-
                      Serial.print("call slave..");
                      Serial.println("pcb1:z3");
                      Serial2.write("z3");
                      Serial2.write("\n\r");
                      delay(1);
                      while (!Serial2.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();   
                    servo3_keep:
                        if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[3]='1';
                         }
                        else {    
                        while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                          Serial2.write("z3\n\r");
                          reback();
                          if(t<1){
                            t=t+1;
                            goto servo3_keep;
                                 }
                          else{
                              if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial.println("The key is disable\n\r");
                                  key_check[3]='0';
                                  break;
                                  }
                               else {
                                  instr3="";
                                  Serial.println("key ok \n\r");
                                  key_check[3]='1';
                                  break;
                                   }
                                }                          
                              }
                            }

               for(i=0;i<=3;i++){
                if(key_check[i]=='1'){
                  Serial.print("servo ");Serial.print(i);Serial.println(" passed\n\r");
                  }
                else{
                  Serial.print("servo ");Serial.print(i);Serial.println(" did not pass\n\r");
                  }
                }  
  }
  ////////////////////////////////////////////////////////
void servo_pcba_2(){   // pcba2
         static unsigned long startime =0;
         unsigned long now =millis();
         startime=now;
                     //servo4    for PWR
                      Serial.print("call slave..");
                      Serial.println("pcb2:z4");
                      Serial3.write("z4\n\r");
                      delay(1);
                      while (!Serial3.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();    
                    servo4_keep:
                       if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[4]='1';
                         }
                        else {    
                            while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                              Serial3.write("z4\n\r");
                              reback();
                              if(t<1){
                                t=t+1;
                                goto servo4_keep;
                                     }
                              else{
                                  if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                      Serial.println("The key is disable\n\r");
                                      key_check[4]='0';
                                      break;
                                        }
                                  else{
                                      instr3="";
                                      Serial.println("key ok \n\r");
                                      key_check[4]='1';
                                      break;
                                    }                          
                                  }
                              }
                        }
                     //servo5     for ANC Key
                      Serial.print("call slave..");
                      Serial.println("pcb2:z5");
                      Serial3.write("z5\n\r"); 
                      delay(1);
                      while (!Serial3.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();    
                    servo5_keep:  
                          if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[5]='1';
                         }   
                          else {                
                                while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial3.write("z5\n\r");
                                  reback();
                                  if(t<1){
                                    t=t+1;
                                    goto servo5_keep;
                                         }
                                  else{
                                      if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                        Serial.println("The key is disable\n\r");
                                        key_check[5]='0';
                                        break;
                                        }
                                     else{
                                        instr3="";
                                        Serial.println("key ok \n\r");
                                        key_check[5]='1';
                                        break;
                                         }
                                        }                          
                                      }
                                }
                          
                   //servo6        for V+
                      Serial.print("call slave..");
                      Serial.println("pcb2:z6");
                      Serial3.write("z6");
                      Serial3.write("\n\r");
                      delay(1);
                      while (!Serial3.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      reback();
                      t=0;
                    servo6_keep:
                          if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[6]='1';
                         }   
                           else { 
                                while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial3.write("z6\n\r");
                                  reback();
                                  if(t<1){
                                    t=t+1;
                                    goto servo6_keep;
                                         }
                                  else{
                                      if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                        Serial.println("The key is disable\n\r");
                                        key_check[6]='0';
                                        break;
                                        }  
                                     else{
                                        instr3="";
                                        Serial.println("key ok \n\r");
                                        key_check[6]='1';
                                        break;
                                         }
                                        }                          
                                      }
                                  }
                               
                    //servo7         for V-
                      Serial.print("call slave..");
                      Serial.println("pcb2:z7");
                      Serial3.write("z7");
                      Serial3.write("\n\r");
                      delay(1);
                      while (!Serial3.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      reback();
                      t=0;   
                    servo7_keep:
                        if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[7]='1';
                         }
                        else {    
                        while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                          Serial3.write("z7\n\r");
                          reback();
                          if(t<1){
                            t=t+1;
                            goto servo7_keep;
                                 }
                          else{
                              if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial.println("The key is disable\n\r");
                                  key_check[7]='0';
                                  break;
                                  }
                               else {
                                  instr3="";
                                  Serial.println("key ok \n\r");
                                  key_check[7]='1';
                                  break;
                                   }
                                }                          
                              }
                            }
               for(i=4;i<=7;i++){
                if(key_check[i]=='1'){
                  Serial.print("servo ");Serial.print(i);Serial.println(" passed\n\r");
                  }
                else{
                  Serial.print("servo ");Serial.print(i);Serial.println(" did not pass\n\r");
                  }
                }
  }
void servo_pcba_3(){  //pcba3
         static unsigned long startime =0;
         unsigned long now =millis();
         startime=now;
                     //servo8    for PWR
                      Serial.print("call slave..");
                      Serial.println("pcb3:z8");
                      Serial4.listen(); // for multi serial need define the focus for serial read
                      Serial4.write("z8");
                      servo[8].attach(30);
                      delay(1);
                      while (!Serial4.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();    
                    servo8_keep:
                       servo[8].attach(30);
                       if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[8]='1';
                          servo[8].detach();
                         }
                        else {    
                            while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                              Serial4.write("z8\n\r");
                              reback();
                              key_check[8]='1';
                              servo[8].detach();
                              if(t<1){
                                t=t+1;
                                goto servo8_keep;
                                     }
                              else{
                                  if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                      Serial.println("The key is disable\n\r");
                                      key_check[8]='0';
                                      break;
                                        }
                                  else{
                                      instr3="";
                                      Serial.println("key ok \n\r");
                                      key_check[8]='1';
                                      break;
                                    }                          
                                  }
                              }
                        }
                     //servo9     for ANC Key
                      Serial.print("call slave..");
                      Serial.println("pcb3:z9");
                      Serial4.listen(); // for multi serial need define the focus for serial read
                      Serial4.write("z9\n\r");
                      servo[9].attach(31);
                      delay(1);
                      while (!Serial4.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();    
                    servo9_keep:
                    servo[9].attach(31);  
                          if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[9]='1';
                          servo[9].detach();
                         }   
                          else {                
                                while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial4.write("z9\n\r");
                                  reback();
                                  servo[9].detach();
                                  if(t<1){
                                    t=t+1;
                                    goto servo9_keep;
                                         }
                                  else{
                                      if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                        Serial.println("The key is disable\n\r");
                                        key_check[9]='0';
                                        break;
                                        }
                                     else{
                                        instr3="";
                                        Serial.println("key ok \n\r");
                                        key_check[9]='1';
                                        break;
                                         }
                                        }                          
                                      }
                                }
                          
                   //servo10        for V+
                      Serial.print("call slave..");
                      Serial.println("pcb3:za");
                      Serial4.listen(); // for multi serial need define the focus for serial read
                      Serial4.write("za");
                      Serial4.write("\n\r");
                      servo[10].attach(32);
                      delay(1);
                      while (!Serial4.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      reback();
                      t=0;
                    servo10_keep:
                    servo[10].attach(32);
                          if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[10]='1';
                          servo[10].detach();
                         }   
                           else { 
                                while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial4.write("za\n\r");
                                  reback();
                                  servo[10].detach();
                                  if(t<1){
                                    t=t+1;
                                    goto servo10_keep;
                                         }
                                  else{
                                      if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                        Serial.println("The key is disable\n\r");
                                        key_check[10]='0';
                                        break;
                                        }  
                                     else{
                                        instr3="";
                                        Serial.println("key ok \n\r");
                                        key_check[10]='1';
                                        break;
                                         }
                                        }                          
                                      }
                                  }
                               
                    //servo11         for V-
                      Serial.print("call slave..");
                      Serial.println("pcb3:zb");
                      Serial4.listen(); // for multi serial need define the focus for serial read
                      Serial4.write("zb");
                      Serial4.write("\n\r");
                      servo[11].attach(33);
                      delay(1);
                      while (!Serial4.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      reback();
                      t=0;   
                    servo11_keep:
                    servo[11].attach(33);
                        if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[11]='1';
                          servo[11].detach();
                         }
                        else {    
                        while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                          Serial4.write("zb\n\r");
                          reback();
                          servo[11].detach();
                          if(t<1){
                            t=t+1;
                            goto servo11_keep;
                                 }
                          else{
                              if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial.println("The key is disable\n\r");
                                  key_check[11]='0';
                                  break;
                                  }
                               else {
                                  instr3="";
                                  Serial.println("key ok \n\r");
                                  key_check[11]='1';
                                  break;
                                   }
                                }                          
                              }
                            }
               for(i=8;i<=11;i++){
                if(key_check[i]=='1'){
                  Serial.print("servo ");Serial.print(i);Serial.println(" passed\n\r");
                  }
                else{
                  Serial.print("servo ");Serial.print(i);Serial.println(" did not pass\n\r");
                  }
                }
  }


void servo_pcba_4(){
 static unsigned long startime =0;
         unsigned long now =millis();
         startime=now;
                     //servo12    for PWR
                      Serial.print("call slave..");
                      Serial.println("pcb4:zc");
                      Serial5.listen(); // for multi serial need define the focus for serial read
                      Serial5.write("zc\n\r");
                      servo[12].attach(34);
                      delay(1);
                      while (!Serial5.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();    
                      servo12_keep:
                      servo[12].attach(34);
                       if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[12]='1';
                          servo[12].detach();
                         }
                        else {    
                            while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                              Serial5.write("zc\n\r");
                              reback();
                              servo[12].detach();
                              if(t<1){
                                t=t+1;
                                goto servo12_keep;
                                     }
                              else{
                                  if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                      Serial.println("The key is disable\n\r");
                                      key_check[12]='0';
                                      break;
                                        }
                                  else{
                                      instr3="";
                                      Serial.println("key ok \n\r");
                                      key_check[12]='1';
                                      break;
                                    }                          
                                  }
                              }
                        }
                     //servo13     for ANC Key
                      Serial.print("call slave..");
                      Serial.println("pcb4:zd");
                      Serial5.listen(); // for multi serial need define the focus for serial read
                      Serial5.write("zd\n\r"); 
                      delay(1);
                      servo[13].attach(35);
                      while (!Serial5.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      reback();    
                    servo13_keep:
                    servo[13].attach(35);  
                          if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[13]='1';
                          servo[13].detach();
                         }   
                          else {                
                                while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial5.write("zd\n\r");
                                  reback();
                                  servo[13].detach();
                                  if(t<1){
                                    t=t+1;
                                    goto servo13_keep;
                                         }
                                  else{
                                      if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                        Serial.println("The key is disable\n\r");
                                        key_check[13]='0';
                                        break;
                                        }
                                     else{
                                        instr3="";
                                        Serial.println("key ok \n\r");
                                        key_check[13]='1';
                                        break;
                                         }
                                        }                          
                                      }
                                }
                          
                   //servo14        for V+
                      Serial.print("call slave..");
                      Serial.println("pcb4:ze");
                      Serial5.listen(); // for multi serial need define the focus for serial read
                      Serial5.write("ze");
                      Serial5.write("\n\r");
                      delay(1);
                      servo[14].attach(36);
                      while (!Serial5.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      reback();
                      t=0;
                    servo14_keep:
                    servo[14].attach(36);
                          if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[14]='1';
                          servo[14].detach();
                         }   
                           else { 
                                while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial5.write("ze\n\r");
                                  reback();
                                  servo[14].detach();
                                  if(t<1){
                                    t=t+1;
                                    goto servo14_keep;
                                         }
                                  else{
                                      if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                        Serial.println("The key is disable\n\r");
                                        key_check[14]='0';
                                        break;
                                        }  
                                     else{
                                        instr3="";
                                        Serial.println("key ok \n\r");
                                        key_check[14]='1';
                                        break;
                                         }
                                        }                          
                                      }
                                  }
                               
                    //servo15         for V-
                      Serial.print("call slave..");
                      Serial.println("pcb4:zf");
                      Serial5.listen(); // for multi serial need define the focus for serial read
                      Serial5.write("zf");
                      Serial5.write("\n\r");
                      delay(1);
                      servo[15].attach(37);
                      while (!Serial5.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      reback();
                      t=0;   
                    servo15_keep:
                    servo[15].attach(37);
                        if(instr3[1]!='d'){
                          instr3="";
                          Serial.println("key ok \n\r");
                          key_check[15]='1';
                          servo[15].detach();
                         }
                        else {    
                        while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                          Serial5.write("zf\n\r");
                          reback();
                          servo[15].detach();
                          if(t<1){
                            t=t+1;
                            goto servo15_keep;
                                 }
                          else{
                              if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial.println("The key is disable\n\r");
                                  key_check[15]='0';
                                  break;
                                  }
                               else {
                                  instr3="";
                                  Serial.println("key ok \n\r");
                                  key_check[15]='1';
                                  break;
                                   }
                                }                          
                              }
                            }
               for(i=12;i<=15;i++){
                if(key_check[i]=='1'){
                  Serial.print("servo ");Serial.print(i);Serial.println(" passed\n\r");
                  }
                else{
                  Serial.print("servo ");Serial.print(i);Serial.println(" did not pass\n\r");
                  }
                }
  }
  /////////////////////////////bt/////////////////
void BT_pcba_1(){
         static unsigned long startime =0;
         unsigned long now =millis();
         startime=now;
                     //servo0    for PWR
                      Serial.print("call slave..");
                      Serial.println("pcb1:zg");
                      Serial2.write("zg\n\r");
                      delay(1);
                      servo[0].attach(22);
                      while (!Serial2.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      bt_reback();    
                    servo0_keep:
                    servo[0].attach(22);
                       if(instr3[1]!='d'){
                          instr3="";
                          bt_check[0]='1';
                          servo[0].detach();
                         }
                        else {    
                            while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                              Serial2.write("zg\n\r");
                              bt_reback();
                              servo[0].detach();
                              if(t<1){
                                t=t+1;
                                goto servo0_keep;
                                     }
                              else{
                                  if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial.println("The bt is disable\n\r");
                                  bt_check[0]='0';
                                      break;
                                        }                         
                                  }
                              }
                        }  
  }
  void BT_pcba_2(){
         static unsigned long startime =0;
         unsigned long now =millis();
         startime=now;
                     //servo4    for PWR
                      Serial.print("call slave..");
                      Serial.println("pcb1:zh");
                      Serial3.write("zh\n\r");
                      delay(1);
                      servo[4].attach(26);
                      while (!Serial3.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      bt_reback();    
                    servo4_keep:
                    servo[4].attach(26);
                       if(instr3[1]!='d'){
                          instr3="";
                          bt_check[4]='1';
                          servo[4].detach();
                         }
                        else {    
                            while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                              Serial3.write("zh\n\r");
                              bt_reback();
                              servo[4].detach();
                              if(t<1){
                                t=t+1;
                                goto servo4_keep;
                                     }
                              else{
                                  if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial.println("The bt is disable\n\r");
                                  bt_check[4]='0';
                                      break;
                                        }                         
                                  }
                              }
                        } 
               }
  void BT_pcba_3(){
         static unsigned long startime =0;
         unsigned long now =millis();
         startime=now;
                     //servo8    for PWR
                      Serial.print("call slave..");
                      Serial.println("pcb1:zi");
                      Serial4.listen(); // for multi serial need define the focus for serial read
                      Serial4.write("zi\n\r");
                      delay(1);
                      servo[8].attach(30);
                      while (!Serial4.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      bt_reback();    
                    servo8_keep:
                    servo[8].attach(30);
                       if(instr3[1]!='d'){
                          instr3="";
                          bt_check[8]='1';
                          servo[8].detach();
                         }
                        else {    
                            while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                              Serial4.write("zi\n\r");
                              bt_reback();
                              servo[8].detach();
                              if(t<1){
                                t=t+1;
                                goto servo8_keep;
                                     }
                              else{
                                  if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial.println("The bt is disable\n\r");
                                  bt_check[8]='0';
                                      break;
                                        }                         
                                  }
                              }
                        }
                  }
  void BT_pcba_4(){
         static unsigned long startime =0;
         unsigned long now =millis();
         startime=now;
                     //servo12    for PWR
                      Serial.print("call slave..");
                      Serial.println("pcb1:zj");
                      Serial5.listen(); // for multi serial need define the focus for serial read
                      Serial5.write("zj\n\r");
                      delay(1);
                      servo[12].attach(34);
                      while (!Serial5.available()) {now=millis(); if(now-startime > TIME_OUT) {}}
                      t=0;
                      bt_reback();    
                      servo12_keep:
                      servo[12].attach(34);
                       if(instr3[1]!='d'){
                          instr3="";
                          bt_check[12]='1';
                          servo[12].detach();
                         }
                        else {    
                            while(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                              Serial5.write("zj\n\r");
                              bt_reback();
                              servo[12].detach();
                              if(t<1){
                                t=t+1;
                                goto servo12_keep;
                                     }
                              else{
                                  if(instr3[1]=='d'&&instr3[2]=='o'&&instr3[3]=='n'&&instr3[4]=='e'){
                                  Serial.println("The bt is disable\n\r");
                                  bt_check[12]='0';
                                      break;
                                        }                         
                                  }
                              }
                        }
                 }
////////////////////////////////////////////////////////
void bt_reback(){
  instr2="";
  btrx2="";
  switch(pcbch){
            case 1:
                   instr2=Serial2.readString(); //pcba1
                   Serial.print(instr2+"\n\r");
                   break;
            case 2:
                   instr2=Serial3.readString(); //pcba2
                   Serial.print(instr2+"\n\r");
                   break;
            case 3:
                   instr2=Serial4.readString(); //pcba3
                   Serial.print(instr2+"\n\r");
                   break;
            case 4:
                   instr2=Serial5.readString(); //pcba4
                   Serial.print(instr2+"\n\r");
                   break;
  }

          k=instr2[0];
          if (instr2[0]=='%'){
          j=instr2[1];
         
          btrx2=instr2.substring(2);
    
          k = btrx2.toInt();
              Serial.print("servo "); Serial.print(k); Serial.println(" pressed");
              servo[k].write(sit[k]); delay(100);
              servo[k].write(up[k]); delay(100);
              servo[k].write(down[k]); delay(100);
            
              servo[k].write(up[k]); delay(100);
              servo[k].write(down[k]); delay(100);
            
              servo[k].write(up[k]); delay(100);
              servo[k].write(down[k]); delay(100);
              servo[k].write(sit[k]); 
              
              delay(1);
            switch(pcbch){
                      case 1:
                             while (!Serial2.available()){}
                             instr3="";
                             instr3=Serial2.readString(); //pcba1 check data
                             break;
                      case 2:
                             while (!Serial3.available()){}
                             instr3="";
                             instr3=Serial3.readString(); //pcba2 check data
                             break;
                      case 3:                            
                             Serial4.listen();
                             while (!Serial4.available()){}
                             instr3="";
                             instr3=Serial4.readString(); //pcba3 check data
                             break;
                      case 4:
                             Serial5.listen();
                             while (!Serial5.available()){}
                             instr3="";
                             instr3=Serial5.readString(); //pcba4 check data
                             break;
            }         
                          Serial.println(instr3+"\n\r");            
          }
          else{}
          
}
///////////////////////////////////////////////////////  
void pwr_pairing_key_pcba_1(){
             cnt =0;
              servo[0].attach(22);
              Serial.print("servo "); Serial.print(cnt); Serial.println(" pairing run");
              servo[cnt].write(sit[cnt]); delay(100);
              servo[cnt].write(up[cnt]); delay(100);
              servo[cnt].write(down[cnt]); 
              delay(4000);
              servo[cnt].write(sit[cnt]); delay(100);
              delay(1);
              servo[0].detach();               
  }
void pwr_pairing_key_pcba_2(){
              cnt =4;
              servo[4].attach(26);
              Serial.print("servo "); Serial.print(cnt); Serial.println(" pairing run");
              servo[cnt].write(sit[cnt]); delay(100);
              servo[cnt].write(up[cnt]); delay(100);
              servo[cnt].write(down[cnt]); 
              delay(4000);
              servo[cnt].write(sit[cnt]); delay(100);  
              delay(1);
              servo[4].detach();  
  }
void pwr_pairing_key_pcba_3(){
              cnt =8;
              servo[8].attach(30);
              Serial.print("servo "); Serial.print(cnt); Serial.println(" pairing run");
              servo[cnt].write(sit[cnt]); delay(100);
              servo[cnt].write(up[cnt]); delay(100);
              servo[cnt].write(down[cnt]); 
              delay(4000);
              servo[cnt].write(sit[cnt]); delay(100);  
              delay(1);
              servo[8].detach();
  }
void pwr_pairing_key_pcba_4(){
              cnt =12;
              servo[12].attach(34);
              Serial.print("servo "); Serial.print(cnt); Serial.println(" pairing run");
              servo[cnt].write(sit[cnt]); delay(100);
              servo[cnt].write(up[cnt]); delay(100);
              servo[cnt].write(down[cnt]); 
              delay(4000);
              servo[cnt].write(sit[cnt]); delay(100);  
              delay(1);
              servo[12].detach();
  }
///////////////////////////////////////////////////////
void pwr_on_key_pcba_1(){
              cnt =0;
              servo[0].attach(22);
              Serial.print("servo "); Serial.print(cnt); Serial.println(" pairing run");
              servo[cnt].write(sit[cnt]); delay(100);
              servo[cnt].write(up[cnt]); delay(100);
              servo[cnt].write(down[cnt]); 
              delay(1700);
              servo[cnt].write(sit[cnt]); delay(100);
              delay(1);
              servo[0].detach(); 
  }
void pwr_on_key_pcba_2(){
              cnt =4;
              servo[4].attach(26);
              Serial.print("servo "); Serial.print(cnt); Serial.println(" pairing run");
              servo[cnt].write(sit[cnt]); delay(100);
              servo[cnt].write(up[cnt]); delay(100);
              servo[cnt].write(down[cnt]); 
              delay(1700);
              servo[cnt].write(sit[cnt]); delay(100);
              delay(1);
              servo[4].detach(); 
  }
void pwr_on_key_pcba_3(){
              cnt =8;
              servo[8].attach(30);
              Serial.print("servo "); Serial.print(cnt); Serial.println(" pairing run");
              servo[cnt].write(sit[cnt]); delay(100);
              servo[cnt].write(up[cnt]); delay(100);
              servo[cnt].write(down[cnt]); 
              delay(1700);
              servo[cnt].write(sit[cnt]); delay(100);
              delay(1);
              servo[8].detach();
  }
void pwr_on_key_pcba_4(){
                cnt =12;
              servo[12].attach(34);
              Serial.print("servo "); Serial.print(cnt); Serial.println(" pairing run");
              servo[cnt].write(sit[cnt]); delay(100);
              servo[cnt].write(up[cnt]); delay(100);
              servo[cnt].write(down[cnt]); 
              delay(1700);
              servo[cnt].write(sit[cnt]); delay(100);
              delay(1);
              servo[12].detach();
  }
//////////////////////////////////////////////////////
void auto_all_pcba_16_key_test(){}
