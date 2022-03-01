

//####################################################################
// for arduino nano 2020/02/14
// check BES & FFT & Uart log & Led
//#################################################################### 
// 1. check V, I
// 2. check freq.
// 3. check key log
// 4. check power fault
//  by CNHsu
//20191219
// 1. Led test
// 2. check uart log
// 3. return servo cmd 
//  by Eason
//20200110
// 1. check uart log
// 2. 16key check
// 3.TIME_OUT=2500
// 4.TIME_OUT2=7000
//  by Eason
//-----------------------------------------------------------
//20200214
// 1. Integration function
//  by Eason
//-----------------------------------------------------------
//20200325
// 1. CHANGE app_bt_key x,x
//  by Eason
#include  <SoftwareSerial.h>  // atm256 use hw serial
//#include <PWM.h> // 2019/10/2 replace PWM.h to tone library
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
int PCBA_ON_OFF=A6; // 2019/12/24 check pcba
int LED7=7;
int LED8=8; // LED control

int fftinputL=A7; // A9 for fft-L
int fftinputR=A5; // A10 for fft-R

int val=0,delay_time=0;//paring_led=0,anc_led=0,charge_led=0,user_led=0;
float a_max,a_min,save_light,paring_led=0,anc_led=0,charge_led=0,user_led=0,no_led=0;

float a_max_ad=3.1;

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
double save[samples];
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

unsigned long TIME_OUT=2500;
unsigned long TIME_OUT2=7000;
boolean newData= false;
const byte numChars =64;
char receivedChars[numChars];
char receivedChars_pwr[numChars];
char receivedChars_anc[numChars];
char receivedChars_vp[numChars];
char receivedChars_vd[numChars];


char cklu[4];


void setup() {
  // put your setup code here, to run once:
  //PWM
  InitTimersSafe();
  SetPinFrequencySafe(pwm, frequency);
  SetPinFrequencySafe(pwm2, frequency2);
  
  Serial.begin(38400); // to mega2560
  Serial.write("3S checker board V16\n\r");
  pinMode(2,OUTPUT); // vbus 5V enable
  pinMode(3,OUTPUT); //PWM2
   pinMode(4,OUTPUT);// 4V enable
   // 5,6 uart
  pinMode(7,OUTPUT); // LED
   pinMode(8,OUTPUT); // LED
  pinMode(9,OUTPUT); //PWM
  pinMode(10,OUTPUT); // u/mA set
  pinMode(11,OUTPUT); // u/mA set
  pinMode(12,INPUT); // FLT check
  pinMode(13,OUTPUT); // LED (no use)
  pinMode(LED_BUILTIN, OUTPUT);  //test
//  Serial1.begin(38400); //HC-05 BT default AT command mode speed, 19Rx, 18Tx
  Serial3.begin(38400); // 5Rx,6Tx, software uart
//  Serial2.begin(9600); // 
 pwmWrite(pwm,128);
 pwmWrite(pwm2,128);

 Serial.setTimeout(30);


}

