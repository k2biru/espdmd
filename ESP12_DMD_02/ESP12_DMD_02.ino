/*
  Quick demo of major drawing operations on a single DMD
 */


#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
//#include "EEPROMAnything.h"
//#include <stdio.h>

#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

#include <Wire.h>
#include "Sodaq_DS3231.h"


#include <SPI.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>

#include "fonts/Arial_Black_16.h"
#include "fonts/SystemFont5x7.h"
#include "fonts/angka6x13.h"
#include "fonts/angka_2.h"
#include "fonts/Font3x5.h"

#define pin_A 16
#define pin_B 12
#define pin_sclk 0
#define pin_clk 14
#define pin_r 13
#define pin_noe 15
#define DISPLAYS_WIDE 2
#define DISPLAYS_HIGH 1

#define MAX_BUFFER 100

struct Setting
{
  union {
    char all;
    struct {
      boolean onlineTime: 1;
      boolean debugUpdate: 1;
      boolean wifiAutoConnect: 1;
      boolean noDataMsg: 1;
      boolean weatherEnable: 1;
      boolean f: 1;
      boolean r: 1;
      boolean weatherReady: 1;
    };
  } ;
};

SPIDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, pin_noe, pin_A, pin_B, pin_sclk); // DMD controls the entire display
byte second,minute,hour,dayOfWeek,date,month,year;
float temperature;
uint32_t old_ts;
byte i,j,durasi=0;
byte layar=0;
unsigned int kecerahan;

char APSSID [20];
char APPass[20];
char ssid[20] ;
char pass[20] ;
byte APCh;

Setting setting;

//const char* ssid = "Yozora v3";
//const char* pass = "1234efgh";

//DateTime dt(2018, 05, 01, 22, 13, 0, 1);

ESP8266WebServer server ( 80 );

// the setup routine runs once when you press reset:
void setup() {


  
  setting.all = readSettings();
  
  AP();// make AP
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  //rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 
  dmd.setBrightness(25);
  dmd.selectFont(SystemFont5x7);
  dmd.begin();


  drawTextOnly("Yozora v3",0,0,3000,Font3x5);
  drawMarquee();
  //drawTextOnly((String)myIP,0,0,3000,Font3x5);
  

  // Circle with a line at a tangent to it

  // Outline box containing a filled box
  durasi=random(58);
  server.on ("/", handleHome);
  server.on ("/update", handleSentData);
  server.onNotFound ( handleNotFound );                   // bila tiddak di index server
  server.begin();

}

// the loop routine runs over and over again forever:
void loop() {
  i++;
  server.handleClient();

  getRTC();
  delay(900);

  switch(layar)
  {
    case 0:
      drawClockSqr(0,0);
      drawTemp(18,4);
      break;
    case 1:
      drawTextClock(0,0);
      break;
    case 2:
      drawTextClockSmall(0,0);
      drawDate(0,0);
      break;
    case 3:
      layar =0;
      break;
  }
  //
  if(durasi-second<=0)
  {
    layar++;
    durasi=random(56,58);
  }
  if(layar==0 && i>=10) 
  {
    drawMarquee();
    i=0;
  }
}
