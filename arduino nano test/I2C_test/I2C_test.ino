//=======================================
// I2C test
// 2019/5/21 CNHsu
//=======================================

#include <Wire.h>

#define I2C_ADDRESS 0x1E


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Serial.println("!2C Test");
 Wire.begin();
I2C_Write(0x10,0xF1);
 Serial.println(I2C_Read(0x00),BIN);
}

void loop() {
  // put your main code here, to run repeatedly:

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