void loop() {
  // put your main code here, to run repeatedly:
 //int control,chstate;
 char i,j,ch;
 String btrx,instr,btrx2;
 String Rssi;
 int first_run=0;
 static unsigned long startime =0;
 unsigned long now =millis();
 

// pwmWrite(pwm,128);
// pwmWrite(pwm2,128);
//noTone(pwm);
//noTone(pwm2);

  instr="";
 // control=digitalRead(8);
 // chstate=control == HIGH ? LOW : HIGH;
 // digitalWrite(13,chstate); 

//////////////////////////////////////////////
 delay(1);
 while (!Serial.available()) {} // wait RX data input
 instr=Serial.readString();
 //Serial.println(instr);
 //Serial.println(String(instr.length()));
 if(instr.length() < 7){ // 20190917 length include \n\r? 
 i=instr[0];

  switch (i){
   case '1' :Serial.print("ON\n\r");
             
             digitalWrite(VBUS_EN,HIGH);
             

             break;
   case '2' :Serial.print("OFF\n\r"); 
             
             digitalWrite(VBUS_EN,LOW);
             
            
             break;
   case '3' :Serial.print("ON\n\r"); 
            
             digitalWrite(BAT_EN,HIGH);
              

             break;
   case '4'  :Serial.print("OFF\n\r"); 
              digitalWrite(BAT_EN,LOW);
             

             break;           
   case '5' :  
             val=0;
             val=analogRead(A5V);
             Serial.write("5VV:");
             Serial.print(val*0.00488);
             //Serial.print(val*0.488);
             Serial.write("\n");
             break;
   case '6'  :
             val=0;
             val=analogRead(A5C);
             Serial.write("5VI:");
             //Serial.print(val*0.00488);
             Serial.print(val*0.488);
             Serial.write("\n");
             break;             
   case '7'  :
             val=0;
             val=analogRead(A4V);
             Serial.print(F("4VV:"));
             Serial.print(val*0.0039);   
             //Serial.print(val*0.488);
             Serial.write("\n");
            // digitalWrite(LED13, HIGH);
             break ;            
   case '8'  :
             val=0;
             val=analogRead(A4C);
             Serial.print(F("4VI:"));
             //Serial.print(val*0.00488);   
             Serial.print(val*0.39);
             Serial.print(F("\n"));
           //  digitalWrite(LED13,  LOW);
             break ;  
   case '9'  :Serial.print(F("uA\n\r")); // 2019/4/29 update mA to uA
             digitalWrite(uA_SET1,LOW);
            // digitalWrite(LED13,  LOW);
             break ; 
   case 'a'  :Serial.print(F("mA\n\r")); // 2019/4/29 update uA to mA
             digitalWrite(uA_SET1,HIGH);
            // digitalWrite(LED13,  HIGH);
             break ;
   case 'b'  :Serial.print(F("uA\n\r"));// 2019/4/29 update mA to uA
             digitalWrite(uA_SET2,LOW);
            // digitalWrite(LED13,  LOW);
             break ;   
   case 'c'  :Serial.print(F("mA\n\r"));// 2019/4/29 update uA to mA
             digitalWrite(uA_SET2,HIGH);
            // digitalWrite(LED13,  HIGH);
             break ; 
   case 'd': Serial.print(F("**PWM D9=500,D3=1K \n\r"));
              pwmWrite(pwm,128);
              pwmWrite(pwm2,128);
              SetPinFrequencySafe(pwm, frequency);
              SetPinFrequencySafe(pwm2, frequency2);
              delay(500);
              pwmWrite(pwm,128);
              pwmWrite(pwm2,128);
              SetPinFrequencySafe(pwm, frequency);
              SetPinFrequencySafe(pwm2, frequency2);            
              break;
   case 'e': Serial.print(F("**PWM D9=1K,D3=500 \n\r"));
              pwmWrite(pwm,128);
              pwmWrite(pwm2,128);
              SetPinFrequencySafe(pwm, frequency2);
              SetPinFrequencySafe(pwm2, frequency); 
              delay(500);
              pwmWrite(pwm,128);
              pwmWrite(pwm2,128);
              SetPinFrequencySafe(pwm, frequency2);
              SetPinFrequencySafe(pwm2, frequency);  
              break;
   case 'f'  :Serial.print(F("LED7=1 \n\r"));
              digitalWrite(LED7,HIGH);            
             break ; 
   case 'g'  :Serial.print(F("LED7=0 \n\r"));
             digitalWrite(LED7,LOW);            
             break ;                                          
   case 'h'  :Serial.print(F("LED8=1 \n\r"));
             digitalWrite(LED8,HIGH);
             break ;              
   case 'i'  :Serial.print(F("LED8=0 \n\r"));
             digitalWrite(LED8,LOW);
             break ; 
   case 'j'  :
//             Serial.print("**CK LED ");  //check led on/off
//             val=0;
//             for(i=0;i<6;i++){
//             val=analogRead(CKLED);
//             Serial.write("LED :");
//             Serial.print(val*0.00488);  //vin=4.78v => 4.78/1024=0.00466 v/bit
//             Serial.write("\n");
//             save[i]=val*0.00488;       // Save every piece of data
//                 if(save[i]>a_max){ 
//                     a_max=save[i];}
//                 else if(save[i]<a_min){
//                     a_min=save[i];}
//             delay(200);
//             }
//             Serial.print(F("maxmum photoresistor voltage is "));
//             Serial.print(a_max);
//             a_max=0;
//             a_min=0;
             check_led_status();
             break;   
   case 'k': 
             check_pcba_status();//check pcba on/off
             break;
   case 'q': Serial.print(F("no Tone\n\r"));  // stop pwm
             pwmWrite(pwm,0);
             pwmWrite(pwm2,0);
             break;                       
  case 's'  :Serial.print(F("**FLT state\n"));
             {
             bool FTLState = digitalRead(FLT);
             Serial.print(FTLState);   
             Serial.print(F("\n"));   
            // led_blink(LED13);
             }
             break ; 
  case 'x'  :
            {
              double vx;            
             for( i=0; i < samples; i++){
              vReal[i]= analogRead(fftinputL); // fftinput=10, LED=11, is pwm output
              vImag[i]=0;
             }
                
                FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  
               FFT.Compute(vReal, vImag, samples, FFT_FORWARD); 
               FFT.ComplexToMagnitude(vReal, vImag, samples); 
               vx = FFT.MajorPeak(vReal, samples, samplingFrequency);
               Serial.print(F("FFTL="));
               Serial.print(vx, 0);
               //-----------------------------------------------------
               //for right
               //
              for( i=0; i < samples; i++){
               vReal[i]= analogRead(fftinputR); // fftinputR=9, LED=11, is pwm output
               vImag[i]=0;
             }
                
                FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  
               FFT.Compute(vReal, vImag, samples, FFT_FORWARD); 
               FFT.ComplexToMagnitude(vReal, vImag, samples); 
               vx = FFT.MajorPeak(vReal, samples, samplingFrequency);
               Serial.print(F("FFTR="));
               Serial.println(vx, 0);
            }
             break ;
 case 't' :
              talk_mode_check();
              break;
 case 'z' :   
                now=millis(); 
                startime=now;
               j=instr[1];
               btrx2=instr.substring(2);
               delay(1);
               switch(j){
                        //////////////paba 1 check key log/////////
                        case '0'://servo 0
                                 Serial.print(F("%r0\n\t"));
                                 break;
                        case '1':
                                 Serial.print(F("%r1\n\t"));
                                 break;                                
                        case '2':
                                 Serial.print(F("%r2\n\t"));
                                 break;
                        case '3':
                                 Serial.print(F("%r3\n\t"));
                                 break;
                        //////////////paba 2  check key log/////////         
                        case '4':
                                 Serial.print(F("%r4\n\t"));
                                 break;
                        case '5':
                                 Serial.print(F("%r5\n\t"));
                                 break;                                
                        case '6':
                                 Serial.print(F("%r6\n\t"));
                                 break;
                        case '7':
                                 Serial.print(F("%r7\n\t"));
                                 break;
                        //////////////paba 3  check key log/////////         
                        case '8':
                                 Serial.print(F("%r8\n\t"));
                                 break;
                        case '9':
                                 Serial.print(F("%r9\n\t"));
                                 break;                                
                        case 'a':
                                 Serial.print(F("%r10\n\t"));
                                 break;
                        case 'b':
                                 Serial.print(F("%r11\n\t"));
                                 break;
                        //////////////paba 4  check key log/////////         
                        case 'c':
                                 Serial.print(F("%r12\n\t"));
                                 break;
                        case 'd':
                                 Serial.print(F("%r13\n\t"));
                                 break;                                
                        case 'e':
                                 Serial.print(F("%r14\n\t"));
                                 break;
                        case 'f':
                                 Serial.print(F("%r15\n\t"));
                                 break;     
                        //////////////bt address /////////         
                        case 'g':
                                 //Serial.print(F("%r0\n\t"));  //pcba1 pwr
                                 break;
                        case 'h':
                                // Serial.print(F("%r4\n\t"));  //pcba2 pwr
                                 break;                                
                        case 'i':
                                 //Serial.print(F("%r8\n\t"));  //pcba3 pwr
                                 break;
                        case 'j':
                                 //Serial.print(F("%r12\n\t")); //pcba4 pwr
                                 break; 
                        case 't':
                                 delay_time= btrx2.toInt();  //select delay time for check key log
                                 break;                                                
                    }
 
                while( (now-startime) < TIME_OUT){              
                now=millis();
                delay(delay_time);
             //   while (!Serial3.available()){}
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
                
                  if (checkdata("app_bt_key 4,6") == true  || checkdata("app_anc_key") == true) //Serial.print("#PWR\n"); 
                     btrx= btrx + "#ANC ";
                  if (checkdata("app_bt_key 4,5") == true) //Serial.print("#V-\n");
                        btrx=btrx + "#V- ";
                  else if (checkdata("app_bt_key 2,5") == true) //Serial.print("#V+\n");
                         btrx=btrx +"#V+ ";
//                  else if (checkdata("app_bt_key 1,5") == true) //Serial.print("#PWR off\n"); 
//                        btrx=btrx+ "#PWR off";
                }               
                  // showdata();

                }

               // Serial.println("!! check done"); 
                            
               Serial.print(btrx+"#done\n\r");
               Serial.flush();
               
               
               break;
 case'm':  
             //////////////paba 1~4 check key log///////// 
                     check_key_log();
                     break;
            //////////////paba 1~4 check key log/////////             
         }

    }
}

