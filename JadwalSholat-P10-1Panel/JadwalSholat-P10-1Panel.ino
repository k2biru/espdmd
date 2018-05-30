/*
   JADWAL WAKTU SHOLAT MENGGUNAKAN NODEMCU ESP8266, LED P10, RTC DS3241, BUZZER
   FITUR :  JADWAL SHOLAT 5 WAKTU DAN TANBIH IMSAK, JAM BESAR, TANGGAL, SUHU, ALARAM ADZAN DAN TANBIH IMSAK,
            DAN HITUNG MUNDUR IQOMAH DAN UPDATE SCROLL TEKS MALALUI WIFI.


  Pin on  DMD P10     GPIO      NODEMCU               Pin on  DS3231      NODEMCU                   Pin on  Buzzer       NODEMCU

        2  A        GPIO16    D0                            SCL         D1 (GPIO 5)                       +            RX (GPIO 3)
        4  B        GPIO12    D6                            SDA         D2 (GPIO 4)                       -            GND
        8  CLK      GPIO14    D5                            VCC         3V
        10 SCK      GPIO0     D3                            GND         GND
        12 R        GPIO13    D7
        1  NOE      GPIO15    D8
        3  GND      GND       GND

  Catatan :

  o Perlu Power Eksternal 5V ke LED P10.
  o Saat Flashing (upload program) cabut sementara pin untuk buzzer.

  Eksternal Library

  - DMD2 : https://github.com/freetronics/DMD2
  - PrayerTime : https://github.com/asmaklad/Arduino-Prayer-Times
  - RTC DS3231 : https://github.com/SodaqMoja/Sodaq_DS3231

  email : bonny@grobak.net - www.grobak.net
*/

#include <ESP8266WiFi.h>
#include "espneotext.h"
#include <SPI.h>
#include <EEPROM.h>


#include <Wire.h>
#include <Sodaq_DS3231.h>

#include <DMD2.h>
#include "fonts/Arial_Black_16.h"
#include "fonts/SystemFont5x7.h"
#include "fonts/angka6x13.h"
#include "fonts/angka_2.h"
#include "fonts/Font3x5.h"

#include "PrayerTimes.h"

#define pin_A 16
#define pin_B 12
#define pin_sclk 0
#define pin_clk 14
#define pin_r 13
#define pin_noe 15

// Inisiasi Jadwal Sholat
double times[sizeof(TimeName) / sizeof(char*)];

// Durasi waktu iqomah
byte value_iqmh;
byte iqomahsubuh = 10;    // Durasi Iqomah dalam detik
byte iqomahdzuhur = 10;   // Durasi Iqomah dalam detik
byte iqomahashar = 7;     // Durasi Iqomah dalam detik
byte iqomahmaghrib = 5;   // Durasi Iqomah dalam detik
byte iqomahisya = 7;      // Durasi Iqomah dalam detik

int durasiadzan = 180000; // Durasi Adzan 1 detik = 1000 ms, 180000 berarti 180 detik atau 3 menit

byte value_ihti = 2; // Koreksi Waktu Menit Jadwal Sholat
byte value_hari;
//byte S_IQMH = 0, S_IHTI = 0, S_HARI = 0;



// BUZZER
const int buzzer = 3; // Pin GPIO Buzzer - RX

//SETUP RTC
//year, month, date, hour, min, sec and week-day(Senin 0 sampai Ahad 6)
//DateTime dt(2018, 04, 19, 01, 15, 0, 4);
char weekDay[][7] = {"SENIN ", "SELASA", " RABU ", "KAMIS ", "JUM'AT", "SABTU ", " AHAD ", "SENIN "}; // array hari, dihitung mulai dari senin, hari senin angka nya =0,
char monthYear[][4] = { " ", "JAN", "FEB", "MAR", "APR", "MEI", "JUN", "JUL", "AGU", "SEP", "OKT", "NOV", "DES" };

//SETUP DMD
#define DISPLAYS_WIDE 2
#define DISPLAYS_HIGH 1
SPIDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, pin_noe, pin_A, pin_B, pin_sclk); // DMD controls the entire display
DMD_TextBox box(dmd);  // "box" provides a text box to automatically write to/scroll the display

