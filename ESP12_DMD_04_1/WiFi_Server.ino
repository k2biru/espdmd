
void AP()
{
 
}

void handleNotFound() {
  String message = "404 :: No body hehe but Sempai !\n\n";
  message += "\n";
  server.send ( 404, "text/plain", message );     // mengirim data 404 (halaman tidak ada) ke pengirim [fungsi debug]
 
}

void handleHome()
{
  String text;
  char temp[20];
  Link link;
  EEPROM.begin(512);
  EEPROM_readAnything(6,link);
  EEPROM.end();
  //byte n = WiFi.scanNetworks();
  WiFi.hostname().toCharArray(temp, 20);
  text = "<html><head><title>Home Of "; text += temp;
  text += "</title><style>body{background-color:#cccccc;font-family:Arial,Helvetica,Sans-Serif;Color:#000088;}";
  text += "</style></head><body><h1>Home Of "; text += temp; text += "</h1>";
  text += "<p>This is the main page of "; text += temp;
  text += "</p><table><tr><th>=========</th><th>=========</th><th>=========</th></tr>";
  text += "<tr><th><a href='/debug'>[DEBUG]</a></th><th><a href='/restart'>[RESTART]</a></th><th><a href='/conf'>[CONFIG]</a></th></tr>";
  text += "<tr><th>=========</th><th>=========</th><th>=========</th></tr></table><br>";
  text += "<p>Setting Wifi</p><form method='POST' action='/update' target='_blak'>";
  text += "SSID:<input type='text' name='newSSID' value='"; text += data.ssid; text += "'><br>Pass:<input type='password' name='newPass' value='"; text += data.pass;
  text += "'> [Opsional]<br>";
  text += "<input type='submit' value='Save'></form><br>";
  text += "<a href='/'>[REFRESH]</a>";

 
  


  
  text += "<form method=\"POST\" action=\"/update\" >";
  text += "  Tanggal:<br>";
  text += "<input type=\"date\" name=\"date\" id=\"myDate\"/> hari : <input type=\"number\" min=\"0\" max=\"6\" name=\"dayW\" id=\"dayW\">";
  text += "  <br>Jam:<br>";
  text += "<input type=\"time\" name=\"time\" id=\"myTime\"/>";
  text += "  <input type=\"submit\" value=\"Perbaharuhi\">";
  text += "</form>";


  text += "<form method=\"POST\" action=\"/update\" >";
  text += " Text/URL<br>";
  text += "[URL/TEXT 1] <input type='checkbox' name='textEn1' value=1 "; if (link.text1) text += "checked"; text += " /><input type=\"text\" name=\"link1\" value=";text+=link.data[0]; text+= "\"> <br>";
  text += "[URL/TEXT 2] <input type='checkbox' name='textEn2' value=1 "; if (link.text2) text += "checked"; text += " /><input type=\"text\" name=\"link2\" value=";text+=link.data[1]; text+= "\"> <br>";
  text += "[URL/TEXT 3] <input type='checkbox' name='textEn3' value=1 "; if (link.text3) text += "checked"; text += " /><input type=\"text\" name=\"link3\" value=";text+=link.data[2]; text+= "\"> <br>";  
  text += "<input type=\"hidden\" name=\"newLink\" value=1 ><input type=\"submit\" value=\"Perbaharuhi\">";
  text += "</form>";

  text += "<script type=\"text/javascript\">";
  text += "var date = new Date();";
  text += "var day = date.getDate();";
  text += "var month = date.getMonth() + 1;";
  text += "var year = date.getFullYear();";
  text += "var ho = date.getHours();";
  text += "var min = date.getMinutes();";
  text += "if (ho < 10) ho = \"0\" + ho;";
  text += "if (min < 10) min = \"0\" + min;";
  text += "if (month < 10) month = \"0\" + month;";
  text += "if (day < 10) day = \"0\" + day;";
  text += "var time1 = ho + \":\" + min;";
  text += "var today = year + \"-\" + month + \"-\" + day;";
  text += "document.getElementById('myDate').value = today;";
  text += "document.getElementById('myTime').value = time1;";  
  text += "document.getElementById('dayW').value = date.getDay();";
  text += "</script>";
  text += "Basic GUI by <a href='mailto:fahrizal.hari@gmail.com'>Fahrizal Hari Utama</a> v1 @512KB<br>(c)Malang ID 2017<br>";
  text += "</body></html>";
  server.send ( 200, "text/html", text );
}

void handleSentData() {
  String message = "SENT DATA\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.send ( 200, "text/plain", message );         // mengirim data 200 (OK data diterima)ke pengirim  [fungsi debug]


if (server.arg("date") != "" || server.arg("time") != "")
  {
    char temp[14], temp1[2], temp2[4];
    byte i;
    byte tg ,bl,jam,menit, dayW;
    uint16_t th;
    
    server.arg("date").toCharArray(temp, 14);
    
    for(i=0; i<=3; i++) temp2[i] = temp[i]; // temp2 berisi tahun
    th=atoi(temp2);

    temp1[0]=temp[5];
    temp1[1]=temp[6];

    bl=atoi(temp1);

    temp1[0]=temp[8];
    temp1[1]=temp[9];

    tg=atoi(temp1);

    if (server.arg("time") != "")
    {
      server.arg("time").toCharArray(temp, 14);

      temp1[0]=temp[0];
      temp1[1]=temp[1];

      jam=atoi(temp1);

      temp1[0]=temp[3];
      temp1[1]=temp[4];

      menit=atoi(temp1);
      
    }
     if (server.arg("dayW") != "") 
     {
      dayW = server.arg("dayW").toInt();
      //if(dayW==0)dayW=7; 
      //dayW--;
      
     }

    updateRTC(tg,bl,th,jam,menit,dayW);
  }

if (server.arg("newLink") != "")
  {
    Link link;
    char tempLink [100];
    server.arg("link1").toCharArray(tempLink, 100);
    strcpy(link.data[0], tempLink);
    
    server.arg("link2").toCharArray(tempLink, 100);
    strcpy(link.data[1], tempLink);
    
    server.arg("link3").toCharArray(tempLink, 100);
    strcpy(link.data[2], tempLink);    

    link.text1 = server.arg("textEn1").toInt();
    link.text2 = server.arg("textEn2").toInt();
    link.text3 = server.arg("textEn3").toInt();

    EEPROM.begin(512);
  Serial.println("tulis EEPROM Link");

  EEPROM_writeAnything(6,link);

  EEPROM.end();
    
  }
  
}
