#include <WiFi.h>
#include <FirebaseESP32.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include "time.h"

#define FIREBASE_HOST "https://esp32-test-47764-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "LhTKeNxqApl0vwiT2aQLJkPC4wau1erTMwpXbzNp"

#define WIFI_SSID "Mukhome_EXT"
#define WIFI_PASSWORD "Boatcarbike11"

#define ONE_WIRE_BUS 22
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -28800;
const int   daylightOffset_sec = 3600;

FirebaseData firebaseData;
FirebaseJson json;

int month;
int day;
int year;
char curr_time[10];

int analogPin = A0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  month = (timeinfo.tm_mon);
  day = (timeinfo.tm_mday);
  year = timeinfo.tm_year+1900;
  sprintf(curr_time, "%02d:%02d:%02d", timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 
}

void setup() {
    Serial.begin(115200);
    sensors.begin(); // Start reading temperatures
 WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
   Serial.println("------------------------------------");
  Serial.println("Connected...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
}

void loop() {

  printLocalTime(); // get current time
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  //char addToTime[50] = year + "/" + month + "/"+ day + curr_time; //Path to store data 
  char temp_path[50];
  char pH_path[50];
  sprintf(temp_path,"%04d/%02d/%02d/%s/Temperature", year,month,day,curr_time);
  sprintf(pH_path,"%04d/%02d/%02d/%s/pH", year,month,day,curr_time);

  if(Firebase.setFloat(firebaseData,temp_path,(sensors.getTempCByIndex(0)))){
    //SHOULD RETURN TRUE
    Serial.println("Added to database");
    Serial.println(sensors.getTempCByIndex(0));
  }else{
    Serial.print("setting temp failed :");
        delay(500);
        return;
  }
if(Firebase.setInt(firebaseData,pH_path,analogRead(analogPin))){
    //SHOULD RETURN TRUE
    Serial.println("Added to database");
    Serial.println(analogRead(analogPin));
  }else{
    Serial.print("setting pH failed :");
        delay(500);
        return;
  }
    Serial.println("Setting successful");
    Serial.println();
    
    delay(5000);

}