//SETUP WIFI AP
IPAddress local_IP(192, 168, 7, 77);
IPAddress gateway(192, 168, 7, 250);
IPAddress subnet(255, 255, 255, 0);

const char* ssid = "yozora";
const char* pass = ""; // Password bisa dikosongkan jika diinginkan
String value;
WiFiServer server(80);


// setup rutinitas yang hanya dilakukan satu kali saat tekan reset:
void setup() {
  delay(1000);

  //Serial.begin(115200);

  //RTC D3231
  Wire.begin();
  rtc.begin();
  //rtc.setDateTime(dt); // Setting tanggal dan waktu untuk disimpan di RTC DS3231 sesuai parameter Datetime dt() di atas

  //DMD
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid,pass);
  dmd.setBrightness(20);
  dmd.begin();

  //Buzzer
  pinMode(buzzer, OUTPUT);      // inisiasi pin untuk buzzer
  digitalWrite(buzzer, LOW);
  delay(50);

  BuzzerPendek();

  //WIFI AP
  //Serial.println();
  //Serial.println();
  //Serial.print("Konfigurasi WiFi access point ... ");
  //Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Berhasil" : "Gagal!");
 
  //Serial.print("Setting soft-AP ... ");
  //Serial.println(WiFi.softAP(ssid,pass) ? "Berhasil" : "Gagal!");
  //Serial.println("");

  //Serial.print("WiFi SSID : ");
  //Serial.println(ssid);
  //Serial.print("WiFi Password : ");
  //Serial.println(pass);
  //Serial.print("WiFi IP address = ");
  //Serial.println(WiFi.softAPIP());
  //Serial.println("");

  // Start the web server
  server.begin();
  //Serial.println("Server started");

  // INTRO BRANDING

  dmd.clearScreen();
  dmd.selectFont(Font3x5);
  dmd.drawString(4, -1, "RisTIE HME FT-UB");
  dmd.drawString(2, 7, ". . : : Inori : : . .");
  delay(3000);

  dmd.clearScreen();
  dmd.selectFont(Arial_Black_16);
  dmd.drawString(-2, 1, "JWS");
  delay(2000);

  dmd.clearScreen();
  dmd.selectFont(Font3x5);
  dmd.drawString(7, 3, "VER.1");
  delay(2000);

  dmd.clearScreen();
  dmd.selectFont(Font3x5);
  dmd.drawString(1, -2, "ALMT IP");
  DMD_TextBox box(dmd, 0, 7);
  String ip = WiFi.softAPIP().toString();
  const char *alamatip = ip.c_str();
  const char *lanjut = alamatip;
  while (*lanjut) {
    //Serial.print(*lanjut);
    box.print(*lanjut);
    delay(250);
    lanjut++;
  }
  delay(2000);

}


long transisi = 0;

// loop rutinitas yang dijalankan berulang selamanya:
void loop() {

  AlarmSholat(); // Banyak dipanggil class AlarmSholat() ini agar waktu sholat lebih akurat
  TampilJam();

  if (millis() - transisi > 15000) { // Tampilkan Tanggal pada detik ke 15
    AlarmSholat();
    TampilTanggal();
  }

  if (millis() - transisi > 18000) { // Tampilkan Suhu pada detik ke 18

    AlarmSholat();
    TampilSuhu();

    AlarmSholat();
    TampilJadwalSholat();

    AlarmSholat();
    WebTeks();

    transisi = millis();
  }

}


//----------------------------------------------------------------------
//JADWAL SHOLAT

void JadwalSholat() {

  DateTime now = rtc.now();

  int tahun = now.year();
  int bulan = now.month();
  int tanggal = now.date();

  int dst = 7; // TimeZone

  set_calc_method(Karachi);
  set_asr_method(Shafii);
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);
  set_isha_angle(18);

  //SETTING LOKASI DAN WAKTU Masjid Miftahul Jannah
  float latitude = 7.9666;
  float longitude = 112.6326;

  get_prayer_times(tahun, bulan, tanggal, latitude, longitude, dst, times);

}

