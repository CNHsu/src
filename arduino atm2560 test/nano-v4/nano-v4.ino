//####################################################################
// for arduino nano 2019/09/16
// check BES & FFT
//#################################################################### 
// 1. check V, I
// 2. check freq.
// 3. check key log
// 4. check power fault
//  by CNHsu
//-----------------------------------------------------------
#include  <SoftwareSerial.h>  // atm256 use hw serial
#include <PWM.h>
#include <arduinoFFT.h>

SoftwareSerial Serial3(5, 6); // RX | TX
int LED13=13; // LED on board
int pwm=9; // pwm 1k
int pwm2=3; // pwm2
int A5V=A0; // 5v voltage
int A5C=A1; //5v currentr
int A4V=A2; //4v voltage
int A4C=A3; //4v current
int VBUS_EN=2;
int BAT_EN=4; 
int uA_SET1=10;// '1' mA, '0'uA, 2019/4/29 update
int uA_SET2=11;// '1' mA, '0' uA, 2019/4/29 update
int FLT=12;//input, '1' ok, '0' power fail
int CKLED=A4; // 2019/11/19 check led

int LED7=7;
int LED8=8; // LED control

int fftinputL=A7; // A9 for fft-L
int fftinputR=A5; // A10 for fft-R

int val=0;


//#######################################
// PWM
int32_t frequency =500; //freq. Hz
int32_t frequency2 =1000; //freq. Hz
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

unsigned long TIME_OUT=5000;
boolean newData= false;
const byte numChars =64;
char receivedChars[numChars];


void setup() {
  // put your setup code here, to run once:
  //PWM
  InitTimersSafe();
  SetPinFrequencySafe(pwm, frequency);
  SetPinFrequencySafe(pwm2, frequency2);
  
  Serial.begin(57600); // to mega2560
  Serial.write("3S checker board V1\n\r");
  pinMode(2,OUTPUT); // vbus 5V enable
  pinMode(3,OUTPUT); //PWM
   pinMode(4,OUTPUT);// 4V enable
   // 5,6 uart
  pinMode(7,OUTPUT); // LED
   pinMode(8,OUTPUT); // LED
  pinMode(9,OUTPUT); //PWM
  pinMode(10,OUTPUT); // u/mA set
  pinMode(11,OUTPUT); // u/mA set
  pinMode(12,INPUT); // FLT check
  pinMode(13,OUTPUT); // LED (no use)
//  Serial1.begin(38400); //HC-05 BT default AT command mode speed, 19Rx, 18Tx
  Serial3.begin(38400); // 5Rx,6Tx, software uart
//  Serial2.begin(9600); // 



}

