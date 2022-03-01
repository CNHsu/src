#include <Arduino.h>
#include <Adafruit_ZeroI2S.h>
#include <PDMZero.h>

/*************************************************************
PDM microphone example : Adaptive Volume : Vu-Meter
By JV March 2019 Version 1.00


*************************************************************/

// Definitions for Audio PDM Microphone
#define SAMPLERATE_HZ 18750
#define DECIMATION    64
#define PCMTIME 25         // sample evaluation time in milliseconds
#define PDM_REPEAT_LOOP_16(X) X X X X X X X X X X X X X X X X // a manual looper

Adafruit_ZeroI2S I2S;

uint16_t sincfilter[DECIMATION] = {1, 2, 9, 21, 39, 63, 94, 132, 179, 236, 302, 379, 467, 565, 674, 792, 920, 1055, 1196, 1341, 1487, 1633, 1776, 1913, 2042, 2159, 2263, 2352, 2422, 2474, 2504, 2516, 2504, 2474, 2422, 2352, 2263, 2159, 2042, 1913, 1776, 1633, 1487, 1341, 1196, 1055, 920, 792, 674, 565, 467, 379, 302, 236, 179, 132, 94, 63, 39, 21, 9, 2, 1};
PDMZero Mypdm = PDMZero(2, A6);   // Create PDM object, with Clock and Data pins used clock=> PA10 = ~2  data => PA07 = A6  MKR1000 / MKR1010

uint32_t samples[2][(SAMPLERATE_HZ * PCMTIME) / 1000];          
uint16_t x;

// Read Stereo audio levels from I2S
void Read_AudioSamples() {
uint16_t  t;
uint32_t fir_suml, fir_sumr;

  for (t = 0; t < (SAMPLERATE_HZ * PCMTIME) / 1000; ++t)
  {
    fir_suml=0; fir_sumr=0; 
    uint16_t *sinc_ptr = sincfilter; // pointer to 64bit FIR-filer cooeficients

    for (uint8_t samplenum = 0; samplenum < (DECIMATION / 16) ; samplenum++) {
      uint32_t sampler = Mypdm.read();    // we read 32 bits at a time, high and low 16bit stereo
      uint32_t samplel = 0x0000FFFF & (sampler >> 16);
      sampler = sampler & 0x0000FFFF;

      PDM_REPEAT_LOOP_16(      // manually unroll loop: for (int8_t b=0; b<16; b++)
      {
          fir_suml += (*sinc_ptr) * (samplel & 0x1);     // do the convolution Left channel
          fir_sumr += (*sinc_ptr) * (sampler & 0x1);     // do the convolution Right channel 
                  
        sinc_ptr++;
        samplel >>= 1;                  // (Note we had to set I2S_SERCTRL_BITREV to get this to work, but saves us time!)
        sampler >>= 1;                  // (Note we had to set I2S_SERCTRL_BITREV to get this to work, but saves us time!)
      }
      )
      
    } // end decimation loop
    
    // copy value to sample buffer : 
   if ( fir_suml!=0 && fir_sumr!=0 ) {
          samples[0][t]=fir_sumr;
          samples[1][t]=fir_suml;

    } // end sample process loop (sample !=0)   
  } // end Sample For loop  

}

void setup()
{ 

/*********** Serial SETUP  **********/
int t=10;  //Initialize serial and wait for port to open, max 10 second waiting
  Serial.begin(921600UL);
  while (!Serial) {
    ; delay(1000);
    if ( (t--)== 0 ) break;
  }
/*********** PDM over I2S Setup ************/
if (!Mypdm.begin()) {
    Serial.println("Failed to initialize I2S/PDM!");
    while (1);
   }
if (!Mypdm.configure(SAMPLERATE_HZ * DECIMATION/16, true)) {
    Serial.println("Failed to configure PDM");
    while (1);
  }
Serial.println("PDM Test starts");             
}



void loop()
{
Read_AudioSamples();                                            // read audio to sample buffer

for (x=0;x< (SAMPLERATE_HZ * PCMTIME) / 1000 ; ++x)            // print sample buffer
{
  Serial.print(samples[0][x]); Serial.print(",");                   
  Serial.print(samples[1][x]); Serial.println(",");  
}

delay(2000);
}