void TampilJadwalSholat() {

  JadwalSholat();

  char sholat[7];
  char jam[5];
  char TimeName[][6] = {"SUBUH", "TRBIT", "DZUHR", "ASHAR", "TRBNM", "MGHRB", "ISYA'"};
  int hours, minutes;

  for (int i = 0; i < 7; i++) {

    get_float_time_parts(times[i], hours, minutes);

    minutes = minutes + value_ihti;

    if (minutes >= 60) {
      minutes = minutes - 60;
      hours ++;
    }

    if (i == 0 || i == 2 || i == 3 || i == 5 || i == 6) { //Tampilkan hanya Subuh, Dzuhur, Ashar, Maghrib, Isya
      sprintf(sholat, "%s", TimeName[i]);
      sprintf(jam, "%02d:%02d", hours, minutes);
      dmd.clearScreen();
      dmd.selectFont(Font3x5);
      dmd.drawString(6, -2, sholat);
      dmd.selectFont(angka_2);
      dmd.drawString(1, 8, jam);
      //Serial.println(sholat);
      //Serial.println(" : ");
      //Serial.println(jam);
      delay(2000);
    }
  }

  //Tambahan Waktu Tanbih (Peringatan 10 menit sebelum mulai puasa) yang biasa disebut Imsak

  get_float_time_parts(times[0], hours, minutes);
  minutes = minutes + value_ihti;
  if (minutes < 11) {
    minutes = 60 - minutes;
    hours --;
  } else {
    minutes = minutes - 10 ;
  }
  sprintf(jam, "%02d:%02d", hours, minutes);
  dmd.clearScreen();
  dmd.selectFont(Font3x5);
  dmd.drawString(4, -2, "TANBIH");
  dmd.selectFont(angka_2);
  dmd.drawString(1, 8, jam);
  //Serial.print("TANBIH");
  //Serial.println(" : ");
  //Serial.println(jam);
  delay(1000);

}

void AlarmSholat() {

  DateTime now = rtc.now();

  int Hari = now.dayOfWeek();
  int Hor = now.hour();
  int Min = now.minute();
  int Sec = now.second();

  JadwalSholat();

  int hours, minutes;

  // Tanbih atau Imsak
  get_float_time_parts(times[0], hours, minutes);
  minutes = minutes + value_ihti;

  if (minutes < 11) {
    minutes = 60 - minutes;
    hours --;
  } else {
    minutes = minutes - 10 ;
  }

  if (Hor == hours && Min == minutes) {
    dmd.clearScreen();
    dmd.selectFont(Font3x5);
    dmd.drawString(4, -2, "TANBIH"); //koordinat tampilan
    dmd.drawString(6, 7, "IMSAK"); //koordinat tampilan
    BuzzerPendek();
    //Serial.println("TANBIH");
    delay(60000);
  }

  // Subuh
  get_float_time_parts(times[0], hours, minutes);
  minutes = minutes + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    dmd.clearScreen();
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN"); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(6, 7, "SUBUH"); //koordinat tampilan
    BuzzerPanjang();
    //Serial.println("SUBUH");
    delay(durasiadzan);//180 detik atau 3 menit untuk adzan

    BuzzerPendek();
    value_iqmh = iqomahsubuh;    //Saat Subuh tambah 2 menit waktu Iqomah
    Iqomah();
  }

  // Dzuhur
  get_float_time_parts(times[2], hours, minutes);
  minutes = minutes + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes && Hari != 4) {

    dmd.clearScreen();
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN"); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(4, 7, "DZUHUR"); //koordinat tampilan
    BuzzerPanjang();
    //Serial.println("DZUHUR");
    delay(durasiadzan);//180 detik atau 3 menit untuk adzan

    BuzzerPendek();
    value_iqmh = iqomahdzuhur;
    Iqomah();

  } else if (Hor == hours && Min == minutes && Hari == 4) {

    dmd.clearScreen();
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN"); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(4, 7, "JUM'AT"); //koordinat tampilan
    BuzzerPanjang();
    //Serial.println("Adzan Jum'at");
    delay(durasiadzan);//180 detik atau 3 menit untuk adzan
    BuzzerPanjang();

    PesanTeks();
  }

  // Ashar
  get_float_time_parts(times[3], hours, minutes);
  minutes = minutes + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    dmd.clearScreen();
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN "); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(6, 7, "ASHAR"); //koordinat tampilan
    BuzzerPanjang();
    //Serial.println("ASHAR");
    delay(durasiadzan);//180 detik atau 3 menit untuk adzan

    BuzzerPendek();
    value_iqmh = iqomahashar;
    Iqomah();
  }

  // Maghrib
  get_float_time_parts(times[5], hours, minutes);
  minutes = minutes + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    dmd.clearScreen();
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN "); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(1, 7, "MAGHRIB"); //koordinat tampilan
    BuzzerPanjang();
    //Serial.println("MAGHRIB");
    delay(durasiadzan);//180 detik atau 3 menit untuk adzan

    BuzzerPendek();
    value_iqmh = iqomahmaghrib;
    Iqomah();
  }

  // Isya'
  get_float_time_parts(times[6], hours, minutes);
  minutes = minutes + value_ihti;

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    dmd.clearScreen();
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "ADZAN"); //koordinat tampilan
    dmd.selectFont(Font3x5);
    dmd.drawString(8, 7, "ISYA'"); //koordinat tampilan
    BuzzerPanjang();
    //Serial.println("ISYA");
    delay(durasiadzan);//180 detik atau 3 menit untuk adzan

    BuzzerPendek();
    value_iqmh = iqomahisya;
    Iqomah();
  }

}


