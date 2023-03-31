#include <SPI.h>
#include <SD.h>
#include <RTClib.h>

File myFile;
RTC_DS3231 rtc;
char t[32];
char w[32];

//inisialisasi variabel Anemo
const float pi = 3.14159265; // pi number
int period = 1000; // Measurement period (miliseconds)
int delaytime = 1000; // Time between samples (miliseconds)
int radio = 90; // Distance from center windmill to outer cup (mm)
int jml_celah = 18; // jumlah celah sensor
unsigned int Sample = 0; // Sample number
unsigned int counter = 0; // B/W counter for sensor
unsigned int RPM = 0; // Revolutions per minute
float speedwind = 0; // Wind speed (m/s)

//inisialisasi LED
int led_hijau = 7;

//inisialisasi microsd
int CS = 10;

void setup() {
  Serial.begin(57600);

//inisialisasi rtc
  rtc.begin();
  Wire.begin();

    if (rtc.lostPower()){ //Setelah upload pertama, comment line if ini
      rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
    }

//anemo
pinMode(2, INPUT);
digitalWrite(2, HIGH);
//microsd
SD.begin(10);
//LED
pinMode(led_hijau, OUTPUT);
}

void windvelocity()
{
speedwind = 0;
counter = 0;
attachInterrupt(0, addcount, CHANGE);
unsigned long millis();
long startTime = millis();
Serial.println(startTime);
while(millis() < startTime + period) {}

detachInterrupt(1);
}

void RPMcalc()
{
RPM=((counter/jml_celah)*60)/(period/1000); // Calculate revolutions per minute (RPM)
}

void WindSpeed()
{
speedwind = ((2 * pi * radio * RPM)/60) / 1000; // Calculate wind speed on m/s
}

void addcount()
{
counter++;
}

void loop() {
  myFile = SD.open("theta15.txt", FILE_WRITE);
  DateTime now = rtc.now();
  
  //Indikator
  if (myFile){
    digitalWrite(led_hijau, HIGH);
    delay(500);
    digitalWrite(led_hijau, LOW);
    
    Sample++;
    Serial.print(Sample);
    Serial.print(": Start measurement…");
    windvelocity();
    Serial.println(" finished.");
    Serial.print("Counter: ");
    Serial.print(counter);
    Serial.print("; RPM: ");
    RPMcalc();
    Serial.print(RPM);
    Serial.print("; Wind speed: ");
    WindSpeed();
    Serial.print(speedwind);
    Serial.print(" [m/s]");
    Serial.println();
    sprintf(t, "%02d.%02d.%02d",  now.day(), now.month(), now.year());
    sprintf(w, "%02d:%02d:%02d",  now.hour(), now.minute(), now.second());

    Serial.print(t);
    Serial.print(" ");
    Serial.print(w);

    delay(1000);

    //SD Card

    //myFile.print(rtc.now());
    myFile.print(t);
    myFile.print(" ");
    myFile.print(w);
    myFile.print(",");
    myFile.print(Sample);
    myFile.print(",");
    myFile.print(speedwind);
    myFile.println();

    myFile.close();
    Serial.println("DONE");
  } else {
    Serial.print("Error Opening File");
    digitalWrite(led_hijau, HIGH);
  }

}
