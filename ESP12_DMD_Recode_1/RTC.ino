void getRTC()
{
  DateTime now = rtc.now(); //get the current date-time
    uint32_t ts = now.getEpoch();
    rtc.convertTemperature(); 
    hour=now.hour();
    minute=now.minute();
    second=now.second();
    month=now.month();
    date=now.date();
    dayOfWeek=now.dayOfWeek();
    year=now.year();
    temperature = rtc.getTemperature();
    
}

void updateRTC(byte tg,byte bl,uint16_t th,byte jam, byte menit, byte dayW)
{
  DateTime dt(th, bl, tg, jam, menit, 0, dayW);
  rtc.setDateTime(dt);
}


