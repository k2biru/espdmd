/*
  Quick demo of major drawing operations on a single DMD
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <ESP8266HTTPClient.h>
#include <EEPROM.h>



#include <Wire.h>
#include "Sodaq_DS3231.h"
#include "writeAnything.h"


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
      boolean a: 1;
      boolean b: 1;
      boolean c: 1;
      boolean d: 1;
      boolean e: 1;
      boolean f: 1;
      boolean g: 1;
      boolean h: 1;
    };
  } ;
} setting;

struct Data
{
  unsigned int kecerahan;
  char APSSID [20];
  char APPass[20];
  char ssid[20] ;
  char pass[20] ;
  byte APCh;
} data;


struct Link
{
  char data[3][100];
  union {
    char all;
    struct {
      boolean text1: 1;
      boolean text2: 1;
      boolean text3: 1;
      boolean d: 1;
      boolean e: 1;
      boolean f: 1;
      boolean g: 1;
      boolean h: 1;
    };
  };
};

SPIDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, pin_noe, pin_A, pin_B, pin_sclk); // DMD controls the entire display
byte second, minute, hour, dayOfWeek, date, month, year;
float temperature;
uint32_t old_ts;
byte i, j, durasi = 0;
byte layar = 0;


const char* APSSID_ = "YoZor@";
const char* APPASS_ = "yoz";

//DateTime dt(2018, 05, 01, 22, 13, 0, 1);

ESP8266WebServer server ( 80 );

// the setup routine runs once when you press reset:
void setup() {
  WiFi.mode(WIFI_AP);
  Serial.begin(9600);

  Wire.begin();
  rtc.begin();
  delay(100);
  // isiEEPROMLink();
  //isiEEPROMSetting();
  bacaEEPROMLinkText();
  bacaEEPROMSetting();


  //AP();// make AP
  delay(1000);
  //IPAddress local_IP(192, 168, 7, 77);
  //IPAddress gateway(192, 168, 7, 250);
  //IPAddress subnet(255, 255, 255, 0);

  // WiFi.mode(WIFI_AP_STA);
  Serial.println("make AP");
  //Serial.println(data.APSSID);
  //Serial.println(data.APPass);
  WiFi.hostname("Yozora v3");
  //WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("ts", "0");



  //rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above
  dmd.setBrightness(10);
  dmd.begin();
  dmd.selectFont(SystemFont5x7);


  drawTextOnly("Yozora v3", 0, 0, 3000, Arial_Black_16);
  dmd.clearScreen();
  //drawMarquee(0, 4);
  //drawTextOnly((String)myIP,0,0,3000,Font3x5);


  // Circle with a line at a tangent to it

  // Outline box containing a filled box
  durasi = random(58);
  server.on ("/", handleHome);
  server.on ("/update", handleSentData);
  server.onNotFound ( handleNotFound );                   // bila tiddak di index server
  server.begin();

}

// the loop routine runs over and over again forever:
void loop() {
  //server.handleClient();
  dmd.setBrightness(10);
  getRTC();
  delay(900);
  if (layar==0)
  {
    drawClockSqr(0, 0);
    
  } else if(layar ==1)
  {
    drawTextClock(0, 0);
  } else if(layar ==2)
  {
    drawDate(0, 0);
  } else if (layar ==3)
  {
    drawTextClockSmall(0, 0);
  }else
  {
    layar=0;
  }
  //
  if (durasi - second <= 0)
  {
    layar++;
    durasi = random(57, 58);
  }
}
