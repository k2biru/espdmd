
void drawClockSqr(byte x_pos, byte y_pos)
{
  dmd.clearScreen();
  
  const byte x_secMin [] = {8, 9, 10, 11, 12, 13, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 13, 12, 11, 10, 9, 8,   7, 6, 5, 4, 3,      2, 1, 0, 0, 0,      0, 0, 0, 0, 0,    0, 0, 0, 0, 0,  0, 0, 0, 1, 2,  3, 4, 5, 6, 7};
  const byte y_secMin [] = {0, 0, 0, 0, 0,    0, 0, 0, 1, 2,      3, 4, 5, 6, 7,      8, 9, 10, 11, 12,   13, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3,  2, 1, 0, 0, 0,  0, 0, 0, 0, 0};
  const byte x_hour [] = {0, 11, 11, 11, 11, 9, 7, 4, 4, 4, 4, 6, 7};
  const byte y_hour [] = {0, 4, 6, 7, 11, 11, 11, 11, 9, 7, 4, 4, 4};
  byte x_hour_c, y_hour_c;
  byte x_min_c, y_min_c;
  byte temp;


  if (second == 0) dmd.drawFilledBox(x_pos ,y_pos ,x_pos +15,y_pos +15, GRAPHICS_OFF); // kosongkan jarum menit dan jam
  temp = hour;
  if (temp == 0)temp = 12;
  if (temp > 12) temp -= 12; // only 12 hours on analogue clocks
 

  //==== menentukan titik tengah (pusat) jarum
  if (temp <= 5) x_hour_c = x_pos + 8;
  else x_hour_c = x_pos + 7;

  if (temp <= 3) y_hour_c = y_pos + 7;
  else if (temp <= 8) y_hour_c = y_pos + 8;
  else if (temp <= 12) y_hour_c = y_pos + 7;

  dmd.drawLine(x_hour_c,y_hour_c, x_pos +  x_hour[temp], y_pos + y_hour[temp]);
  
  // draw minute
  if (minute <= 29)x_min_c = x_pos + 8;
  else x_min_c = x_pos + 7;

  if (minute <= 14) y_min_c = y_pos + 7;
  else if (minute <= 44) y_min_c = y_pos + 8;
  else if (minute <= 59) y_min_c = y_pos + 7;

  dmd.drawLine( x_min_c, y_min_c, x_pos + x_secMin[minute], y_pos + y_secMin[minute]);

  // draw Second
  
 dmd.drawBox(x_pos ,x_pos , x_pos +15, y_pos +15);
 dmd.setPixel(x_pos + x_secMin[second], y_pos + y_secMin[second] , GRAPHICS_OFF);
}

void drawTemp(byte x_pos, byte y_pos)
{
  String MSG;
  MSG= String(temperature)+"'C";
  dmd.drawString(x_pos,y_pos,MSG);
}

void drawHmd(byte x_pos, byte y_pos)
{
  String MSG;
  MSG= String(humidity)+"%";
  dmd.drawString(x_pos,y_pos,MSG);
}


void drawMarquee(byte x,byte y)
{
Link link;
  EEPROM.begin(512);
  EEPROM_readAnything(6,link);
  EEPROM.end();
  
  //dmd.clearScreen();
  dmd.selectFont(SystemFont5x7);
  DMD_TextBox box(dmd, x, y);
  String msg = link.data[random(0,2)] ;
  String tampil = "          "+ msg +"            ";

  //unsigned int tx_long = dmd.stringWidth(tampil); 
  //const char *MSG_chr = tampil.c_str();
  //dmd.drawString(16,0,msg);
  //delay(1000);
  //dmd.scrollX(-16);
  //delay(10000);
  //dmd._drawString(dmd, 0,16, "asdsa",GRAPHICS_ON,SystemFont5x7);

  const char *next = tampil.c_str();
  while(*next) {
  //  Serial.print(*next);
    box.print(*next);
    delay(200);
    next++;
  }
  //Serial.println("==============");
  
}
void drawTextClock(byte x_pos, byte y_pos)
{
  dmd.clearScreen();

  String temp;

  if (hour<10){
    temp = "0"+ String(hour);
  } else {
    temp = String(hour);
  }
  
  dmd.selectFont(angka6x13);
  dmd.drawString(x_pos+ 3, y_pos+ 0, temp);

  if (minute<10){
    temp = "0"+ String(minute);
  } else {
    temp = String(minute);
  }

  dmd.selectFont(Font3x5);
  dmd.drawString(x_pos+ 22, y_pos-2, temp);

  if (second<10){
    temp = "0"+ String(second);
  } else {
    temp = String(second);
  }

  dmd.selectFont(Font3x5);
  dmd.drawString(x_pos+ 22, y_pos + 6, temp);

  int n;
  n = second + 1;

  if (n % 2 == 0) {
    dmd.drawFilledBox(x_pos+ 18, y_pos+4, x_pos+19, y_pos+6, GRAPHICS_OFF);
    dmd.drawFilledBox(x_pos+ 18, y_pos+8, x_pos+19, y_pos+10, GRAPHICS_OFF);
  } else {
    dmd.drawFilledBox(x_pos+ 18, y_pos+4, x_pos+19, y_pos+6);
    dmd.drawFilledBox(x_pos+ 18, y_pos+8, x_pos+19, y_pos+10);
  }
}


void drawTextClockSmall(byte x_pos, byte y_pos)
{
  String jam;
  if (hour<10){
    jam = "0"+ String(hour)+ ":";
  } else {
    jam = String(hour) + ":" ;
  }
  if (minute<10){
    jam += "0" + String(minute);
  } else {
    jam += String(minute);
  }//tampilan jam

  dmd.clearScreen();
  dmd.selectFont(angka_2);
  dmd.drawString(x_pos+1, y_pos+0, jam);
  //Serial.println(jam);
  
}
void drawDate(byte x_pos, byte y_pos)
{
  char weekDay[][7] = {" Ahad ", "SENIN ", "SELASA", " RABU ", "KAMIS ", "JUM'AT", "SABTU "}; // array hari, dihitung mulai dari ahad, hari ahad angka nya =0,
  char monthYear[][4] = { " ", "JAN", "FEB", "MAR", "APR", "MEI", "JUN", "JUL", "AGU", "SEP", "OKT", "NOV", "DES" };

  String hari = weekDay[dayOfWeek];
  String tanggal = String(date) + " " + monthYear[month];

  dmd.clearScreen();
  dmd.selectFont(Font3x5);
  dmd.drawString(x_pos+4, y_pos-2, hari);
  dmd.drawString(x_pos+4, y_pos+7, tanggal);
  //Serial.println(hari + ", " + tanggal);
}

void drawTextOnly(String MSG, byte x, byte y, const uint16_t durr,const uint8_t *font)
{
  dmd.clearScreen();
  dmd.selectFont(font);
  dmd.drawString(x,y, MSG, GRAPHICS_ON);
  delay(durr);
}

