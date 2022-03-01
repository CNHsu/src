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

// step 28byj-48, blue (A+), pink(B+),yellow (A-), orange(B-), red (com)
// connect to     M3-1(blue), M3-2(yellow), M3-GND(red), M3-4(orange),M3-5(pink)
//// 伺服器馬達, 紅色(VDD), 棕色(GND), 橙色(Signal)
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

// Create the motor shield object with the default I2C address (default 0x60)
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(4096, 2); // for 28BYJ set 4096
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(200, 1); // for DPW11 motor set 200
Servo myservo;  // create servo object to control a servo

int dt;

void setup() {
  Serial.begin(115200);           // set up Serial library at 9600 bps
  Serial.println("V2 Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz  
  myMotor->setSpeed(4000);  // 10 rpm
  myMotor2->setSpeed(10);  
  // servo
  myservo.attach(31,544,2400); // servo pin D31, calibrate, you can set any pin for servo
  myservo.write(200); // default degree
 // default dt
 dt=50;
   
}

void loop() {
  String instr="";
  String binstr="";
  char ch;
  int i,state;
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
    myMotor->step(2048, FORWARD, DOUBLE); // for 28BYJ DOUBLE is better
    myMotor2->step(100, FORWARD, MICROSTEP); // for DPW11 motor
    break;
  case 'b':
    myMotor->step(2048, BACKWARD, DOUBLE);
    myMotor2->step(100, BACKWARD, MICROSTEP); // for DPW11 motor
    break;
  case '1':
    //myMotor->onestep(FORWARD, MICROSTEP); // 100step 15mm,
    myMotor->step(20, FORWARD, DOUBLE); // MICROSTEP, 20step ~3mm
    myMotor2->step(10, FORWARD, MICROSTEP); // MICROSTEP, 20step ~3mm
    break;
  case '2':
    //myMotor->onestep(BACKWARD, MICROSTEP);
    myMotor->step(20, BACKWARD, DOUBLE);
    myMotor2->step(10, BACKWARD, MICROSTEP);
    break; 
  case '3':
   
   // myMotor->step(20, BACKWARD, DOUBLE);
    myMotor2->step(20, FORWARD,DOUBLE); //1
    delay(dt);
    myMotor2->step(20, BACKWARD,DOUBLE);
    delay(dt);
     myMotor2->step(20, FORWARD,DOUBLE); //2
    delay(dt);
     myMotor2->step(20, BACKWARD,DOUBLE);
    delay(dt);
    myMotor2->step(20, FORWARD,DOUBLE); //3
    delay(dt);
     myMotor2->step(20, BACKWARD,DOUBLE);

    break;     
  case 'd':
    Serial.println("Demo ");
    for(i=0;i<100;i++){
      Serial.write("[");Serial.print(i);Serial.write("] \n\r");
      Serial.println("forward ");
    myMotor->step(2048, FORWARD, DOUBLE); // for 28BYJ DOUBLE is better
    myMotor2->step(100, FORWARD, MICROSTEP);
    myMotor->release();
    myMotor2->release(); 
    delay(2000);
    Serial.println("backward ");
    myMotor->step(2048, BACKWARD, DOUBLE);
    myMotor2->step(100, BACKWARD, MICROSTEP);
    myMotor->release();
    myMotor2->release();
    Serial.println("server ");
    myservo.write(20);
    delay(100);
    myservo.write(90);
    delay(100);
    
    myservo.write(20);
    delay(100);
    myservo.write(90);
    delay(100);
    
    myservo.write(20);
    delay(100);
    myservo.write(90); 
    delay(60000);
    }
    break;
   case 'm':
         binstr="";
         binstr=instr.substring(1); 
         state=binstr.toInt();
         Serial.print("Servo run =");
         Serial.println(binstr);
         if(state >=0 && state <=180) {
          myservo.write(state);
          delay(100);
          myservo.write(90);
         }
         else Serial.println("outof servo range!!");
          break;  
   case 's':
         binstr="";
         binstr=instr.substring(1); 
         dt=binstr.toInt();
         Serial.print("Set delay =");
         Serial.println(dt);
         break;            
  }
  myMotor->release();
  myMotor2->release();  
  //delay(2000);
}
