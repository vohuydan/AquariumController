This repo contains the work of a custom Aquarium Controller.

# Objective and Purpose

The objective of this project is to create a tool to log pH of an reef tank and have the data in the cloud for viewing at any location. 





Firebase was chosen as the backend of this project for the ease of creating a user authentication and real time data base. Firebase easily integrates with the ESP32 and Android, and also has a lot of supporting documentation. 

PHP/MySQL were considered and would be plausible for local or single person use, however, for large scale production, Firebase would be more ideal with the added benefit of authentication.

The board used for this project is the ESP32 development board. The reason for choosing this board is because it has a built in WIFI and BLE module and can be used with the Arduino IDE for quick prototyping. A better option would be the  RaspberryPi Zero W, which would add on an SD card for local storage. The physcal footprint of both boards are small enough that a PCB will not be needed. Lastly, at the $10 price point(for each board), this is relatively cheap.

Assumptions: WIFI will always be available with no interuptions. Power will not be interrupted.

Data Logging: The code for ESP32 keeps track of 8 continuous days. Firebase will show a maximum of 8 days worth of data. Every half an hour, data is logged and uploaded to Firebase. On the 9th day, the ESP32 will delete the oldest day from the realtime database. 

4/27/2021
Created repo. ESP32 is logging data and uploading to Firebase at every half hour interval. 
![firebase](https://user-images.githubusercontent.com/77210680/116316478-dd864880-a766-11eb-858b-f6c90a4ecdb7.PNG)
