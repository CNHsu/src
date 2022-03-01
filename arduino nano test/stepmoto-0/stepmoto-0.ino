//*********************************************
//https://sites.google.com/site/csjhmaker/d-dong-li-pian/28byj-48-bu-jin-ma-da
//

#include <Stepper.h>


int t= 1000;
boolean a[]={1,1,0,0,0,0,0,1};
boolean b[]={0,1,1,1,0,0,0,0};
boolean c[]={0,0,0,1,1,1,0,0};
boolean d[]={0,0,0,0,0,1,1,1};

void setup() {
Serial.begin(38400);
pinMode(8,OUTPUT);
pinMode(9,OUTPUT);
pinMode(10,OUTPUT);
pinMode(11,OUTPUT);
}

void loop() {

for(int i=7; i>=0 ; i--){
digitalWrite(8,a[i]);
digitalWrite(9,b[i]);
digitalWrite(10,c[i]);
digitalWrite(11,d[i]);
delayMicroseconds(t);
Serial.println(i);

}
}
