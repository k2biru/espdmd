void display1()
{
  drawClockSqr(0, 0); 
  dmd.selectFont(Comic_Sans_MS_13);
  drawTemp(17, 2);
}

void display2()
{
  drawTextClock(0, 0);
}

void display3()
{
  drawDate(0, 0);
}

void display4()
{
  drawTextClockSmall(0, 0);
}

void display5()
{
  dmd.clearScreen();
  drawTemp(13, 2);
}

void display6()
{
  if (isnan(humidity))humidity = dht.readHumidity();
 
  if (!isnan(humidity)) {
  dmd.clearScreen();
  dmd.selectFont(Comic_Sans_MS_13);
  drawHmd(17,2); 
  }

}

void display7()
{
  drawTextClockSmall(17, 3);
}

void display8()
{
  drawTextClock(16, 0);
}



