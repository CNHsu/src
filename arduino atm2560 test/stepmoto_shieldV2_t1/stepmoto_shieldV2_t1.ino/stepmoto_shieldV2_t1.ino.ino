/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438

//20191001 CN
https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/install-software
// SDA, SCL
// 5V, GND, VIN
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address (default 0x60)
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  Serial.begin(115200);           // set up Serial library at 9600 bps
  Serial.println("V2 Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(10);  // 10 rpm   
}

void loop() {
  String instr="";
  char ch;
  while(!Serial.available()) {}
  instr=Serial.readString();
  ch=instr[0];
  switch(ch){
  
 // Serial.println("Single coil steps");
 // myMotor->step(10, FORWARD, SINGLE); 
 // myMotor->step(10, BACKWARD, SINGLE); 
 // delay(2000);
 // Serial.println("Double coil steps");
 // myMotor->step(10, FORWARD, DOUBLE); 
 // myMotor->step(10, BACKWARD, DOUBLE);
 // delay(2000);
//  Serial.println("Interleave coil steps");
 // myMotor->step(10, FORWARD, INTERLEAVE); 
//  myMotor->step(10, BACKWARD, INTERLEAVE); 
 // delay(2000);
 case 'f':
    // Serial.println("Microstep steps");
    myMotor->step(100, FORWARD, MICROSTEP); 
    break;
  case 'b':
    myMotor->step(100, BACKWARD, MICROSTEP);
    break;
  case '1':
    //myMotor->onestep(FORWARD, MICROSTEP); // 100step 15mm,
    myMotor->step(20, FORWARD, MICROSTEP); // MICROSTEP, 20step ~3mm
    break;
  case '2':
    //myMotor->onestep(BACKWARD, MICROSTEP);
    myMotor->step(20, BACKWARD, MICROSTEP);
    break;    
  }
  myMotor->release();  
  delay(2000);
}
