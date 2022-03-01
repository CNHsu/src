//*********************************************
//https://sites.google.com/site/csjhmaker/d-dong-li-pian/28byj-48-bu-jin-ma-da
// step 28byj-48, blue (A+), pink(B+),yellow (A-), orange(B-), red (com)
// stepper.h stepper myStepper(steprevolution, A+, A-, B+,B-)
//https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/resources
//// 伺服器馬達, 紅色(VDD), 棕色(GND), 橙色(Signal)
//http://makeitanywhere.org/nyc/index.php?option=com_content&view=article&id=6&Itemid=132#!DK_Motorshield_V1_Top_B
// KD kit
//  PWM 1A D9
// PWM 1B  D10
// PWM 2A D11
// DIR LATCH D12
// DIR_SER D8
// DIR EN D7
// PWM 0A D6
// PWM 0B D5
// DIR CLK D4
// PWM 2B D3
//
// 74-595 shift register
//激磁順序(２相激磁)
//
//A+ A- B+ B-
//1   0   0   1
//1   1   0   0
//0   1   1   0
//0   0   1   1

// code for latch
// bit7-- 3A
// bit6.. 2A
//        1A
//        1B
//        2B
//        4A
//        3B
// bit0-- 4B
// use DK motor shield v1.0
// 


//#include <AFMotor.h>

//#include <Stepper.h>
//const int stepsPerRevolution = 48; //DPW11 motor 

#define si 8
#define rck 12 // latch
#define sck 4
#define dir_en 7
#define M3en 6 // analogwrite as PWM to control
#define M4en 5
#define M1en 11
#define M2en 3


//AF_Stepper motor(200, 2);  //48步馬達接第2組接口 1 for M!/M2, 2 for M3/M4

//byte val[]={0xd1,0xca,0x2e,0x35}; // ok
byte val[]={0x1b,0xd1,0xe4,0x2e};
//byte val[]={0xa0,0xe4,0x44,0x56,0x12,0x1b,0x09,0xb9}; // not work
#define index 3
#define voltage  90// analogWrite to Mxen motor voltage
#define motospeed 15 


void setup() {
  Serial.begin(115200);
  Serial.println("Stepper shift reg test");
  //motor.setSpeed(20);  //轉速 PWM voltage, max 255=5V
  pinMode(si, OUTPUT);
  pinMode(rck, OUTPUT);
  pinMode(sck, OUTPUT);
  pinMode(dir_en, OUTPUT);
  pinMode(M1en, OUTPUT);
  pinMode(M2en, OUTPUT);
  pinMode(M3en, OUTPUT);
  pinMode(M4en, OUTPUT);
  
}

void loop() {
  String instr="";
  char ch;
  int i;
  //motor.release();
  digitalWrite(dir_en, HIGH);
  motoenable(false); 
  delay(2000);

while(!Serial.available()) {}
instr=Serial.readString();
ch=instr[0];
  switch(ch){
   case 'f':
     Serial.println("Interleave FORWARD");    
    motosteps(10,true); //forward
     delay(500);
     break;
   case 'b':
     Serial.println("Interleave BACKWARD");
     motosteps(10,false); // backward
     delay(500);
     break;
    case 'a':
     Serial.println("loop");
     for(i=0; i<20;i++){
     motosteps(10,false); // backward
     delay(500);
      motosteps(10,true); // forward
     delay(1000);
     }
     break;    
     
  }
}



void latch_tx(byte value) {
  digitalWrite(rck, LOW);
  shiftOut(si,sck, MSBFIRST, value);
  digitalWrite(rck,HIGH);
}

void motoenable(boolean state) {
  if(state==true){
     analogWrite(M1en,voltage);
      analogWrite(M2en,voltage);
      analogWrite(M3en,voltage);
      analogWrite(M4en,voltage);
      digitalWrite(dir_en, LOW);
  } else {
      analogWrite(M1en,LOW);
      analogWrite(M2en,LOW);
      analogWrite(M3en,LOW);
      analogWrite(M4en,LOW);
      digitalWrite(dir_en, HIGH);    
  }
  
}

/*
void motoenable(boolean state) {
  if(state==true){
      digitalWrite(M1en,HIGH);
      digitalWrite(M2en,HIGH);
      digitalWrite(M3en,HIGH);
      digitalWrite(M4en,HIGH);
      digitalWrite(dir_en, LOW);
  } else {
      digitalWrite(M1en,LOW);
      digitalWrite(M2en,LOW);
      digitalWrite(M3en,LOW);
      digitalWrite(M4en,LOW);
      digitalWrite(dir_en, HIGH);    
  }
  
}
*/
void motosteps(int step, boolean dir) {
  int i;
  int k;
   motoenable(true);
    for (k=0;k<= step;k++) {
     if (dir == true) {    
       for (i=0;i<=index;i++) {
         latch_tx(val[i]);
         delay(motospeed);
        }
     }
     else {
         for (i=index;i>=0;i--) { 
         latch_tx(val[i]);
         delay(motospeed);
        } 
     }
    }
    motoenable(false);

}




