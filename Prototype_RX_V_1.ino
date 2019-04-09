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
#include <Wire.h>
#include <String.h>


NRFLite _radio;
char data[24];

void setup()
{
    Serial.println("testing...");
    Serial.begin(9600);
    _radio.init(1, 9, 10); // Set this radio's Id = 1, along with its CE and CSN pins
}

void loop()
{
    while (_radio.hasData())
    {
        _radio.readData(&data);
        Serial.println(data);
    }
}
