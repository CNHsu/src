//=======================================
// I2C test
// test px318j
// 2019/5/21 CNHsu

// A4 : SDA
// A5 :SCL
//=======================================

#include <Wire.h>

#define I2C_ADDRESS 0x1c // i2c ID=0x1C
int LED=13;


void setup() {
  // put your setup code here, to run once:
  pinMode(12,INPUT);
  Wire.begin();
  Serial.begin(9600);
  Serial.write("I2C test address px318i 0x1C:\n\r");
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
              I2C_Write(0xf4,0xEE); // sw reset
              delay(30);

              Serial.print("0xf0 :");
               Serial.println(I2C_Read(0xf0),HEX);
              Serial.print("0xf1 :");
              Serial.println(I2C_Read(0xf1),HEX);
              Serial.print("0xf2 :");
              Serial.println(I2C_Read(0xf2),HEX);
              Serial.print("0xfe :");
              Serial.println(I2C_Read(0xfe),HEX);
              Serial.print("0xff :");
              Serial.println(I2C_Read(0xff),HEX);
              Serial.print("0x00,01: psData: ");
              Serial.print(I2C_Read(0x00),HEX);
              Serial.print(" , ");
              Serial.println(I2C_Read(0x01),HEX);
              Serial.println("--- end ---");
              I2C_Write(0x4f,0xff); // wait time
              Serial.print("0x4f :");
              Serial.println(I2C_Read(0x4f),HEX);
               I2C_Write(0x60,0x15); // set ps mean
              Serial.print("0x60 :");
              Serial.println(I2C_Read(0x60),HEX);
              I2C_Write(0x61,0x20); // set ps mean    
              Serial.print("0x61 :");
              Serial.println(I2C_Read(0x61),HEX);
              Serial.print("0x62 :");
              Serial.println(I2C_Read(0x62),HEX);
              Serial.print("0x63 :");
              Serial.println(I2C_Read(0x63),HEX);
              I2C_Write(0x64,0x0B); // set ps mean
              Serial.print("0x64 :");
              Serial.println(I2C_Read(0x64),HEX);
              Serial.print("0x67 :");
              Serial.println(I2C_Read(0x67),HEX);              
              Serial.print("0x69 :");
              Serial.println(I2C_Read(0x69),HEX);
              Serial.print("0x6a :");
              Serial.println(I2C_Read(0x6a),HEX);              
              Serial.print("0x6b :");
              Serial.println(I2C_Read(0x6b),HEX);
              I2C_Write(0x6c,0x00); // set th_low lowbyte
              I2C_Write(0x6d,0x01); // set th_low hibyte
              I2C_Write(0x6e,0x00); // set th_hi lowbyte
              I2C_Write(0x6f,0x03); // set th_hi hibyte
              Serial.print("ox6c psthlow :");
              Serial.print(I2C_Read(0x6c),HEX);
              Serial.print(" , ");
              Serial.println(I2C_Read(0x6d),HEX);
               I2C_Write(0x6e,0x00); // set psthhigh           
              Serial.print("0x6e psthhigh :"); 
              Serial.print(I2C_Read(0x6e),HEX);              
              Serial.print(" , ");
              Serial.println(I2C_Read(0x6f),HEX);    
              I2C_Write(0xf0,0x02); // enable ps
              delay(10);
               while (I2C_Read(0xf0) != 0x02)
            {  
              I2C_Write(0xf0,0x02);             
              Serial.println(I2C_Read(0xf0),HEX);
            }           
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
            while (I2C_Read(0xf0) != 0x00)
            {
                if ( (I2C_Read(0xf0) & 0x02) ){ // chekc psEn
                Serial.print("psData :");   
               Serial.print(I2C_Read(0x00),HEX);
               Serial.print(" , "); 
               Serial.println(I2C_Read(0x01),HEX);
               Serial.print("0x60 :"); 
               Serial.println(I2C_Read(0x60),HEX);
               Serial.print("0xff :"); 
               Serial.println(I2C_Read(0xff),HEX);
               delay(500);               
                }
               Serial.print("0xf0 :");
               Serial.println(I2C_Read(0xf0),HEX);
               Serial.print("0xfe :"); 
                Serial.println(I2C_Read(0xfe),HEX);
                I2C_Write(0xf0,0x02);
               delay(1000);
               if (digitalRead(12) == LOW) Serial.println("***** INT low");
            }                
             break; 
   case '7' : Serial.print("*PXS_DATA\n\r");
             Serial.println(I2C_Read(0x00),HEX);
             Serial.println(I2C_Read(0x01),HEX);
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
