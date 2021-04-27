
//Include WiFi.h
#include <WiFi.h>

//Include Firebase ESP32 library (this library)
#include <FirebaseESP32.h>


#define FIREBASE_HOST "https://test2-a5ec7-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "rZfgflZT7eRs05fJQ8AH5Dlx1hE9Mxm1FXTXG0jO"

#define WIFI_SSID "Mukhome_EXT"
#define WIFI_PASSWORD "Boatcarbike11"


//Define the Firebase Data object
FirebaseData firebaseData;
FirebaseJson json;



int analogPin = A0;
int val = 0;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
 
  /*
  This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
  Firewall that allows only GET and POST requests.
  
  Firebase.enableClassicRequest(firebaseData, true);
  */
 
  //String path = "/data";
   Serial.println("------------------------------------");
  Serial.println("Connected...");
}

void loop() {
  // put your main code here, to run repeatedly:
  
 /*val = analogRead(analogPin);  // read the input pin
  int Sdata = map(val,0,4095,0,1000);
  Serial.println(Sdata);
  delay(100); 
  json.set("/data", Sdata);
  Firebase.updateNode(firebaseData,"/Sensor",json);*/
  String path =String(count);
  //Firebase.push(firebaseData,path,0);
  count++;
  delay(1000);
}