//
//void servo_log_check(){
//
//  Serial.print("%r0\n\t");
//  
//  }

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
            receivedChars[ndx-1] = '\0'; // terminate the string
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
///////////////////////////////
void  check_key_log(){
      static unsigned long startime =0;
      unsigned long now =millis();
      int re1=0;int re2=0;int re3=0;int re4=0;
      char pwr_off_statue;
      cklu[0]='\0';cklu[1]='\0';cklu[2]='\0';cklu[3]='\0';
      now =millis(); 
      startime=now;
      while( (now-startime) < TIME_OUT2){  
      now =millis();            
      delay(delay_time);
      recwithend();
            if(newData == true){
              showdata();
                 if  (re1==0 && checkdata("CHIP") == true || checkdata("app_bt_key 1,5") == true){ //Serial.print("#power on\n");
                     cklu[0]='1'; re1=1;}
                 else {
                      switch(re1){
                                  case 0: cklu[0]='0'; break;
                                  case 1: cklu[0]='1'; break;
                                 }
                       }
//                 if  (re2==0 && checkdata("app_anc_key") == true){//Serial.print("#ANC\n");
//                        cklu[1]='1'; re2=1;}
//                 else {
//                      switch(re2){
//                                  case 0: cklu[1]='0'; break;
//                                  case 1: cklu[1]='1'; break;
//                                 }
//                       }
                 if  (re2==0 && checkdata("app_bt_key 2,5")== true){ //Serial.print("#V+\n");
                      cklu[1]='1'; re2=1;}
                 else {
                      switch(re2){
                                  case 0: cklu[1]='0'; break;
                                  case 1: cklu[1]='1'; break;
                                 }
                       }
                 if  (re3==0 && checkdata("app_bt_key 4,5") == true ){ //Serial.print("#V-\n");
                      cklu[2]='1'; re3=1;}
                 else {
                      switch(re3){
                                  case 0: cklu[2]='0'; break;
                                  case 1: cklu[2]='1'; break;
                                }
                       }
                       cklu[3]='\0';
               }
      }
           while (!Serial.available()) {}
           delay(1);
           if(re4==0){
           Serial.print(cklu);}
           else{
           Serial.print(pwr_off_statue);}
           Serial.print(F("\n\r"));           
           Serial.readString();
}  
/////////////////////////////////////////////////////
void check_led_status(){
  int k;
    val=0;
    anc_led=0;
    paring_led=0;
    charge_led=0; 
    no_led=0;
    for(int j=0;j<5;j++){
      save[j]='\0';
      }
   for(k=0;k<6;k++){
       val=analogRead(CKLED);
//       Serial.write("LED:");
//       Serial.print(val*0.00488);  //vin=4.78v => 4.78/1024=0.00466 v/bit       //debug
//       Serial.write("\n");
       save_light=val*0.00488;
       save[k]=save_light;
       
       if(save[k]>a_max_ad){ 
         a_max_ad=save[k];}       
             
       if(k>4 && save[k]<=0.1 &&save[k-1]<=0.1 && save[k-2]<=0.1 && save[k-3]<=0.1 && save[k-4]<=0.1&& save[k-5]<=0.1 ){
        no_led++;
        }
       else{
             if( save_light>=a_max_ad-0.43){ // abs(save[k]-save[k-1])>2.85 ||
                anc_led++;
             }
                  else{ 
                        if (k>0 && abs(save[k]-save[k-1])<=0.2&& save_light<=1.6){
                                  charge_led++;}
                        else{
                          //  if(k>0 && 1.07>abs(save[k]-save[k-1])>0.2 && save_light<3.2){
                                 paring_led++; 
                           //    }
                               
                        }
                    }
              }  
              delay(200);  
                                  
   }
//   Serial.print("max:");
//   Serial.print(a_max_ad);   //debug
//   Serial.print("\n\r");
     if(no_led>=1){Serial.print(F("4\n\r"));}//led is no 
     else{
        if(anc_led>=1){
          Serial.print(F("1\n\r"));//led is anc on 
          }
        else {
            if(charge_led>=3){
              Serial.print(F("3\n\r"));//led is charging 
              }
             else
             Serial.print(F("2\n\r"));//led is paring mode 
        }
        }
          }
