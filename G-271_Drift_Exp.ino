/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <String.h>
 
/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();
  

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);

  //todo Update SSID and Password
  WiFiMulti.addAP("HOME-C360-2.4", "follow1803daily2");

}

void loop() {


  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
   /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

      
 
    

  float X = (event.magnetic.x);
  float Y = (event.magnetic.y);
  float Z = (event.magnetic.z);
  //todo Update Board Num
  String data = "Board_Num=4&Mag_Name=G_271&X=";
data.concat(X);
data.concat("&Y=");
data.concat(Y);
data.concat("&Z=");
data.concat(Z);
  
  
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://home.cs.dartmouth.edu/~hud/add.php")) {  // HTTP
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      //http.POST("Board_Num=YIKES&Mag_Name=HELLYEAH&Timestamp=1234&X=" + X + "&Y=" + Y + "&Z=" + Z);
      http.POST(data);
      http.writeToStream(&Serial);
      http.end();
      Serial.print("[HTTP] PUT...\n");
      http.end();
      } else {
      Serial.printf("[HTTP} Unable to connect\n");
    } 
  }

  delay(300000);
}