//----------------------------------------------------------------------
//IQOMAH

void Iqomah() {

  JadwalSholat();

  DateTime now = rtc.now();
  //iqomah----------------------------------
  int langkah, i, hours, minutes;
  dmd.clearScreen();
  dmd.selectFont(Font3x5);
  //dmd.clearScreen( true );
  dmd.drawString(3, -2, "IQOMAH"); //koordinat tampilan
  int tampil;
  //value_iqmh = EEPROM.read(0);

  int detik = 0, menit = value_iqmh;
  for (detik = 0; detik >= 0; detik--) {
    delay(1000);
    String iqomah = Konversi(menit) + ":" + Konversi(detik);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 9, iqomah);
    if (detik <= 0) {
      detik = 60;
      menit = menit - 1;
    } if (menit <= 0 && detik <= 1) {
      dmd.clearScreen();
      digitalWrite(2, HIGH);//alarm Iqomah
      delay(1000);
      digitalWrite(2, LOW);//alarm Iqomah
      delay(50);
      dmd.selectFont(Font3x5);
      dmd.drawString(2, -2, "LURUS 8"); //koordinat tampilan
      dmd.drawString(0, 7, "RAPATKAN"); //koordinat tampilan
      delay(10000);
      for (tampil = 0; tampil < 300 ; tampil++) { // 300 detik atau 5 menit nilai tunda sholt
        menit = 0;
        detik = 0;
        dmd.clearScreen();
        dmd.selectFont(Font3x5);
        dmd.drawString(4, -2, "SHOLAT"); //koordinat tampilan
        ///////////////////////
        now = rtc.now();
        String jam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()) ; //tampilan jam
        dmd.selectFont(Font3x5);
        dmd.drawString(2, 7, jam);
        /////////////////////
        delay (1000);
      }
    }
  }

  value_iqmh = 0;   // Kembalikan waktu iqomah ke 0
}


//----------------------------------------------------------------------
//TAMPILKAN JAM BESAR

void TampilJam() {
  DateTime now = rtc.now();

  dmd.clearScreen();

  //JAM
  String jam = Konversi(now.hour()) ; //tampilan jam
  dmd.selectFont(angka6x13);
  dmd.drawString(3, 0, jam);

  //MENIT
  String menit = Konversi(now.minute()) ; //tampilan menit
  dmd.selectFont(Font3x5);
  dmd.drawString(22, -2, menit);

  //DETIK
  String detik = Konversi(now.second()) ; //tampilan menit
  dmd.selectFont(Font3x5);
  dmd.drawString(22, 6, detik);


  //DETIK TITIK DUA
  int n;
  n = now.second() + 1;

  if (n % 2 == 0) {
    dmd.drawFilledBox(18, 4, 19, 6, GRAPHICS_OFF);
    dmd.drawFilledBox(18, 8, 19, 10, GRAPHICS_OFF);
  } else {
    dmd.drawFilledBox(18, 4, 19, 6);
    dmd.drawFilledBox(18, 8, 19, 10);
  }

  //Serial.println(jam + " : " + menit + " : " + detik);
  delay(1000);

}