/////////////////////////////////////////////////
void check_pcba_status(){ //20191224
  int k;
   val=0;
   a_max=0;
   a_min=0;
   for(k=0;k<4;k++){
       val=analogRead(PCBA_ON_OFF);
//       Serial.write("LED :");
//       Serial.print(val*0.00488);  //vin=4.78v => 4.78/1024=0.00466 v/bit
//       Serial.write("\n");
        save[k]=val*0.00488;       // Save every piece of data
       if(save[k]>a_max){ 
           a_max=save[k];}
       else if(save[k]<a_min){
           a_min=save[k];}
   }

      if(a_max>1){
        digitalWrite(VBUS_EN,LOW);
        delay(50);
        digitalWrite(VBUS_EN,LOW);
        delay(50);
        digitalWrite(BAT_EN,LOW);
        delay(50);
        digitalWrite(BAT_EN,LOW);
        Serial.print(F("2\n\r")); //pcba is not put on
        }
      else{
        digitalWrite(VBUS_EN,LOW);
        delay(50);
        digitalWrite(VBUS_EN,LOW);
        delay(50);
        digitalWrite(BAT_EN,LOW);
        delay(50);
        digitalWrite(BAT_EN,LOW);
        Serial.print(F("1\n\r")); // pcba has been put on
        }
      
  }
 /////////////////////////////////////////////////
