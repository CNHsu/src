// 2019/3/5 arduino nano v3 for BES test tool
// 2019/3/12 revised some io definition
//  by CNHsu
int LED=13; // D13, LED or relay 2019/3/12
int A5V=A0; // 5v voltage
int A5C=A1; //5v current
int A4V=A2; //4v voltage
int A4C=A3; //4v current
// A4,5,6,7 no use
// A4=TD35, A5=TD34,A6=TD33, A7=TD32
int VBUS_EN=2;//D2
int BAT_EN=3; //D3
//D4 no use
int LED1=5;   //D5
int LED2=6;   //D6
int LED3=7;   //D7
int K3=8; //D8, relay K3, '1' on, '0' off
int K2=9; //D9, relay K2, '1' on, '0' off
int K1=13; //D13, LED and relay K1, '1' on, '0' off
int uA_SET1=10;//D10 for 5V source '1' uA, '0'mA
int uA_SET2=11;//D11 for 4V BAT source '1' uA, '0' mA
int FLT=12;//D12, input, '1' ok, '0' power fail

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
 /*
  Serial.write("PCB test tool 2019/03/05 \n\r"); 
  Serial.write("[1] 5V ON, [2] 5V OFF\n\r"); // VBUS_EN
  Serial.write("[3] 4V ON, [4] 4V OFF\n\r"); // BAT_EN
  Serial.write("==========================\n\r");
  Serial.write("[5] 5V voltage\n\r");       //A0
  Serial.write("[6] 5V current\n\r");       //A1
  Serial.write("[7] 4V voltage\n\r");       //A2
  Serial.write("[8] 4V current\n\r");       //A3
  Serial.write("==========================\n\r");
  Serial.write("[9] 5V mA,[a]=5V uA\n\r"); //uA_SET1
  Serial.write("[b] 4V mA,[c]=OFF\n\r");//uA_SET2
  Serial.write("==========================\n\r");
  Serial.write("[d] K1=1,[e] K1=0\n\r"); //uA_SET1
  Serial.write("[f] K2=1,[g] K2=0\n\r");//uA_SET2
  Serial.write("[h] K3=1,[i] K3=0\n\r");//uA_SET2
  
  Serial.write("[u/v] LED1, [w/x] LED2, [y/z] LED3\n\r");
  Serial.write("[s] FLT \n\r"); // input value
  Serial.write("[Q] quit \n\r");
  Serial.write(": \n\r");
  */
  while (!Serial.available()) {} // wait RX data input
  i=Serial.read();
  Serial.write(i);
  switch (i){
   case 'Q': break;
   case '1' : Serial.print("**5 ON\n\r");
             digitalWrite(VBUS_EN,HIGH);
             digitalWrite(LED, HIGH);
             break;
    case '2' : Serial.print("**5 OFF\n\r");
             digitalWrite(VBUS_EN,LOW);
             digitalWrite(LED, LOW);
             break;
     case '3' : Serial.print("**4V ON\n\r");
             digitalWrite(BAT_EN,HIGH);
             digitalWrite(LED, HIGH);
             break;
   case '4'  :Serial.print("**4 OFF\n\r");
             digitalWrite(BAT_EN,LOW);
             digitalWrite(LED,  LOW);
             break;           
   case '5' :Serial.print("**5 voltage\n");
             val=analogRead(A5V);
             Serial.write("5VV\n");
             Serial.print(val*0.00488);
             Serial.write("\n");
             digitalWrite(LED, HIGH);
             break;
   case '6'  :Serial.print("**5 current\n");
             val=analogRead(A5C);
             Serial.write("5VI\n");
             Serial.print(val*0.00488);
             Serial.write("\n");
             digitalWrite(LED,  LOW);
             break;             
   case '7'  :Serial.print("**4 voltage\n");
             val=analogRead(A4V);
             Serial.write("4VV\n");
             Serial.print(val*0.00488);   
             Serial.write("\n");
             digitalWrite(LED, HIGH);
             break ;            
   case '8'  :Serial.print("**4 current\n");
             val=analogRead(A4C);
             Serial.write("4VI\n");
             Serial.print(val*0.00488);   
             Serial.write("\n");
             digitalWrite(LED,  LOW);
             break ;  
   case '9'  :Serial.print("**5V mA\n\r");
             digitalWrite(uA_SET1,LOW);
             digitalWrite(LED,  LOW);
             break ; 
   case 'a'  :Serial.print("**5V uA\n\r");
             digitalWrite(uA_SET1,HIGH);
             digitalWrite(LED,  HIGH);
             break ;
   case 'b'  :Serial.print("**4V mA\n\r");
             digitalWrite(uA_SET2,LOW);
             digitalWrite(LED,  LOW);
             break ;   
   case 'c'  :Serial.print("**4V uA\n\r");
             digitalWrite(uA_SET2,HIGH);
             digitalWrite(LED,  HIGH);
             break ;            
   case 'd'  :Serial.print("**K1=1\n\r");
             digitalWrite(K1,HIGH);
             digitalWrite(LED,  HIGH);
             break ;
   case 'e'  :Serial.print("**K1=0\n\r");
             digitalWrite(K1,LOW);
             digitalWrite(LED,  LOW);
             break ;
   case 'f'  :Serial.print("**K2=1\n\r");
             digitalWrite(K2,HIGH);
             digitalWrite(LED,  HIGH);
             break ;
   case 'g'  :Serial.print("**K2=0\n\r");
             digitalWrite(K2,LOW);
             digitalWrite(LED,  LOW);
             break ;
   case 'h'  :Serial.print("**K3=1\n\r");
             digitalWrite(K3,HIGH);
             digitalWrite(LED,  HIGH);
             break ;
   case 'i'  :Serial.print("**K3=0\n\r");
             digitalWrite(K3,LOW);
             digitalWrite(LED,  LOW);
             break ;           
   case 'u'  :Serial.print("**LED1-on\n\r");
             digitalWrite(LED1,  HIGH);
             led_blink(LED);
             break ;  
   case 'v'  :Serial.print("**LED1-off\n\r");
             digitalWrite(LED1,  LOW);
             led_blink(LED);
             break ;               
   case 'w'  :Serial.print("**LED2-on\n\r");
             digitalWrite(LED2,  HIGH);   
             led_blink(LED);
             break ; 
  case 'x'  :Serial.print("**LED2-off\n\r");
             digitalWrite(LED2,  LOW);   
             led_blink(LED);
             break ; 
   case 'y'  :Serial.print("**LED3-on\n\r");
             digitalWrite(LED3,  HIGH);   
             led_blink(LED);
             break ;   
   case 'z'  :Serial.print("**LED3-off\n\r");
             digitalWrite(LED3,  HIGH);   
             led_blink(LED);
             break ;  
  case 's'  :Serial.print("**FLT state\n\r");
             bool FTLState = digitalRead(FLT);
             Serial.print(FTLState);   
             Serial.write("\n");   
             led_blink(LED);
             break ;  
  }
   if (i=='Q') {    //等到軟體串列埠RX收到右方板傳來'Y'字元
    led_blink(LED);
    Serial.println("Hello!");  //左方板向PC傳送字串
    }

 delay(2000);
}

 void led_blink(int i) {
  int k;
  for (k=0;k<5;k++){
  digitalWrite(i, HIGH);
  delay(500);
  digitalWrite(i, LOW);
  delay(500);
  }
  }
