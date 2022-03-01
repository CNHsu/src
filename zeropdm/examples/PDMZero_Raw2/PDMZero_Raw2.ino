/*************************************************************
PDM microphone example : Adaptive Volume : Vu-Meter
By JV March 2019 Version 1.00

Imprived version vs Adafruit, better filter and faster loop
*************************************************************/
#include <PDMZero.h>
#define DBG 1  // Debug info

// Definitions for Audio PDM Microphone
#define SAMPLERATE_HZ 9375
#define DECIMATION    128
#define PCMTIME 10         // sample evaluation time in milliseconds
#define PDM_REPEAT_LOOP_16(X) X X X X X X X X X X X X X X X X // a manual loop-unroller!

// 128 taps deciation filter
uint16_t sincfilter[DECIMATION] {    0,1,   2,5,  9,15, 21,30,  39,51,  63,78,  94,113, 132,155,  179,207,  236,269,  302,340,  379,423,  467,516,  565,619,  674,733,  792,856,  920,987,  1055,1125,  1196,1268,  1341,1414,  1487,1560,  1633,1704,  1776,1844,  1913,1977,  2042,2100,  2159,2211,  2263,2307,  2352,2387,  2422,2448,  2474,2490,  2506,2511,  2516,2511,  2506,2490,  2474,2448,  2422,2387,  2352,2307,  2263,2211,  2159,2100,  2042,1977,  1913,1844,  1776,1704,  1633,1560,  1487,1414,  1341,1268,  1196,1125,  1055,987, 920,856,  792,733,  674,619,  565,516,  467,423,  379,340,  302,269,  236,207,  179,155,  132,113,  94,78,  63,51,  39,30,  21,15,  9,6,  4,3,  2,1,  1,0};

PDMZero Mypdm = PDMZero(2, A6);   // Create PDM object, with Clock and Data pins used clock=> PA10 = ~2  data => PA07 = A6  MKR1000 / MKR1010

uint32_t samples[2][(SAMPLERATE_HZ * PCMTIME) / 1000];          
uint16_t x;

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

Read_AudioSamples();                                           // read audio to sample buffer
for (x=0;x< (SAMPLERATE_HZ * PCMTIME) / 1000 ; ++x)            // print sample buffer
{
 if (samples[0][x]!=0) { 
  Serial.print(68000); Serial.print(",");                      // autorange max manual -> Plotter
  Serial.print(64000); Serial.print(",");                         
  Serial.print(2*samples[0][x]-samples[1][x]); Serial.print(",");                   
  Serial.print(2*samples[1][x]-samples[0][x]); Serial.println(",");  
 } // end inf not zero
}
}



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
        samplel >>= 1;                  // read left to right, LSB set
        sampler >>= 1;                  // read left to right, LSB set
      }
      )
      
    } // end decimation loop
    
    // copy value to sample buffer : 
   if ( fir_suml!=0 && fir_sumr!=0 ) {
          samples[0][t]=fir_suml;
          samples[1][t]=fir_sumr+500;            // physical diufference between left and right microphones's - measured !!

    } // end sample process loop (sample !=0)   
  } // end Sample For loop  

}