void talk_mode_check(){
             String btrx5,Rssi;
             static unsigned long startime =0;
             unsigned long now =millis();
                while( (now-startime) < TIME_OUT){              
                now=millis();
                delay(delay_time);
             //   while (!Serial3.available()){}
                recwithend();
                if(newData == true){
                  
                  showdata();
                Rssi=receivedChars;
                  if (checkdata("CHIP") == true) //Serial.print("#power on\n");
                      btrx5="#power on ";
                  if (checkdata("BLE addr") == true)  {
                    //Serial.print("#BT : "); 
                    //Serial.print(Rssi);
                   // Serial.write("\n\r");
                   btrx5=btrx5+ "#BT : "+Rssi;                  
                  }
                
//                  if (checkdata("app_bt_key 1,5") == true) //Serial.print("#PWR\n"); 
//                     btrx= btrx + "#PWR ";
                  if (checkdata("app_bt_key 4,2") == true) //Serial.print("#V-\n");
                        btrx5=btrx5 + "#V- ";
                  else if (checkdata("app_bt_key 2,2") == true) //Serial.print("#V+\n");
                         btrx5=btrx5 +"#V+ ";
                  else if (checkdata("app_anc_key") == true) //Serial.print("#ANC\n");
                        btrx5=btrx5 + "#ANC ";
                  else if (checkdata("app_bt_key 1,3") == true) //Serial.print("#PWR off\n"); 
                        btrx5=btrx5+ "#PWR off";
                }               
                  // showdata();

                }
               while (!Serial.available()) {}                       
               Serial.print(btrx5);
               Serial.write("#down");
               Serial.flush();  
  }
