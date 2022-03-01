// 2019/3/5 arduino nano v3 for BES test tool
// 2019/3/12 revised some io definition
// 2019/3/27 modified V/I output value after real meeter check
// 2019/4/29 Q1,Q3 modify from nmos to pmos.... reverse uA_SET
// 2019/5/28 include HC-05 BT module
// HC-05 module need pre-setting
// AT+ROLE=1
// AT+INQM=1,5,48
// AT+CLASS=240404
// 2019/6/6 update command set for HC-05
// control HC-05 PIO2-8 & PIO11
// "PIO=2,1" // port-2 hight,
// "PIO=7,0" // port-7 low
// will MPIO show settting status
//  by CNHsu
//-----------------------------------------------------------
#include  <SoftwareSerial.h> 
SoftwareSerial BTSerial(5, 6); // RX | TX

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
int LED1=5;   //D5 2019/5/29 use for BT
int LED2=6;   //D6 2019/5/29 use for BT
int LED3=7;   //D7
int K3=8; //D8, relay K3, '1' on, '0' off
int K2=9; //D9, relay K2, '1' on, '0' off
int K1=13; //D13, LED and relay K1, '1' on, '0' off
int uA_SET1=10;//D10 for 5V source '1' mA, '0'uA, 2019/4/29 update
int uA_SET2=11;//D11 for 4V BAT source '1' mA, '0' uA, 2019/4/29 update
int FLT=12;//D12, input, '1' ok, '0' power fail

int val=0;
char *endptr;
char aRssi[10];
int cnt;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.write("wait input string, Q to quit:\n\r");
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
//  pinMode(5,OUTPUT);
//  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,INPUT);
  pinMode(13,OUTPUT);
  BTSerial.begin(38400); //HC-05 BT default AT command mode speed 
}

void loop() {
  // put your main code here, to run repeatedly:
 int a=65;
 char i,ch;
 String btrx,instr;
 String Rssi;
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
  Serial.write("[9] 5V uA,[a]=5V mA\n\r"); //uA_SET1, 2019/4/29 update
  Serial.write("[b] 4V uA,[c]=4V mA\n\r");//uA_SET2, 2019/4/29 update
  Serial.write("==========================\n\r");
  Serial.write("[d] K1=1,[e] K1=0\n\r"); //uA_SET1
  Serial.write("[f] K2=1,[g] K2=0\n\r");//uA_SET2
  Serial.write("[h] K3=1,[i] K3=0\n\r");//uA_SET2
  
  Serial.write("[u] BT reset/init/status/inqm/class\n\r");
  Serial.write(" [v] BT inq (cnt=loop)\n\r");
  Serial.write("[s] FLT \n\r"); // input value
  Serial.write("[Q] quit \n\r");
  Serial.write(": \n\r");
  */
  while (!Serial.available()) {} // wait RX data input
 // i=Serial.read();
 // Serial.write(i);
 instr=Serial.readString();
 Serial.println(instr);
 Serial.println(String(instr.length()));
 if(instr.length() == 1){
 i=instr[0];
  switch (i){
   case 'Q': break;
   case '1' : Serial.print("**5 ON\n\r");
             digitalWrite(VBUS_EN,HIGH);
             digitalWrite(LED3, HIGH);
             break;
    case '2' : Serial.print("**5 OFF\n\r");
             digitalWrite(VBUS_EN,LOW);
             digitalWrite(LED3, LOW);
             break;
     case '3' : Serial.print("**4V ON\n\r");
             digitalWrite(BAT_EN,HIGH);
             digitalWrite(LED3, HIGH);
             break;
   case '4'  :Serial.print("**4 OFF\n\r");
             digitalWrite(BAT_EN,LOW);
             digitalWrite(LED3,  LOW);
             break;           
   case '5' :Serial.print("**5 voltage\n");
             val=analogRead(A5V);
             Serial.write("5VV\n");
             Serial.print(val*0.00488);
             //Serial.print(val*0.488);
             Serial.write("\n");
             digitalWrite(LED3, HIGH);
             break;
   case '6'  :Serial.print("**5 current\n");
             val=analogRead(A5C);
             Serial.write("5VI\n");
             //Serial.print(val*0.00488);
             Serial.print(val*0.488);
             Serial.write("\n");
             digitalWrite(LED3,  LOW);
             break;             
   case '7'  :Serial.print("**4 voltage\n");
             val=analogRead(A4V);
             Serial.write("4VV\n");
             Serial.print(val*0.00488);   
             //Serial.print(val*0.488);
             Serial.write("\n");
             digitalWrite(LED3, HIGH);
             break ;            
   case '8'  :Serial.print("**4 current\n");
             val=analogRead(A4C);
             Serial.write("4VI\n");
             //Serial.print(val*0.00488);   
             Serial.print(val*0.488);
             Serial.write("\n");
             digitalWrite(LED3,  LOW);
             break ;  
   case '9'  :Serial.print("**5V uA\n\r"); // 2019/4/29 update mA to uA
             digitalWrite(uA_SET1,LOW);
             digitalWrite(LED3,  LOW);
             break ; 
   case 'a'  :Serial.print("**5V mA\n\r"); // 2019/4/29 update uA to mA
             digitalWrite(uA_SET1,HIGH);
             digitalWrite(LED3,  HIGH);
             break ;
   case 'b'  :Serial.print("**4V uA\n\r");// 2019/4/29 update mA to uA
             digitalWrite(uA_SET2,LOW);
             digitalWrite(LED3,  LOW);
             break ;   
   case 'c'  :Serial.print("**4V mA\n\r");// 2019/4/29 update uA to mA
             digitalWrite(uA_SET2,HIGH);
             digitalWrite(LED3,  HIGH);
             break ;            
   case 'd'  :Serial.print("**K1=1\n\r");
             digitalWrite(K1,HIGH);
             digitalWrite(LED3,  HIGH);
             break ;
   case 'e'  :Serial.print("**K1=0\n\r");
             digitalWrite(K1,LOW);
             digitalWrite(LED3,  LOW);
             break ;
   case 'f'  :Serial.print("**K2=1\n\r");
             digitalWrite(K2,HIGH);
             digitalWrite(LED3,  HIGH);
             break ;
   case 'g'  :Serial.print("**K2=0\n\r");
             digitalWrite(K2,LOW);
             digitalWrite(LED3,  LOW);
             break ;
   case 'h'  :Serial.print("**K3=1\n\r");
             digitalWrite(K3,HIGH);
             digitalWrite(LED,  HIGH);
             break ;
   case 'i'  :Serial.print("**K3=0\n\r");
             digitalWrite(K3,LOW);
             digitalWrite(LED3,  LOW);
             break ;           
   case 'u'  :Serial.print("**LED1-BT INIT\n\r");
              Serial.write("AT+RESET\n\r ");
              BTSerial.print("AT+RESET\n\r ");
            delay(1000);
            while(BTSerial.available())  // read BT wait 'ok'
               {                 
                 delay(10);
                  ch=BTSerial.read();
                  if(ch=='\r') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART
                btrx= "";                              
              Serial.write("AT+INIT\n\r ");
              BTSerial.print("AT+INIT\n\r ");
            delay(1000);
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='\r') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART   
                btrx= ""; 
              Serial.write("AT+state\n\r ");
              BTSerial.print("AT+state\n\r ");
             delay(1000);
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='\r') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART
                btrx= "";  
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='\r') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART
                btrx="";
             Serial.write("BT INQM\n\r ");
             BTSerial.print("AT+INQM=1,1,3\n\r ");
             delay(1000);
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART
                btrx="";  
             Serial.write("BT class\n\r ");
             BTSerial.print("AT+class=240404\n\r ");
             delay(1000);
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART   
             break ;  
   case 'v'  :Serial.print("**LED1-BT rx: scan\n\r"); // BT initial
             Serial.write("BT scan\n\r ");
             cnt=0;
             while(cnt < 10){ // !!!!!!! endless loop
              cnt++;
              btrx="";
              Rssi="";
             BTSerial.print("AT+INQ\n\r ");
             delay(1000);
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='\r') break;
                  btrx += ch;                  
               }               
               // Serial.println(btrx); // out to UART
                val=btrx.length();
             //   Serial.println(String(val));
                if(val > 28) {
                  Serial.println(btrx); // out to UART
                   Rssi=btrx.substring(val-4,val);
                   Rssi.toCharArray(aRssi,10);                   
                   val= strtoul(aRssi,&endptr,16);
                   Serial.println(Rssi); // out to UART
                   Serial.println("Rssi="+String(val));
                }
                Serial.println("==>"+String(cnt));
               // delay(1000);           
             }
             Serial.println("---end scan ---\n\r");
             break ; 
   case 'w'  :Serial.print("**LED2-at tx\n\r"); // BT INq and search
              Serial.write("AT inqm\n\r ");
              BTSerial.print("AT+INQM\n\r ");// inq one device time out 12sec
