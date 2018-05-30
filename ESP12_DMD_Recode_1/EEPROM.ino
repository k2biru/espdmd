/*
 * EEPROM MAP 512 byte
 * 
 * ADS    Value
 * ===========================================
 * 0      [Indikator EEPROM kosong/Tidak] ~
 * 3      [Indikator EEPROM kosong/Tidak]
 * 
 * 6      [link/text [3x101]] ~
 * 309    [link/text [3x101]]
 * 
 * 311      [setting]
 * 312      [data]
 * 
 * 
 */
 
 ///////////////////////////////////////////////////////////
 


void bacaEEPROMSetting()
{
  EEPROM.begin(512);
  Serial.println("Membaca EEPROM Setting");
  EEPROM_readAnything(311,setting);
  EEPROM_readAnything(312,data);
  EEPROM.end();
   
  Serial.println("data");
  Serial.println(data.kecerahan);
  Serial.println(data.APSSID);
  Serial.println(data.APPass);
  Serial.println(data.ssid);
  Serial.println(data.pass);
  Serial.println(data.APCh);
  Serial.println("Setting");
  Serial.println(setting.all);
}

void isiEEPROMSetting()
{
  data.kecerahan = 20;
  strncpy(data.APSSID, "Yozora V3.1", 20);
  strncpy(data.APPass, "yzr", 20);
  strncpy(data.ssid, "HME Lt.2", 20);
  strncpy(data.pass, "", 20);
  data.APCh = 14;
  setting.all='F';
  
  EEPROM.begin(512);
  Serial.println("tulis EEPROM Setting");

  EEPROM_writeAnything(311,setting);
  EEPROM_writeAnything(312,data);
  

  EEPROM.end();
  
}

void isiEEPROMLink()
{
  Link link;
  strncpy(link.data[0], "http://ristie.ub.ac.id/io/date/today.php", 100);
  strncpy(link.data[1], "http://ristie.ub.ac.id/io/weather/open_weather.php", 100);
  strncpy(link.data[2], "Tulisan ini", 100);
  link.all = 'G';
  
  EEPROM.begin(512);
  Serial.println("tulis EEPROM Link");

  EEPROM_writeAnything(6,link);

  EEPROM.end();
}
void bacaEEPROMLinkText()
{
  Link link;
  EEPROM.begin(512);
  EEPROM_readAnything(6,link);
  EEPROM.end();
  Serial.println("Link ");
  Serial.println(link.data[0]);
  Serial.println(link.data[1]);
  Serial.println(link.data[2]);
  Serial.println(link.all);
}







  
