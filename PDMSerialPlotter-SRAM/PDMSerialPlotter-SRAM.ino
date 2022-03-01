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
short sampleBuffer[25600];
unsigned int readsize;
unsigned char a[2];

// number of samples read
unsigned int samplesRead;
void onPDMdata();
// SD
const int chipSelect = 4;
int SD_success; 
int file_size; // file size bytes
int file_maxsize=20000;
File dataFile;



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
  readsize=0;
}  
void loop() {
String dataString="";  
unsigned int i;

  // wait for samples to be read
  if (samplesRead) {
    // print samples to the serial monitor or plotter
   // for (int i = 0; i < samplesRead; i++) {

     //Serial.println(sampleBuffer[i]); // only this format can plot
    // Serial.write(sampleBuffer[i]);

  
    // clear the read count
    if(readsize < file_maxsize)  
     samplesRead = 0;
    else {
      samplesRead=0;
      PDM.end();
      Serial.write("write samples =");
      Serial.write(readsize);
      Serial.write("\n\r --end ---");
      Serial.println("start read out samples data");
      for(i=0; i< readsize; i++){
          a[0]=sampleBuffer[i]>>8;
          a[1]=sampleBuffer[i] & 0x00ff;
          Serial.write(a[1]);
          Serial.write(a[0]);
      }
      Serial.println(" --- read done !! ---");
    }
    
  }
}

void onPDMdata() {
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read into the sample buffer
  PDM.read(sampleBuffer+readsize, bytesAvailable);
  
  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2; 
  readsize=readsize+samplesRead;


}
