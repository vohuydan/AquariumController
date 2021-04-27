#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 21
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200); 
 Serial.println("Dallas Temperature IC Control Library Demo"); 
 // Start up the library 
 sensors.begin(); 
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 Serial.println("DONE"); 
/********************************************************************/
 Serial.print("Temperature is: "); 
 Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
   delay(1000); 
}
