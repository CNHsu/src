//####################################################################
// for arduino atm2560, 2019/09/10
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
//#include  <SoftwareSerial.h>  // atm256 use hw serial
#include <EEPROM.h>
#include <PWM.h>
#include <arduinoFFT.h>

//SoftwareSerial Serial1(5, 6); // RX | TX, atm256 don't6 use software serial
int LED13=13; // LED on board
int pwm=11; // pwm 1k
int pwm2=9; //pwm2
int A5V=A0; // 5v voltage
int A5C=A1; //5v current
int A4V=A2; //4v voltage
int A4C=A3; //4v current
int VBUS_EN=51;//D51
int BAT_EN=50; //D50
int uA_SET1=53;//D53 for 5V source '1' mA, '0'uA, 2019/4/29 update
int uA_SET2=52;//D52 for 4V BAT source '1' mA, '0' uA, 2019/4/29 update
int FLT=49;//D49, input, '1' ok, '0' power fail

int SELEN_B=46; //D46 74hc4852 INH 'H'=inhibit output, 'L'=enable output
int S0=45;      // S1S0=00,01,10,11 select 74hc4852 Y0,Y1,Y2,Y3
int S1=44;      // 1. select A0,A1,A2,A3 for which pcba1-4
                // 2. select SR/SL/MR/ML/AGND for which pcba1-4
                // 3. selct TXD for which pcba1-4 to arduino serial3
int SVP=43;     // 74hc4852 inhibit for V+ key short to GND, S0,S1 select which pcba
int SVN=42;     // 74hc4852 inhibit for V- key short to GND, S0,S1 select which pcba
int SANC=41;    // 74hc4852 inhibit for ANC key short to GND, S0,S1 select which pcba
int SPWR=40;    // 74hc4852 inhibit for PWR key short to GND, S0,S1 select which pcba
int LED3 =39; // temp not use
int LED2 =38; // temp not use
int K1=38;
int K2=37;
int K3=36;
int fftinputL=A9; // A9 for fft-L
int fftinputR=A10; // A10 for fft-R

int val=0;
char *endptr;
char aRssi[10];
int cnt;

int eeaddr =0;
byte eev;

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
  
  Serial.begin(38400);
  Serial.write("3S PWR/BT tester atm2560\n\r");
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
//  pinMode(5,OUTPUT);
//  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  //pinMode(10,INPUT); // for fft input
  pinMode(11,OUTPUT);
  pinMode(12,INPUT);
  pinMode(13,OUTPUT); //
  Serial1.begin(38400); //HC-05 BT default AT command mode speed, 19Rx, 18Tx
  Serial3.begin(115200); // 15Rx,14Tx, test BES serial port @'z'
  Serial2.begin(9600); // 

  setIO_default(); // initial some IO setting


}

