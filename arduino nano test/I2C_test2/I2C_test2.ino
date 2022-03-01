//=======================================
// I2C test
// A4 SDA, A5 SCL
// 2019/5/21 by CNHsu
//=======================================
#include <Wire.h>
#define I2C_ADDRESS 0x1E
int LED=13; // D13, LED or relay 2019/3/12
//int val=0;


void setup() {
  // put your setup code here, to run once:
  pinMode(12,INPUT);
  Wire.begin();
  Serial.begin(9600);
  Serial.write("I2C test address 0x1E:\n\r");
  Serial.write("1: COM_TEST :\n\r");
  Serial.write("2: CON0, 3: CON1:\n\r");
  Serial.write("4: PXS_LT, 5: PXS_HT:\n\r");
  Serial.write("6: PXS_DATA, 7: INT_FLAG:\n\r");
}

void loop() {
  // put your main code here, to run repeatedly:
 char i;
 /*
 
  */
  while (!Serial.available()) {} // wait RX data input
  i=Serial.read();
  Serial.write(i);
  switch (i){
   case 'Q': break;
   case '1' : Serial.print("*COM TEST read only\n\r");
             Serial.println(I2C_Read(0x00),HEX);
             led_blink(LED);
             break;
   case '2' : Serial.print("*CON0, set pxs\n\r");
             Serial.println(I2C_Read(0x01),BIN);
             I2C_Write(0x01,0x40); // proximity sensor enable
             Serial.println(I2C_Read(0x01),HEX);
             break;
   case '3' : Serial.print("*CON1, set int \n\r");
             Serial.println(I2C_Read(0x02),BIN); 
            I2C_Write(0x02,0x81); // pxs interrupt is event type,pxs_prst
             Serial.println(I2C_Read(0x02),HEX);
             break; 
   case '4' : Serial.print("*PXS_LT\n\r");
             Serial.println(I2C_Read(0x03),BIN);
             I2C_Write(0x03,0x30);
             Serial.println(I2C_Read(0x03),HEX);
             break; 
   case '5' : Serial.print("*PXS_HT\n\r");
             Serial.println(I2C_Read(0x04),BIN);
            I2C_Write(0x04,0xA0);
             Serial.println(I2C_Read(0x04),HEX);
             break;   
   case '6' : Serial.print("*PXS_DATA\n\r");
            while (I2C_Read(0x05) != 0xFF)
            {
               Serial.println(I2C_Read(0x05),HEX);
               if (digitalRead(12) == LOW) Serial.println("***** INT low");
            }                
             break; 
   case '7' : Serial.print("*PXS_DATA\n\r");
             Serial.println(I2C_Read(0x08),HEX);
             break;  
   case '8' : Serial.print("*command code 0x21\n\r");
              I2C_Write(0x09,0x21);
              led_blink(LED);
             break;                         
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
  delay(100);
  digitalWrite(i, LOW);
  delay(100);
  }
  }

  uint8_t I2C_Read(uint8_t reg)  
{  
      uint8_t chr = 0;  
      Wire.beginTransmission(I2C_ADDRESS);  //device address     
      Wire.write(reg);  // read register  
      Wire.endTransmission(false);   
      Wire.requestFrom(I2C_ADDRESS, 1, true);    //address, quantity, stop      
      if(Wire.available()){  
            chr = Wire.read();           //data                       
      }
       return  chr ;

}  
  
uint8_t I2C_Write(uint8_t reg, uint8_t data)  
{  
     Wire.beginTransmission(I2C_ADDRESS);           //device address    
     Wire.write(reg);                                        // REG  
     Wire.write(data);                                       // DATA  
     Wire.endTransmission(true); // generate stop condition  // STOP  
  
}  
