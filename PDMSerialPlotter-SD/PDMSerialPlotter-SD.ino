/*
  This example reads audio data from the on-board PDM microphones, and prints
  out the samples to the Serial console. The Serial Plotter built into the
  Arduino IDE can be used to plot the audio data (Tools -> Serial Plotter)

  Circuit:
  - Arduino Nano 33 BLE board

  This example code is in the public domain.
*/

#include <PDM.h>
#include <SPI.h>
#include <SD.h>
// buffer to read samples into, each sample is 16-bits
short sampleBuffer[256];

// number of samples read
volatile int samplesRead;
void onPDMdata();
// SD
const int chipSelect = 4;
int SD_success; 
int file_size; // file size bytes
int file_maxsize=20000;
File dataFile;
unsigned char a[2];


void setup() {
  Serial.begin(9600);
  while (!Serial);

  // configure the data receive callback
  PDM.onReceive(onPDMdata);

  // optionally set the gain, defaults to 20
  // PDM.setGain(30);

  // initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate
  if (!PDM.begin(1, 16000)) { // 16000 & 41667 in PDM.cpp only support this two
    Serial.println("Failed to start PDM!");
    while (1);
  }
  SD_success=0;
}  
void loop() {
String dataString="";  
// initial SD

  while(!SD_success){
   Serial.print("\nInitializing SD card...........");
   if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    //while (1);
  }
  else{
  Serial.println("card initialized.");
  if (SD.exists("try.txt")) {
    Serial.println("try.txt exist!!!");
    SD.remove("try.txt");
  }
  else Serial.println("**try.txt NOT exist!!!");
  delay(1000);
  SD_success=1;
  file_size=0;
  dataFile = SD.open("try.txt", FILE_WRITE);
  if(dataFile)
  Serial.println("SD file try.txt open");
  else {Serial.println("SD file open error xxxxx"); SD_success=0;}
  }
  }
  





  
  // wait for samples to be read
  if (samplesRead && dataFile) {
    // print samples to the serial monitor or plotter
    for (int i = 0; i < samplesRead; i++) {
      // filter small signal
      //if (abs(sampleBuffer[i]) < 200) sampleBuffer[i]=0;
     //Serial.println(sampleBuffer[i]); // only this format can plot
    // Serial.write(sampleBuffer[i]);
    //write to file
    // big endian
     a[0]=sampleBuffer[i]>>8;
     a[1]=sampleBuffer[i] & 0x00ff;
     if(file_size < file_maxsize){
       dataFile.write(a,2);
     } 
     else {
      dataFile.close();Serial.println("dataFile done!!"); 
      // re-open the file for reading:
      dataFile = SD.open("try.txt");
      if (dataFile) {
        Serial.println("try.txt:");

    // read from the file until there's nothing else in it:
    while (dataFile.available()) {
      Serial.write(dataFile.read());
     }
    // close the file:
    dataFile.close();
    } else {
    // if the file didn't open, print an error:
    Serial.println("error opening try.txt");
    }

    while(1); // stop the loop!!!!
   }  
  }
    file_size=file_size+samplesRead;
    // clear the read count  
    samplesRead = 0;
  }
}

void onPDMdata() {
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);
  
  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2; 


}
