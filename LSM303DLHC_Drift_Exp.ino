/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LSM303.h>
#include <String.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

LSM303 compass;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();

  

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);

  //todo: Put in SSID and Password
  WiFiMulti.addAP("HOME-C360-2.4", "follow1803daily2");

}

void loop() {
  
   /* Get a new sensor event */ 
  compass.read();
  Serial.print("X: "); Serial.print(compass.m.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(compass.m.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(compass.m.z); Serial.print("  ");Serial.println("uT");

      
 
    

  float X = (compass.m.x);
  float Y = (compass.m.y);
  float Z = (compass.m.z);

  //todo update Board_Num
  String data = "Board_Num=5&Mag_Name=LSM303DHC&X=";
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
      http.POST(data);
      http.writeToStream(&Serial);
      http.end();
      Serial.print("[HTTP] PUT...\n");
      http.end();
      } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  } else {
    
  }

  delay(300000);
}
