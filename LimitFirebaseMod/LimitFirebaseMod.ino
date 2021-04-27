#include <WiFi.h>
#include <FirebaseESP32.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include "time.h"

#define FIREBASE_HOST "https://test2-a5ec7-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "rZfgflZT7eRs05fJQ8AH5Dlx1hE9Mxm1FXTXG0jO"

#define WIFI_SSID "Mukhome_EXT"
#define WIFI_PASSWORD "Boatcarbike11"

#define ONE_WIRE_BUS 21
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
char prev_time[10];
const int date_counter = 8;
char date_stack[date_counter][10]={"Empty1",
                        "Empty2",
                        "Empty3",
                        "Empty4",
                        "Empty5",
                        "Empty6",
                        "Empty7",
                        "Empty8"};
int analogPin = A0;
bool hit = true;

/*================================================================ SUPPORTING FUNCTION ====================================================*/

//more like get time since print is commented out. Gets the time and stores in global variable
//No input output
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  month = (timeinfo.tm_mon);
  day = (timeinfo.tm_mday);
  year = timeinfo.tm_year+1900;
  sprintf(curr_time, "%02d:%02d:%02d", timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 
}

//Checks to see if current date is in the local stack  (NOT IN FIREBASE)
//Input : global date stack 2D array, char array (pointer to address of char array)
//Output : bool
bool inPath(char datestack[date_counter][10], char *date_path){ // check if date is already in stackaaa
  //Serial.printf("Lookging for %s\n",date_path);
  for (int i = 0; i < date_counter;i++){
    //Serial.printf("checking if  number  %d in stack is the one ---->%s same as %s\n",i,datestack[i], date_path);
    if (strcmp(datestack[i],date_path)==0){
      //Serial.printf("There is a match\n");
      return true;
    }
  }
  //Serial.printf("There is no match\n");
  return false;
}

//shifts date left in array and returns the address of the oldest entry 
//Input : glpbal date stack 2D array, char array (pointer to address of char array)
//Output : pointer to address of char array of oldest entry
char *shift_date(char datestack[date_counter][10], char* date_path) {
  char* old = (char*)malloc(10);
  //Serial.printf("INSIDE DATE PATH %s\n",date_path);
  strcpy(old,datestack[0]);
  for (int i = 0; i < date_counter - 1; i++) { //shift everything over by 1
    strcpy(datestack[i], datestack[i + 1]);

  }
  strcpy(datestack[date_counter - 1], date_path); //update last entry in stack to newest date
  return old; //retrun old date to delete from firebase
}



/*================================================================ MAIN FUNCTION ====================================================*/
void setup() {
    Serial.begin(115200);
    pinMode(22,OUTPUT); //LED used to verify setup is complete
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
  
  
  printLocalTime();
  strcpy(prev_time,curr_time);
  pinMode(22,OUTPUT);
  digitalWrite(22,HIGH);

  //ADD GRAB DATE HERE--------------->Future implementation
  
}

void loop() {

  printLocalTime(); // get current time
  if(strcmp(curr_time,prev_time) != 0){
    Serial.printf("%s\n",curr_time);
    strcpy(prev_time,curr_time);
    if(((curr_time[3] =='0' && curr_time[4] =='0') ||(curr_time[3] =='3' && curr_time[4] =='0')) && (curr_time[6] =='0' && curr_time[7] =='0') ){ //Every half hour
      Serial.printf("MAGIC NUMBER HIT\n");
      hit = true;
    }
  }

  if (!hit){  //if no hit(not on the hour or half hour, exit this loop and rerun
    return;
    
  }


  
  char date_path[15];
  sprintf(date_path, "%04d/%02d/%02d",year,month,day);
  //Serial.printf("%s\n", date_path);
 


  //this checks to see if a new day has occured. If so, shift date array to the left by one. 
  if(!inPath(date_stack,curr_time)){
    char *p =shift_date(date_stack,curr_time);
    //delete date in firebase
    Firebase.deleteNode(firebaseData,p);
    //create new date from date_path
     free(p); // free memory space created in shift date
     
  }
  
  
  char temp_path[50];
  char pH_path[50];
  sprintf(temp_path,"%s/%s/Temperature", date_path,curr_time);
  sprintf(pH_path,"%s/%s/pH", date_path,curr_time);
  if(Firebase.setFloat(firebaseData,temp_path,(sensors.getTempFByIndex(0)))){
    //SHOULD RETURN TRUE
    Serial.println("Added to database");
    Serial.println(sensors.getTempFByIndex(0));
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


  hit = false;

  
}
