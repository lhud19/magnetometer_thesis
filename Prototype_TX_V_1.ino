/*

Demonstrates simple RX and TX operation.
Please read the notes in NRFLite.h for a description of all library features.

Radio    Arduino
CE    -> 9
CSN   -> 10 (Hardware SPI SS)
MOSI  -> 11 (Hardware SPI MOSI)
MISO  -> 12 (Hardware SPI MISO)
SCK   -> 13 (Hardware SPI SCK)
IRQ   -> No connection
VCC   -> No more than 3.6 volts
GND   -> GND

*/

#include <SPI.h>
#include <NRFLite.h>

#include <Arduino.h>
#include <Wire.h>
#include <String.h>

#include <FaBo9Axis_MPU9250.h>

FaBo9Axis fabo_9axis;

NRFLite _radio;
char data[24];
char BufX[8];
char BufY[8];
char BufZ[8];
void setup()
{
    Serial.begin(9600);
    Serial.println("We are testing now...");
    _radio.init(0, 9, 10); // Set transmitter radio to Id = 0, along with the CE and CSN pins
     
    if (fabo_9axis.begin()) {
    Serial.println("configured FaBo 9Axis I2C Brick");
  } else {
    Serial.println("device error");
    while(1);
  }



}


void loop()
{
    float mx,my,mz;
    fabo_9axis.readMagnetXYZ(&mx,&my,&mz);
    
    Serial.print("mx: ");
    Serial.print(mx);
    Serial.print(" my: ");
    Serial.print(my);
    Serial.print(" mz: ");
    Serial.println(mz);

    
    dtostrf(mx, 4, 2, BufX);
    dtostrf(my, 4, 2, BufY);
    dtostrf(mz, 4, 2, BufZ);

    data[0] = '\0';

    strcat(data, "S9: ");
    strcat(data, BufX);
    strcat(data, " ");
    strcat(data, BufY);
    strcat(data, " ");
    strcat(data, BufZ);
    Serial.println(data);


    
    
    _radio.send(1, &data, sizeof(data)); // Send to the radio with Id = 1
    Serial.println("Data sent.");
    delay(1000);
}
