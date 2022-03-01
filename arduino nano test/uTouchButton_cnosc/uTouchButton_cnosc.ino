#include <SPI.h>
#include <ILI9341_due_config.h>
#include <ILI9341_due.h>
#include "fonts/Arial_bold_14.h"

#include <URTouch.h>

// For the Adafruit shield, these are the default.
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
// by CNHsu define touch
#define t_SCK 3              
#define t_CS 4                
#define t_MOSI 5              
#define t_MISO 6             
#define t_IRQ 7 
URTouch myTouch(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);

ILI9341_due tft = ILI9341_due(TFT_CS, TFT_DC, TFT_RST);

//URTouch  myTouch(30, 28, 26, 24, 22);

int x, y;
char strBuff[10];
char stCurrent[20] = "";
int stCurrentLen = 0;
char stLast[20] = "";

int Buffer;
word colorlist[] = { ILI9341_WHITE, ILI9341_BLACK, ILI9341_RED, ILI9341_BLUE, ILI9341_LIME, ILI9341_FUCHSIA, ILI9341_YELLOW, ILI9341_AQUA };


/*************************
**  Required functions  **
*************************/
int stopflag;
void setup()
{
  stopflag=0;
	Serial.begin(9600);
	// Initial setup
	tft.begin();
	tft.setRotation(iliRotation270);	// landscape
	tft.fillScreen(ILI9341_BLACK);

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_HI);

	tft.setFont(Arial_bold_14);
	//drawButtons();
  //tft.setRotation((iliRotation)1); //set screen direction

tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  //tft.drawLine(32, 0, 32, tft.height() - 1, ILI9341_WHITE);
  tft.drawLine(tft.width() - 32, 0, tft.width() - 32, tft.height() - 1, ILI9341_WHITE);
  tft.printAt("S", tft.width() - 20, 8);
  tft.printAt("T", tft.width() - 20, 24);
  tft.printAt("O", tft.width() - 20, 40);
  tft.printAt("P", tft.width() - 20, 56);
  tft.printAt(" ", tft.width() - 20, 72);
  tft.printAt("+", tft.width() - 18, 138);
  tft.printAt("-", tft.width() - 18, 202);
  tft.fillRect(tft.width() - 32, 96, 32, 32, ILI9341_WHITE);
  tft.drawLine(tft.width() - 32, 160, tft.width() - 1, 160, ILI9341_WHITE);
  tft.drawLine(tft.width() - 32, 192, tft.width() - 1, 192, ILI9341_WHITE);
  tft.drawLine(tft.width() - 32, 224, tft.width() - 1, 224, ILI9341_WHITE);

  
}

void loop()
{
 int px,py,bsize;
 int cx,cy;
 int i;
 float f;
 String st;
 cx=tft.width();
 cy=tft.height();
 px=0; // time0
 py=120; // 240/2
 bsize=4;
 for(x = 0;x < (cx-32);x++){
       //Buffer = 63-(analogRead(A0)>>4);
       Buffer=analogRead(A0)>>2;
      // Serial.println(Buffer);
      f=Buffer*0.0196;
      st=String(f);
      tft.printAt(st, tft.width() - 100, 10);
      Buffer=abs(Buffer-240);
      tft.drawLine(px,py,x,Buffer,ILI9341_RED) ;     
      //tft.fillCircle(x,Buffer, bsize,ILI9341_RED );
      px=x;
      py=Buffer;
}
//tft.fillScreen(ILI9341_BLACK);
if(stopflag==0)
  tft.fillRect( 0, 0, (cx-34), cy, ILI9341_BLACK);
tft.drawRect( 0,0, (cx-34),cy,ILI9341_BLUE);
tft.drawLine( 0, cy/2, (cx-34), cy/2, ILI9341_BLUE);
tft.drawLine( (cx-34)/2, 0, (cx-34)/2,cy,ILI9341_BLUE);
// x-coord
for(x=0; x< (cx-32); x=x+4){
  tft.drawLine(x, (cy/2)-2,x, (cy/2)+2, ILI9341_WHITE);
}
for(x=0; x< (cx-32); x=x+20){
  tft.drawLine(x, (cy/2)-4,x, (cy/2)+4, ILI9341_WHITE);
}
// y-coord
for(y=0; y< cy; y=y+4){
  tft.drawLine( (cx-34)/2-2, y,(cx-34)/2+2, y, ILI9341_WHITE);
}
for(x=0; x< (cx-32); x=x+20){
  tft.drawLine((cx-34)/2-4, y,(cx-34)/2+4, y, ILI9341_WHITE);
}

while(myTouch.dataAvailable()==true)
{

      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY();
          if ((y>192) && (y<224) && (x>tft.width() - 100))
          {
            if (bsize>1)
            {
              while (myTouch.dataAvailable()) {};
              delay(50);
              Serial.println("hit-");
            }
          }
          if ((y>128) && (y<160)&& (x>tft.width() - 100))
          {
            if (bsize<10)
            {
              while (myTouch.dataAvailable()) {};
              delay(50);
               Serial.println("hit+");
            }
          }          
           if ((y<73) && (x>tft.width() - 100))
          {
            if (bsize<10)
            {
              while (myTouch.dataAvailable()) {};
              delay(50);
                if(stopflag==0){
                tft.printAt("S", tft.width() - 20, 8);
                tft.printAt("T", tft.width() - 20, 24);
                tft.printAt("A", tft.width() - 20, 40);
                tft.printAt("R", tft.width() - 20, 56);
                tft.printAt(" ", tft.width() - 20, 72);
                stopflag=1;
                Serial.println("hit star");
                }                
                else {
                stopflag=0;
                tft.printAt("S", tft.width() - 20, 8);
                tft.printAt("T", tft.width() - 20, 24);
                tft.printAt("O", tft.width() - 20, 40);
                tft.printAt("P", tft.width() - 20, 56);
                tft.printAt(" ", tft.width() - 20, 72); 
                Serial.println("hit stop");               
                }               
            }
          }  
    
}

}

