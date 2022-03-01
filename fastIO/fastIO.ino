inline void digitalWrite_fast(int pin, bool val)
{
   if (val)  
      PORT->Group[g_APinDescription[pin].ulPort].OUTSET.reg = (1ul << g_APinDescription[pin].ulPin);
   else     
      PORT->Group[g_APinDescription[pin].ulPort].OUTCLR.reg = (1ul << g_APinDescription[pin].ulPin);
}


inline int digitalRead_fast(int pin)
{
   return !!(PORT->Group[g_APinDescription[pin].ulPort].IN.reg & (1ul << g_APinDescription[pin].ulPin));
}


void setup() {                
   pinMode(13, OUTPUT);
   pinMode(12, INPUT);     

}


void loop() {
  // int val = digitalRead_fast(12);

   digitalWrite_fast(13, HIGH ); 
   delay(1);           
   digitalWrite_fast(13, LOW );   
   delay(1); 

}
