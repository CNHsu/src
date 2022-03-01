
int LED=13;
int P5V_V=A1; // 5v voltage
int P5V_C=A2; //5v current
int P4V_V=A3; //4v voltage
int P4V_C=A4; //4v current
int P5V_switch=2; //D2
int P4V_switch=3; //D3

int val=0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.write("wait input string, Q to quit:\n\r");
}

void loop() {
  // put your main code here, to run repeatedly:
 int a=65;
 char i;
  Serial.write(": \n\r"); 
  Serial.write("1. 5V ON\n\r");
  Serial.write("2. 5V OFF\n\r");
  Serial.write("3. 4V ON\n\r");
  Serial.write("4. 4V OFF\n\r");
  Serial.write("5. 5V voltage\n\r");
  Serial.write("6. 5V current\n\r");
  Serial.write("7. 4V voltage\n\r");
  Serial.write("8. 4V current\n\r");
  Serial.write("Q, quit \n\r");
  Serial.write(": \n\r");
  while (!Serial.available()) {} // wait RX data input
  i=Serial.read();
  Serial.write(i);
  switch (i){
   case 'Q': break;
   case '1' : Serial.print("**5 ON\n\r");
             digitalWrite(P5V_switch,HIGH);
             digitalWrite(LED, HIGH);
             break;
    case '2' : Serial.print("**5 OFF\n\r");
             digitalWrite(P5V_switch,LOW);
             digitalWrite(LED, LOW);
             break;
     case '3' : Serial.print("**4V ON\n\r");
             digitalWrite(P4V_switch,HIGH);
             digitalWrite(LED, HIGH);
             break;
   case '4'  :Serial.print("**4 OFF\n\r");
             digitalWrite(P4V_switch,LOW);
             digitalWrite(LED,  LOW);
             break;           
   case '5' :Serial.print("**5 voltage\n\r");
             val=analogRead(P5V_V);
             Serial.print(val*0.00488);
             digitalWrite(LED, HIGH);
             break;
   case '6'  :Serial.print("**5 current\n\r");
             val=analogRead(P5V_C);
             Serial.print(val*0.00488);
             digitalWrite(LED,  LOW);
             break;             
   case '7'  :Serial.print("**4 voltage\n\r");
             val=analogRead(P4V_V);
             Serial.print(val*0.00488);   
             digitalWrite(LED, HIGH);
             break ;            
   case '8'  :Serial.print("**4 current\n\r");
             val=analogRead(P4V_C);
             Serial.print(val*0.00488);   
             digitalWrite(LED,  LOW);
             break ;            
  }
   if (i=='Q') {    //等到軟體串列埠RX收到右方板傳來'Y'字元
    led_blink();
    led_blink();
    Serial.println("Hello!");  //左方板向PC傳送字串
    }

 delay(2000);
}

 void led_blink() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  }
