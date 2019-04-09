/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <Wire.h>
#include <String.h>

#include <FaBo9Axis_MPU9250.h>

FaBo9Axis fabo_9axis;

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
    Serial.println("RESET");
  Serial.println();

 


  Serial.println("configuring device.");

  if (fabo_9axis.begin()) {
    Serial.println("configured FaBo 9Axis I2C Brick");
  } else {
    Serial.println("device error");
    while(1);
  }

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
    float mx,my,mz;
    fabo_9axis.readMagnetXYZ(&mx,&my,&mz);
    Serial.print("mx: ");
    Serial.print(mx);
    Serial.print(" my: ");
    Serial.print(my);
    Serial.print(" mz: ");
    Serial.println(mz);

  //todo Update Board Num
  String data = "Board_Num=8&Mag_Name=MPU9250&X=";
data.concat(mx);
data.concat("&Y=");
data.concat(my);
data.concat("&Z=");
data.concat(mz);
  
  
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
  }

  delay(300000);
}
