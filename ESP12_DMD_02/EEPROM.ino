/*
 * EEPROM MAP
 * ADS    Value
 * ===========================================
 * 0      [Indikator EEPROM kosong/Tidak] ~
 * 3      [Indikator EEPROM kosong/Tidak]
 * 
 * 4      [Kecerahan [unsigned Int]]~
 * 5      [Kecerahan [unsigned Int]]
 * 
 * 11     [APSSID char]~
 * 30     [APSSID char]
 * 31     [APSSID pass char]~
 * 50     [APSSID pass char]
 * 
 * 51     [Port char]       [NOT USE]
 * 52     [wifi Channel]      1~14;
 * 53     [8bit of settings]    [Not Used yet]
 * 
 * 56     [SSID char]~
 * 76     [SSID char]~
 * 77     [SSID pass char]~
 * 97     [SSID pass char]~     
 * 
 * 100    [Bbit Seting HTTP/Text]
 * 
 * 101    [HTTP/TextLink char 1]~
 * 200    [HTTP/TextLink char 1]
 * 201    [HTTP/TextLink char 2]~
 * 300    [HTTP/TextLink char 2]
 * 301    [HTTP/TextLink char 3]~
 * 400    [HTTP/TextLink char 3]
 * 
 * 401    [HTTP Weather ]~
 * 500    [HTTP Weather ]
 * 
 * 
 */
 
 ///////////////////////////////////////////////////////////
 void defValue()
 {
  strncpy(APSSID, "Yozora V3.1", 20);
  strncpy(APPass, "yzr", 20);

  strncpy(ssid, "HME Lt.2", 20);
  strncpy(pass, "", 20);

  APCh = 14;
  setting = 0x00;

  kecerahan = 20;

  strncpy(APSSID, "hello", 20);
  strncpy(APSSID, "hello", 20);
  strncpy(APSSID, "hello", 20);
  
 }
 
 void writeEEPROMChar(char buff [MAX_BUFFER], unsigned int from , unsigned int length)
{
  EEPROM.begin(512);
  unsigned int i;
  for ( i = 0; i < length ; i++)
  {
    EEPROM.write(i + from , buff[i]);
  }
  EEPROM.end();
}

void readEEPROMChar( char *buff, unsigned int from , unsigned int length)
{
  EEPROM.begin(512);
  unsigned int i;
  for ( i = 0; i < length ; i++)
  {
    buff[i] = EEPROM.read(i + from);
  }
  EEPROM.end();
}


byte readSettings()
{
  byte setting;
  /// read AP
  readEEPROMChar(APSSID, 11, 20);
  readEEPROMChar(APPass, 31, 20);
  //readEEPROMChar(, 31, 20);
  EEPROM.begin(512);
  APCh = EEPROM.read(52);
  setting = EEPROM.read(53);
  //if (APCh<1||APCh>14)APCh=6;
  EEPROM.end();
  /// read WiFi
  readEEPROMChar(ssid, 56, 20);
  readEEPROMChar(pass, 77, 20);
  return setting;
}

void writeSettings( byte byteSet)
{
  char buff[20];
  byte temp;
  readEEPROMChar(buff, 11, 20);
  if (buff != APSSID)writeEEPROMChar(APSSID, 11, 20);
  readEEPROMChar(buff, 31, 20);
  if (buff != APPass)writeEEPROMChar(APPass, 31, 20);
  readEEPROMChar(buff, 56, 20);
  if (buff != ssid)writeEEPROMChar(ssid, 56, 20);
  readEEPROMChar(buff, 77, 20);
  if (buff != pass)writeEEPROMChar(pass, 77, 20);
  EEPROM.begin(512);
  temp = EEPROM.read(52);
  if (temp != APCh)EEPROM.write(52, APCh);
  temp = EEPROM.read(53);
  if (temp != byteSet)EEPROM.write(53, byteSet);
  EEPROM.end();
}
  
