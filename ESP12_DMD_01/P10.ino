
void drawClockSqr(byte x_pos, byte y_pos)
{
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

void drawMarquee()
{
  //dmd.clearScreen();
  dmd.selectFont(SystemFont5x7);
  DMD_TextBox box(dmd, 16, 8);
  String msg = "lupakanlah";
  String tampil = "           "+msg+"           ";

  //unsigned int tx_long = dmd.stringWidth(tampil); 
  //const char *MSG_chr = tampil.c_str();
  dmd.drawString(16,0,msg);
  delay(1000);
  dmd.scrollX(-16);
  delay(10000);
  //dmd._drawString(dmd, 0,16, "asdsa",GRAPHICS_ON,SystemFont5x7);

  //const char *next = tampil.c_str();
  //while(*next) {
  //  Serial.print(*next);
  //  box.print(*next);
  //  delay(200);
  //  next++;
  //}
  Serial.println("==============");

  
}