void loop() {
  // put your main code here, to run repeatedly:
 int a=65;
 char i,ch;
 String btrx,instr;
 String Rssi;

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
 /*
  Serial.write("PCB test tool 2019/09/16 update \n\r"); 
  Serial.write("[1] 5V ON, [2] 5V OFF\n\r"); // VBUS_EN
  Serial.write("[3] 4V ON, [4] 4V OFF\n\r"); // BAT_EN
  Serial.write("==========================\n\r");
  Serial.write("[5] 5V voltage\n\r");       //A0
  Serial.write("[6] 5V current\n\r");       //A1
  Serial.write("[7] 4V voltage\n\r");       //A2
  Serial.write("[8] 4V current\n\r");       //A3
  Serial.write("==========================\n\r");
  Serial.write("[9] 5V uA,[a]=5V mA\n\r"); //uA_SET1, 2019/4/29 update
  Serial.write("[b] 4V uA,[c]=4V mA\n\r");//uA_SET2, 2019/4/29 update
  Serial.write("==========================\n\r");
  Serial.write("[d] K1=1,[e] K1=0\n\r"); //uA_SET1
  Serial.write("[f] K2=1,[g] K2=0\n\r");//uA_SET2
  Serial.write("[h] K3=1,[i] K3=0\n\r");//uA_SET2
  
  Serial.write("[u] BT reset/init/status/inqm/class\n\r");
  Serial.write(" [v] BT inq (cnt=loop)\n\r");
  Serial.write("[s] FLT \n\r"); // input value
  Serial.write("[z] key check \n\r"); // input value
  Serial.write("[Q] quit \n\r");
  Serial.write(": \n\r");
  */
 // /*
  while (!Serial.available()) {} // wait RX data input
 instr=Serial.readString();
 Serial.println(instr);
 Serial.println(String(instr.length()));
 if(instr.length() == 3){ // 20190917 length include \n\r? 
 i=instr[0];
  switch (i){
   case 'Q': break;
   case '1' : Serial.print("**5 ON\n\r");
             digitalWrite(VBUS_EN,HIGH);
             break;
   case '2' : Serial.print("**5 OFF\n\r");
             digitalWrite(VBUS_EN,LOW);
             break;
   case '3' : Serial.print("**4V ON\n\r"); 
             digitalWrite(BAT_EN,HIGH);
             break;
   case '4'  :Serial.print("**4 OFF\n\r");
             digitalWrite(BAT_EN,LOW);
             break;           
   case '5' :Serial.print("**5 voltage\n");   
             val=0;
             mux_sel(0); // default select                
             val=analogRead(A5V);
             Serial.write("5VV\n");
             Serial.print(val*0.00488);
             //Serial.print(val*0.488);
             Serial.write("\n");
             break;
   case '6'  :Serial.print("**5 current\n");
             val=0;
             val=analogRead(A5C);
             Serial.write("5VI\n");
             //Serial.print(val*0.00488);
             Serial.print(val*0.488);
             Serial.write("\n");
             break;             
   case '7'  :Serial.print("**4 voltage\n");
             val=0;
             val=analogRead(A4V);
             Serial.write("4VV\n");
             Serial.print(val*0.00488);   
             //Serial.print(val*0.488);
             Serial.write("\n");
             digitalWrite(LED3, HIGH);
             break ;            
   case '8'  :Serial.print("**4 current\n");
             val=0;
             val=analogRead(A4C);
             Serial.write("4VI\n");
             //Serial.print(val*0.00488);   
             Serial.print(val*0.488);
             Serial.write("\n");
             digitalWrite(LED3,  LOW);
             break ;  
   case '9'  :Serial.print("**5V uA\n\r"); // 2019/4/29 update mA to uA
             digitalWrite(uA_SET1,LOW);
             digitalWrite(LED3,  LOW);
             break ; 
   case 'a'  :Serial.print("**5V mA\n\r"); // 2019/4/29 update uA to mA
             digitalWrite(uA_SET1,HIGH);
             digitalWrite(LED3,  HIGH);
             break ;
   case 'b'  :Serial.print("**4V uA\n\r");// 2019/4/29 update mA to uA
             digitalWrite(uA_SET2,LOW);
             digitalWrite(LED3,  LOW);
             break ;   
   case 'c'  :Serial.print("**4V mA\n\r");// 2019/4/29 update uA to mA
             digitalWrite(uA_SET2,HIGH);
             digitalWrite(LED3,  HIGH);
             break ;            
   case 'd'  :Serial.print("**K1=1\n\r");
             digitalWrite(K1,HIGH);
             digitalWrite(LED3,  HIGH);
             break ;
   case 'e'  :Serial.print("**K1=0\n\r");
             digitalWrite(K1,LOW);
             digitalWrite(LED3,  LOW);
             break ;
   case 'f'  :Serial.print("**K2=1\n\r");
             digitalWrite(K2,HIGH);
             digitalWrite(LED3,  HIGH);
             break ;
   case 'g'  :Serial.print("**K2=0\n\r");
             digitalWrite(K2,LOW);
             digitalWrite(LED3,  LOW);
             break ;
   case 'h'  :Serial.print("**K3=1\n\r");
             digitalWrite(K3,HIGH);
             digitalWrite(LED3,  HIGH);
             break ;
   case 'i'  :Serial.print("**K3=0\n\r");
             digitalWrite(K3,LOW);
             digitalWrite(LED3,  LOW);
             break ;           
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
   case 'w'  :Serial.print("**LED2-at tx\n\r"); // BT INq and search
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
  case 'x'  :Serial.print("**FFT \n\r");
            double x;
             for( i=0; i < samples; i++){
              vReal[i]= analogRead(fftinputL); // fftinput=10, LED=11, is pwm output
              vImag[i]=0;
             }

            /* Print the results of the simulated sampling according to time */
                Serial.println("Data:");
             //   PrintVector(vReal, samples, SCL_TIME);
                FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
             //   Serial.println("Weighed data:");
            //   PrintVector(vReal, samples, SCL_TIME);
               FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
             //  Serial.println("Computed Real values:");
            //   PrintVector(vReal, samples, SCL_INDEX);
            //   Serial.println("Computed Imaginary values:");
           //    PrintVector(vImag, samples, SCL_INDEX);
               FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
            //   Serial.println("Computed magnitudes:");
             //  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
               //PrintVector(vReal, (samples >> 1), SCL_PLOT); // IDE uart plot
               x = FFT.MajorPeak(vReal, samples, samplingFrequency);
               Serial.print("FFTL=");
               Serial.println(x, 6);
               //-----------------------------------------------------
               //for right
               //
              for( i=0; i < samples; i++){
               vReal[i]= analogRead(fftinputR); // fftinputR=9, LED=11, is pwm output
               vImag[i]=0;
             }
                Serial.println("Data:");
              //  PrintVector(vReal, samples, SCL_TIME);
                FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
             //   Serial.println("Weighed data:");
            //   PrintVector(vReal, samples, SCL_TIME);
               FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
             //  Serial.println("Computed Real values:");
            //   PrintVector(vReal, samples, SCL_INDEX);
            //   Serial.println("Computed Imaginary values:");
           //    PrintVector(vImag, samples, SCL_INDEX);
               FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
            //   Serial.println("Computed magnitudes:");
             //  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
               //PrintVector(vReal, (samples >> 1), SCL_PLOT); // IDE uart plot
               x = FFT.MajorPeak(vReal, samples, samplingFrequency);
               Serial.print("FFTR=");
               Serial.println(x, 6);
             
             break ; 
   case 'y'  :Serial.print("**LED3-on\n\r");
             digitalWrite(LED3,  HIGH);   
             //led_blink(LED);
             break ;   
   case 'z'  :Serial.print("**test tx rx\n\r");
             // serial3 15Rx, 14Tx
             i=0;
             instr="";
             while (i <3) {     
             while (!Serial3.available()) {} // wait RX data input
             instr=Serial3.readString();
             if(instr.indexOf("BLE addr is:")>0) // find the first pwr
               {
                a=instr.indexOf("BLE addr is:");
                Serial.println("first pwr");
                Serial.println(instr.substring(a));
                break;
               }
              if(instr.indexOf("app_bt_key 1,5")>0) // find pwr
                   {
                    Serial.println("pwr");
                    break;
                   }
             if(instr.indexOf("app_bt_key 4,2")>0) // find V-
                   {
                    Serial.println("V-");
                    break;
                   }
             if(instr.indexOf("app_bt_key 2,2")>0) // find V+
                 {
                  Serial.println("v+");
                  break;
                 }
             if(instr.indexOf("app_anc_key")>0) // find ANC
                {
                  Serial.println("ANC");
                  break;
                }
             Serial.println(String(i,DEC));             
             i++;
             }
             Serial.println("#exit");
             instr="";             
             break;
  case 's'  :Serial.print("**FLT state\n\r");
             bool FTLState = digitalRead(FLT);
             Serial.print(FTLState);   
             Serial.write("\n");   
             led_blink(LED13);
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
        case '0': Serial.println("select 0\n\r");
                      mux_sel(0);
                      break;
        case '1': Serial.println("select 1\n\r");
                      mux_sel(1);
                      break;                        
        case '2': Serial.println("select 2\n\r");
                      mux_sel(2);
                      break;
        case '3': Serial.println("select 3\n\r");
                      mux_sel(3);
                      break; 
        case 'p': Serial.println("select PWR\n\r");
                      digitalWrite(SPWR,LOW);
                      digitalWrite(SVP,HIGH);
                      digitalWrite(SVN,HIGH);
                      digitalWrite(SANC,HIGH);
                      break; 
        case 'a': Serial.println("select ANC\n\r");
                      digitalWrite(SPWR,HIGH);
                      digitalWrite(SVP,HIGH);
                      digitalWrite(SVN,HIGH);
                      digitalWrite(SANC,LOW);
                      break; 
        case 'u': Serial.println("select V+\n\r");
                      digitalWrite(SPWR,HIGH);
                      digitalWrite(SVP,LOW);
                      digitalWrite(SVN,HIGH);
                      digitalWrite(SANC,HIGH);
                      break; 
        case 'd': Serial.println("select V-\n\r");
                      digitalWrite(SPWR,HIGH);
                      digitalWrite(SVP,HIGH);
                      digitalWrite(SVN,LOW);
                      digitalWrite(SANC,HIGH);
                      break;   
        case 'c': Serial.println("disable all keys\n\r");
                      digitalWrite(SPWR,HIGH);
                      digitalWrite(SVP,HIGH);
                      digitalWrite(SVN,HIGH);
                      digitalWrite(SANC,HIGH);
                      break;  
        case 'e': Serial.println("enable, SELEN_B=0\n\r");
                      digitalWrite(SELEN_B,LOW);
                      break;             
        case 'i': Serial.println("inhibit, SELEN_B=1\n\r");
                      digitalWrite(SELEN_B,HIGH);
                      break;     
       }
            
      }
 }
   if (i=='Q') {    //等到軟體串列埠RX收到右方板傳來'Y'字元
    led_blink(LED3);
    Serial.println("Hello!");  //左方板向PC傳送字串
    }
 
//delay(1000);
 //*/
}

 void led_blink(int i) {
  int k;
  for (k=0;k<3;k++){
  digitalWrite(i, HIGH);
  delay(500);
  digitalWrite(i, LOW);
  delay(500);
  }
  }

void setIO_default(){
  digitalWrite(SELEN_B,HIGH);
  digitalWrite(SPWR,HIGH);
  digitalWrite(SVP,HIGH);
  digitalWrite(SVN,HIGH);
  digitalWrite(SANC,HIGH);
}

void mux_sel(int i){
    switch(i){
      case 0: digitalWrite(S0,LOW);
              digitalWrite(S1,LOW);
              digitalWrite(SELEN_B,LOW);
              break;
      case 1: digitalWrite(S0,HIGH);
              digitalWrite(S1,LOW);
              digitalWrite(SELEN_B,LOW);
              break;
      case 2: digitalWrite(S0,LOW);
              digitalWrite(S1,HIGH);
              digitalWrite(SELEN_B,LOW);
              break;
      case 3: digitalWrite(S0,HIGH);
              digitalWrite(S1,HIGH);
              digitalWrite(SELEN_B,LOW);
              break;                                          
    }
}

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
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
