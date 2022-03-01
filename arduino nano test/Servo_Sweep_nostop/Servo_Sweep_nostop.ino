/* Sweep
2019/8/5 from seeep example
pwm  3,5,6,9,10,11

2019/8/5: servo use 3,5,6,9
by CNHsu
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
Servo myservo3,myservo5,myservo6;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  //myservo.attach(9);  // attaches the servo on pin 9 to the servo object
 myservo.attach(9,544,2400); // calibrate
 myservo.write(90); // default degree
 myservo3.attach(3,544,2400); // calibrate
 myservo3.write(90); // default degree
 myservo5.attach(5,544,2400); // calibrate
 myservo5.write(90); // default degree
 myservo6.attach(6,544,2400); // calibrate
 myservo6.write(90); // default degree
  Serial.begin(38400);
  Serial.println("Hello Servo (m1,m2,m3,m4):");
  Serial.println("example m175: servo1 75degree");
}

void loop() {
  int state;
  char ch;
  String instr="";
  String binstr="";
  
while(!Serial.available()) {}  
 instr=Serial.readString();
 binstr="";
if (instr[0]=='m') { // m1=servo3,,m2=servo5,m3=servo=6,m4=servo9
  binstr=instr.substring(2);
  ch=instr[1];
}
else
{
  binstr=instr;
  ch='1'; // default servo3
}
 state=binstr.toInt();
 if (state >=0 && state <=180)
  {
    Serial.write("servo option:");
    Serial.write(ch);
    Serial.write("=");
    Serial.println(binstr);
    
    switch(ch)
    {
      case'1' : myservo3.write(state);
                delay(200);
                myservo3.write(90); // back to default 90degree
               break;
      case'2' : myservo5.write(state);
                delay(200);
                myservo5.write(90); // back to default 90degree                
               break;
      case'3' : myservo6.write(state);
                delay(200);
                myservo6.write(90); // back to default 90degree   
while(1){
    for (pos = 90; pos <= state; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo3.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
        myservo5.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
        myservo6.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = state; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo3.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
        myservo5.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
        myservo6.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }                           
}
               break;
      case'4' : myservo.write(state);
                delay(200);
                myservo.write(90); // back to default 90degree
               break;
    }
    
  } 
  
/* test code  
    Serial.write("servo option:");
    Serial.println(binstr);
    myservo.write(state);
    delay(200);
     myservo.write(90); // return to original

     myservo3.write(state);
     delay(200);
     myservo3.write(90); // return to original

     myservo5.write(state);
     delay(200);
     myservo5.write(90); // return to original

     myservo6.write(state);
     delay(200);
     myservo6.write(90); // return to original
 */ 


  
  for (pos = 90; pos <= state; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo6.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = state; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo6.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  
}

