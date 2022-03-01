
int LED=13;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.write("wait input string, Q to quit:\n");
}

void loop() {
  // put your main code here, to run repeatedly:
 int a=65;
 //String i;
 char i;
  Serial.write("test\n");
  led_blink(); 
 while (!Serial.available()) {} // wait RX data input
 //i=Serial.readString();
   i=Serial.read();
  Serial.print(i);

   if (i=='Q') {    //等到軟體串列埠RX收到右方板傳來'Y'字元
    led_blink();
    led_blink();
    Serial.println("Hello!");  //左方板向PC傳送字串
    }

 delay(2000);
}

 void led_blink() {
  digitalWrite(LED, HIGH);
  delay(300);
  digitalWrite(LED, LOW);
  delay(300);
  }
