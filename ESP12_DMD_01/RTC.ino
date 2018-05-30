void getRTC()
{
  DateTime now = rtc.now(); //get the current date-time
    uint32_t ts = now.getEpoch();
    rtc.convertTemperature(); 
    hour=now.hour();
    minute=now.minute();
    second=now.second();
    temperature = rtc.getTemperature();
}



