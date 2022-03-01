
int t;

void setup() {
  // put your setup code here, to run once:
  t=0;
Serial1.begin(115200);
while(!Serial1){ if(t>5) break;delay(1000); t++;};
Serial1.println("This is serial1");
 t=0;
SerialUSB.begin(115200);
while(!SerialUSB){ if(t>5) break;delay(1000); t++;};
SerialUSB.println("This is serialUSB");
 t=0;
Serial.begin(115200);
while(!Serial){ if(t>5) break;delay(1000); t++;};
Serial.println("This is serial");
}

void loop() {
  // put your main code here, to run repeatedly:
  
Serial1.println("This is serial1"); Serial1.println(t); t++;
delay(1000);
SerialUSB.println("This is serialUSB");SerialUSB.println(t); t++;
delay(1000);
Serial.println("This is serial");Serial.println(t); t++;
delay(1000);
}