void TampilJamKecil() {
  DateTime now = rtc.now();

  String jam = Konversi(now.hour()) + ":" + Konversi(now.minute()); //tampilan jam

  dmd.clearScreen();
  dmd.selectFont(angka_2);
  dmd.drawString(1, 0, jam);
  //Serial.println(jam);

}


//----------------------------------------------------------------------
//TAMPILKAN TANGGAL

void TampilTanggal() {
  DateTime now = rtc.now();

  String hari = weekDay[now.dayOfWeek()];
  String tanggal = Konversi(now.date()) + " " + monthYear[now.month()];

  dmd.clearScreen();
  dmd.selectFont(Font3x5);
  dmd.drawString(4, -2, hari);
  dmd.drawString(4, 7, tanggal);
  //Serial.println(hari + ", " + tanggal);
  delay(3000);

}


//----------------------------------------------------------------------
// TAMPILKAN SUHU

void TampilSuhu() {
  //Tampilkan Suhu
  dmd.clearScreen();
  dmd.selectFont(Font3x5);
  dmd.drawString(9, -2, "SUHU");
  String suhu = Konversi(rtc.getTemperature()) + "C";
  dmd.selectFont(SystemFont5x7);
  dmd.drawString(8, 8, suhu);
  //Serial.println("SUHU : " + suhu + "C");
  delay(3000);
}


//----------------------------------------------------------------------
// BUNYIKAN BEEP BUZZER

void BuzzerPanjang() {
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  delay(50);
}

void BuzzerPendek() {
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(50);
}

//----------------------------------------------------------------------
// TAMPILKAN SCROLLING TEKS YANG DIINPUT MELALUI WEBSITE

void WebTeks() {
  // Check if a client has connected
  WiFiClient client = server.available();

  // Read the first line of the request
  String command1 = client.readStringUntil('/');
  String command = client.readStringUntil('/');
  //Serial.println(command);

  if (command == "text") {
    value = client.readStringUntil('/');
    value.replace("%20", " ");
    //Serial.println(value);
  }

  else {  // Prepare the response
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    s += file1;
    client.flush();

    // Send the response to the client
    while (s.length() > 2000) {
      String dummy = s.substring(0, 2000);
      client.print(dummy);
      s.replace(dummy, " ");
    }

    client.print(s);
    delay(1);
    ////Serial.println("Client disconnected");

  }

  dmd.clearScreen();
  TampilJamKecil();
  dmd.selectFont(Font3x5);
  DMD_TextBox box(dmd, 0, 8);
  String spasi = "          ";
  String logo = "Tanpa adanya suatu kemauan yang keras kalian tidak akan pernah mendapatkan hasil yang maksimal." + spasi;
  String tampil = spasi + logo + value + spasi;
  const char *MESSAGE = tampil.c_str();
  const char *next = MESSAGE;
  while (*next) {
    //Serial.print(*next);
    box.print(*next);
    delay(200);
    next++;
    
  }
}


void PesanTeks() {

  for (int i = 0; i < 5; i++) {
    dmd.clearScreen();
    dmd.selectFont(Font3x5);
    dmd.drawString(4, -2, "JUM'AT"); //koordinat tampilan
    DMD_TextBox box(dmd, 0, 8);
    String spasi = "          ";
    String pesan = "PERIKSA KEMBALI HP ANDA DAN PASTIKAN SUDAH DI MATIKAN DEMI KEKHUSUAN DALAM BERIBADAH" + spasi;
    String tampil = spasi + pesan + spasi;
    const char *MESSAGE = tampil.c_str();
    const char *next = MESSAGE;
    while (*next) {
      //Serial.print(*next);
      box.print(*next);
      delay(200);
      next++;
    }

  }
}



//----------------------------------------------------------------------
///Konversi angka agar ada nol didepannya jika diawah 10

String Konversi(int sInput) {
  if (sInput < 10)
  {
    return"0" + String(sInput);
  }
  else
  {
    return String(sInput);
  }
}
