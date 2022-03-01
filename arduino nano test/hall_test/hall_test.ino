//-----------------------------------------
// 2019/6/11 by CNHsu
// hall IC 44E 402
// '0' :磁鐵接近
// '1' : no

int hall=12;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(hall,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int status;
  status = digitalRead(hall);
  Serial.println(status);
  delay(500);
}
