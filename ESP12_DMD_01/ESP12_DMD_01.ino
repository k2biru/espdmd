/*
  Quick demo of major drawing operations on a single DMD
 */

#include <Wire.h>
#include "Sodaq_DS3231.h"


#include <SPI.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>

#define pin_A 16
#define pin_B 12
#define pin_sclk 0
#define pin_clk 14
#define pin_r 13
#define pin_noe 15
#define DISPLAYS_WIDE 2
#define DISPLAYS_HIGH 1


SPIDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, pin_noe, pin_A, pin_B, pin_sclk); // DMD controls the entire display
byte second,minute,hour;
float temperature;
uint32_t old_ts;
byte i=0;

//DateTime dt(2018, 04, 29, 00, 32, 0, 6);

// the setup routine runs once when you press reset:
void setup() {

    
  Serial.begin(9600);
    Wire.begin();
    rtc.begin();
  //rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 
  dmd.setBrightness(25);
  dmd.selectFont(SystemFont5x7);
  dmd.begin();
  
  second=4;
  minute=35;
  hour=4;
  drawMarquee();

  // Circle with a line at a tangent to it

  // Outline box containing a filled box


}

// the loop routine runs over and over again forever:
void loop() {
  i++;

  getRTC();
  delay(900);
  drawClockSqr(0,0);
  drawTemp(18,5);
  if (i>=10) 
  {
    drawMarquee();
    i=0;
  }
}
