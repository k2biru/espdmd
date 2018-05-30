/*
  Countdown on a single DMD display
 */

#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial14.h>

const int COUNTDOWN_FROM = 12;
int counter = COUNTDOWN_FROM;
#define pin_A 16
#define pin_B 12
#define pin_sclk 0
#define pin_clk 14
#define pin_r 13
#define pin_noe 15

#define DISPLAYS_WIDE 2
#define DISPLAYS_HIGH 1
SPIDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, pin_noe, pin_A, pin_B, pin_sclk);  // DMD controls the entire display
DMD_TextBox box(dmd, 0, 2);  // "box" provides a text box to automatically write to/scroll the display

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  dmd.setBrightness(255);
  dmd.selectFont(Arial14);
  dmd.begin();
}

// the loop routine runs over and over again forever:
void loop() {
  //box.printf("Dc");
  drawJam();
  delay(1000);
}

void drawJam()
{
  dmd.drawBox(0,11,4,15);
}

