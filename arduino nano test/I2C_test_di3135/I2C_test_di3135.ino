//=======================================
// I2C test
// test di3135
// 2019/5/21 CNHsu

// A4 : SDA
// A5 :SCL
// di3135 connect
// VDD, VIO= 3.3V

//=======================================

#include <Wire.h>

#define Slave_ID 0x18 // SD0=GND, i2c ID=0x18
//#define Slave_IDR 0x31 // SD0=GND, i2c read ID=0x18
//#define Slave_IDW 0x30 // SD0=GND, i2c write ID=0x18
int LED=13;
int x,y,z;


void setup() {
  // put your setup code here, to run once:
  pinMode(12,INPUT);
  Wire.begin();
  Serial.begin(9600);
  Serial.write("I2C test address ?:\n\r");
  Serial.write("1: COM_TEST :\n\r");
  Serial.write("2: CON0, 3: CON1:\n\r");
  Serial.write("4: PXS_LT, 5: PXS_HT:\n\r");
  Serial.write("6: PXS_DATA, 7: INT_FLAG:\n\r");
}

void loop() {
  // put your main code here, to run repeatedly:
 char i;
 int k;
 char i1,i2;
 String instr ="";
 /*
 
  */
  while (!Serial.available()) {} // wait RX data input
  i=Serial.read();
  Serial.write(i);
  switch (i){
   case 'Q': break;
   case '1' : Serial.print("*COM TEST read only\n\r");              
              Serial.println(I2C_Read(0x00),HEX);              
              Serial.println(I2C_Read(0x01),HEX);              
              Serial.println(I2C_Read(0x02),HEX);
              Serial.println(I2C_Read(0x07),HEX); 
              // active g sensor here!
              I2C_Write(0x08,0x02); // 02 enter normal meas mode            
              Serial.println(I2C_Read(0x08),HEX);
              Serial.println("0x09 ctrl cfg1 enable");
              I2C_Write(0x09,0x0f); // enable lowpass filter, z or y or x
              Serial.println(I2C_Read(0x09),HEX);
              // fscale full scale range
              I2C_Write(0x0a,0x02); // 8g
              Serial.println(I2C_Read(0x0a),HEX);
              Serial.println("ctrl_cfg3");
              Serial.println(I2C_Read(0x0b),HEX);
              Serial.println("inact_time");              
              Serial.println(I2C_Read(0x0d),HEX);
              Serial.println("x-y-z data");              
              Serial.println(I2C_Read(0x10),HEX);
              Serial.println(I2C_Read(0x11),HEX);
              Serial.println("x data");
              Serial.println(ReadValue(I2C_Read(0x10),I2C_Read(0x11)));
              
              Serial.println(I2C_Read(0x12),HEX);
              Serial.println(I2C_Read(0x13),HEX);
              Serial.println(I2C_Read(0x14),HEX);              
              Serial.println(I2C_Read(0x15),HEX);
              Serial.println("fifo");              
              Serial.println(I2C_Read(0x16),HEX); 
              Serial.println("fifo_sts");
              Serial.println(I2C_Read(0x17),HEX);
              Serial.println("AOI_sts");              
              Serial.println(I2C_Read(0x18),HEX);
              Serial.println(I2C_Read(0x19),HEX);
              Serial.println("TAP_sts");
              Serial.println(I2C_Read(0x1a),HEX);
              Serial.println("IRQ");
              Serial.println(I2C_Read(0x20),HEX);
              Serial.println(I2C_Read(0x21),HEX); 
              Serial.println("CTRL_CFG1");
              I2C_Write(0x22,0x01); // trigger source tag 
              Serial.println(I2C_Read(0x22),HEX);   
              Serial.println("FIFO_THS");
              Serial.println(I2C_Read(0x23),HEX);
              Serial.println("Filter ");
               I2C_Write(0x24,0x23); // hipass & data format is unsigned
              Serial.println(I2C_Read(0x24),HEX);
              Serial.println("AOI");
              Serial.println(I2C_Read(0x30),HEX);
              Serial.println("AOI time and thr");
              Serial.println(I2C_Read(0x31),HEX); 
              Serial.println(I2C_Read(0x32),HEX);
              Serial.println(I2C_Read(0x33),HEX);
              Serial.println(I2C_Read(0x34),HEX); 
              Serial.println("AOI HYSX");
              Serial.println(I2C_Read(0x35),HEX);
              Serial.println("AOI HYSY");
              Serial.println(I2C_Read(0x36),HEX);
              Serial.println("TAP cfg");
              I2C_Write(0x37,0x2A); // tap set double tap
              Serial.println(I2C_Read(0x37),HEX); 
              Serial.println("TAP time thr");
              Serial.println(I2C_Read(0x38),HEX);
              //
              Serial.println("TAP pulse window");
              I2C_Write(0x39,0x0A); // tap set double tap pulse 100ms
              Serial.println(I2C_Read(0x39),HEX);
              I2C_Write(0x3A,0x0A); // tap quiet window 100ms
              Serial.println(I2C_Read(0x3a),HEX);
              I2C_Write(0x39,0x0A); // tap interval window 100ms   
              Serial.println(I2C_Read(0x3b),HEX);    
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
   case '5' : Serial.print("*read x y z\n\r");
              x=ReadValue(I2C_Read(0x10),I2C_Read(0x11));
              y=ReadValue(I2C_Read(0x12),I2C_Read(0x13));
              z=ReadValue(I2C_Read(0x14),I2C_Read(0x15));
              Serial.print("(x,y,z)=");
              Serial.print(x);
              Serial.print(",");
              Serial.print(y);
              Serial.print(",");
              Serial.println(z);
              Serial.print("FIFI unread :");
              Serial.println(I2C_Read(0x16),HEX);
              
             break;   
   case '6' : Serial.print("*x y z data\n\r");
              Serial.println("x-y-z data"); 
              for(k=0;k < 50;k++){
              Serial.print("value:");  
              Serial.print(ReadValue(I2C_Read(0x10),I2C_Read(0x11))-x);
              Serial.print(",");
              Serial.print(ReadValue(I2C_Read(0x12),I2C_Read(0x13))-y);
              Serial.print(",");
              Serial.print(ReadValue(I2C_Read(0x14),I2C_Read(0x15))-z);                      
               Serial.print("\n\r");
               delay(500);

              }
             break; 
   case '7' : Serial.print("*check tap\n\r");
              Serial.print("TAP_sts(0x1A) :");
              for(k=0;k <100;k++){
                // tap test              
               //  while ((I2C_Read(0x1a) & 0x80 ) == 0x00) ;
              Serial.println(I2C_Read(0x1a),HEX);
              delay(250);            
              }
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

int ReadValue(uint8_t c1, uint8_t c2)
{
 String str="";
 word vv;
  vv= word(c2,c1);
  vv= vv>>4;
  str=vv;
 // Serial.print(c1,HEX);
 // Serial.print(" , ");
 // Serial.println(c2,HEX);
 // Serial.println(vv,HEX);
 //Serial.print("value=");
 //Serial.println(str.toInt());
 return str.toInt();
}

  uint8_t I2C_Read(uint8_t reg)  
{  
      uint8_t chr = 0;  
      Wire.beginTransmission(Slave_ID);  //device address     
      Wire.write(reg);  // read register  
      Wire.endTransmission(false);   
      Wire.requestFrom(Slave_ID, 1, true);    //address, quantity, stop      
      if(Wire.available()){  
            chr = Wire.read();           //data                       
      }
       return  chr ;

}  
  
uint8_t I2C_Write(uint8_t reg, uint8_t data)  
{  
     Wire.beginTransmission(Slave_ID);           //device address    
     Wire.write(reg);                                        // REG  
     Wire.write(data);                                       // DATA  
     Wire.endTransmission(true); // generate stop condition  // STOP  
  
}  
