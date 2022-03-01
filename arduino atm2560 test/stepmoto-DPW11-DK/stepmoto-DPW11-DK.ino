//*********************************************
//https://sites.google.com/site/csjhmaker/d-dong-li-pian/28byj-48-bu-jin-ma-da
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

#include <AFMotor.h>

AF_Stepper motor(200, 2);  //48步馬達接第2組接口 1 for M!/M2, 2 for M3/M4

void setup() {
  Serial.begin(115200);
  Serial.println("Stepper test!");
  motor.setSpeed(20);  //轉速 PWM voltage, max 255=5V
}

void loop() {
  String instr="";
  char ch;
  motor.release();
  delay(2000);

while(!Serial.available()) {}
instr=Serial.readString();
ch=instr[0];
  switch(ch){
   case 'f':
     Serial.println("Interleave FORWARD");
     motor.step(100, FORWARD, MICROSTEP);  //50 ~0.4cm,SINGLE,INTERLEAVE,DOUBLE,MICROSTEP
     delay(500);
     break;
   case 'b':
     Serial.println("Interleave BACKWARD");
     motor.step(100, BACKWARD, INTERLEAVE);  //反轉4096為360度一圈
     delay(500);
     break;
  }
}