void loop() {
  // put your main code here, to run repeatedly:
 //int control,chstate;
 char i,ch;
 String btrx,instr;
 String Rssi;

 static unsigned long startime =0;
 unsigned long now =millis();
 

 pwmWrite(pwm,128);
 pwmWrite(pwm2,128);

  instr="";
 // control=digitalRead(8);
 // chstate=control == HIGH ? LOW : HIGH;
 // digitalWrite(13,chstate); 

//////////////////////////////////////////////
 while (!Serial.available()) {} // wait RX data input
 instr=Serial.readString();
 //Serial.println(instr);
 //Serial.println(String(instr.length()));
 if(instr.length() == 3){ // 20190917 length include \n\r? 
 i=instr[0];
  switch (i){
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
   case '5' :Serial.print("**5 voltage ");   
             val=0;
             val=analogRead(A5V);
             Serial.write("5VV :");
             Serial.print(val*0.00488);
             //Serial.print(val*0.488);
             Serial.write("\n");
             break;
   case '6'  :Serial.print("**5 current ");
             val=0;
             val=analogRead(A5C);
             Serial.write("5VI :");
             //Serial.print(val*0.00488);
             Serial.print(val*0.488);
             Serial.write("\n");
             break;             
   case '7'  :Serial.print("**4 voltage ");
             val=0;
             val=analogRead(A4V);
             Serial.write("4VV :");
             Serial.print(val*0.00488);   
             //Serial.print(val*0.488);
             Serial.write("\n");
            // digitalWrite(LED13, HIGH);
             break ;            
   case '8'  :Serial.print("**4 current ");
             val=0;
             val=analogRead(A4C);
             Serial.write("4VI :");
             //Serial.print(val*0.00488);   
             Serial.print(val*0.488);
             Serial.write("\n");
           //  digitalWrite(LED13,  LOW);
             break ;  
   case '9'  :Serial.print("**5V uA\n\r"); // 2019/4/29 update mA to uA
             digitalWrite(uA_SET1,LOW);
            // digitalWrite(LED13,  LOW);
             break ; 
   case 'a'  :Serial.print("**5V mA\n\r"); // 2019/4/29 update uA to mA
             digitalWrite(uA_SET1,HIGH);
            // digitalWrite(LED13,  HIGH);
             break ;
   case 'b'  :Serial.print("**4V uA\n\r");// 2019/4/29 update mA to uA
             digitalWrite(uA_SET2,LOW);
            // digitalWrite(LED13,  LOW);
             break ;   
   case 'c'  :Serial.print("**4V mA\n\r");// 2019/4/29 update uA to mA
             digitalWrite(uA_SET2,HIGH);
            // digitalWrite(LED13,  HIGH);
             break ; 
   case 'd': Serial.print("**PWM D9=500,D3=1K \n\r");
              SetPinFrequencySafe(pwm, frequency);
              SetPinFrequencySafe(pwm2, frequency2);          
              break;
   case 'e': Serial.print("**PWM D9=1K,D3=500 \n\r");
              SetPinFrequencySafe(pwm, frequency2);
              SetPinFrequencySafe(pwm2, frequency);          
              break;
   case 'f'  :Serial.print("LED7=1 \n\r");
             digitalWrite(LED7,HIGH);            
             break ; 
   case 'g'  :Serial.print("LED7=0 \n\r");
             digitalWrite(LED7,LOW);            
             break ;                                          
   case 'h'  :Serial.print("LED8=1 \n\r");
             digitalWrite(LED8,HIGH);
             break ;              
   case 'i'  :Serial.print("LED8=0 \n\r");
             digitalWrite(LED8,LOW);
             break ;  
   case 'j'  :Serial.print("**CK LED ");
             val=0;
             for(i=0;i<10;i++){
             val=analogRead(CKLED);
             Serial.write("LED :");
             //Serial.print(val*0.00488);
             Serial.print(val*0.00488);
            // Serial.print(val);
             Serial.write("\n");
             delay(200);
             }
             break;                                   
  case 's'  :Serial.print("**FLT state\n");
             {
             bool FTLState = digitalRead(FLT);
             Serial.print(FTLState);   
             Serial.write("\n");   
            // led_blink(LED13);
             }
             break ; 
  case 'x'  :Serial.print("**FFT  ");
            {
              double vx;            
             for( i=0; i < samples; i++){
              vReal[i]= analogRead(fftinputL); // fftinput=10, LED=11, is pwm output
              vImag[i]=0;
             }
                Serial.print("Data: ");
                FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  
               FFT.Compute(vReal, vImag, samples, FFT_FORWARD); 
               FFT.ComplexToMagnitude(vReal, vImag, samples); 
               vx = FFT.MajorPeak(vReal, samples, samplingFrequency);
               Serial.print("FFTL= ");
               Serial.print(vx, 6);
               //-----------------------------------------------------
               //for right
               //
              for( i=0; i < samples; i++){
               vReal[i]= analogRead(fftinputR); // fftinputR=9, LED=11, is pwm output
               vImag[i]=0;
             }
                Serial.print(" Data: ");
                FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  
               FFT.Compute(vReal, vImag, samples, FFT_FORWARD); 
               FFT.ComplexToMagnitude(vReal, vImag, samples); 
               vx = FFT.MajorPeak(vReal, samples, samplingFrequency);
               Serial.print("FFTR= ");
               Serial.println(vx, 6);
            }
             break ; 
 case 'z' :    // Serial.print(" press key .....");
              //  Serial.print("5 ");  delay(500);
              //  Serial.print("4 ");  delay(500);
              //  Serial.print("3 ");  delay(500);
              //  Serial.print("2 ");  delay(500);
              //  Serial.print("1 ");  delay(500);            
              //  Serial.print("... ");
                now=millis(); 
                startime=now;
                while( (now-startime) < TIME_OUT){              
                now=millis();
                recwithend();
                if(newData == true){
                  showdata();
                Rssi=receivedChars;
                  if (checkdata("CHIP") == true) //Serial.print("#power on\n");
                      btrx="#power on ";
                  if (checkdata("BLE addr") == true)  {
                    //Serial.print("#BT : "); 
                    //Serial.print(Rssi);
                   // Serial.write("\n\r");
                   btrx=btrx+ "#BT : "+Rssi;                  
                  }
                  
                  if (checkdata("app_bt_key 1,5") == true) //Serial.print("#PWR\n"); 
                     btrx= btrx + "#PWR ";
                  else if (checkdata("app_bt_key 4,2") == true) //Serial.print("#V-\n");
                        btrx=btrx + "#V- ";
                  else if (checkdata("app_bt_key 2,2") == true) //Serial.print("#V+\n");
                         btrx=btrx +"#V+ ";
                  else if (checkdata("app_anc_key") == true) //Serial.print("#ANC\n");
                        btrx=btrx + "#ANC ";
                  else if (checkdata("app_bt_key 1,3") == true) //Serial.print("#PWR off\n"); 
                        btrx=btrx+ "#PWR off";
                }               
                  // showdata();

                }

               // Serial.println("!! check done");               
               Serial.print(btrx);
               Serial.print("#done\n\r");
               break;             
                                       
           
         }

    }
}


 void led_blink(int i) {
  int k;
  for (k=0;k<3;k++){
  digitalWrite(i, HIGH);
  delay(200);
  digitalWrite(i, LOW);
  delay(200);
  }
  }

void recwithend(){
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;

    while (Serial3.available() > 0 && newData == false) {
        rc = Serial3.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {           
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showdata() {
    if (newData == true) {
      // Serial.print("This just in ... ");
       // Serial.println(receivedChars);
        newData = false;
    }
}

boolean checkdata(String st){
  int k,j;
  int stleng;
  boolean match= false;
  
  stleng= st.length();
  k=0;
    for(j=0; j< numChars ;j++){
      
       if(st[k] == receivedChars[j]) {
           match=true; 
           k++;
          // Serial.println(st[k]);
          // Serial.println(k);
          // Serial.println(receivedChars[j]);
          }
        else if(k < stleng) {k=0; match=false;}
       }
   // clear char array

   receivedChars[0]= '\0';   
   return match;
 
}