//              BTSerial.listen();
             btrx="";
             delay(200);
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART 
              Serial.write("AT bind\n\r ");
              BTSerial.print("AT+BIND=1119,15,123495\n\r ");
             delay(200);
             btrx="";
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART 
                delay(500);
              while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART                      
             break ; 
  case 'x'  :Serial.print("**LED2-off\n\r");
             //digitalWrite(LED2,  LOW);   
             //led_blink(LED);
                delay(500);
                btrx="";
              while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART                
             break ; 
   case 'y'  :Serial.print("**LED3-on\n\r");
             digitalWrite(LED3,  HIGH);   
             //led_blink(LED);
             break ;   
   case 'z'  :Serial.print("**LED3-off\n\r");
             digitalWrite(LED3,  LOW);   
             //led_blink(LED);
             break ;  
  case 's'  :Serial.print("**FLT state\n\r");
             bool FTLState = digitalRead(FLT);
             Serial.print(FTLState);   
             Serial.write("\n");   
             led_blink(LED2);
             break ;  
  }
 }
 else {
   if (instr[0]=='p') {
    btrx="";
    btrx="AT+"+instr+"\n\r ";
    Serial.print("**HC-05 PIO\n\r"); // BT INq and search
    Serial.print(btrx);
    BTSerial.print(btrx);// inq one device time out 12sec
    btrx="";
             delay(200);
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART       
    BTSerial.print("AT+MPIO\n\r ");// inq one device time out 12sec
    btrx="";
             delay(200);
             while(BTSerial.available())  // read BT wait 'ok'
               {                 
                  delay(10);
                  ch=BTSerial.read();
                  if(ch=='13') break;
                  btrx += ch;
               } 
                Serial.println(btrx); // out to UART         
      }
 }
   if (i=='Q') {    //等到軟體串列埠RX收到右方板傳來'Y'字元
    led_blink(LED);
    Serial.println("Hello!");  //左方板向PC傳送字串
    }
 
 delay(2000);
}

 void led_blink(int i) {
  int k;
  for (k=0;k<3;k++){
  digitalWrite(i, HIGH);
  delay(500);
  digitalWrite(i, LOW);
  delay(500);
  }
  }
