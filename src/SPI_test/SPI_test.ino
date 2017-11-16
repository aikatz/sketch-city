/*
 SCP1000 Barometric Pressure Sensor Display

 Shows the output of a Barometric Pressure Sensor on a
 Uses the SPI library. For details on the sensor, see:
 http://www.sparkfun.com/commerce/product_info.php?products_id=8161
 http://www.vti.fi/en/support/obsolete_products/pressure_sensors/

 This sketch adapted from Nathan Seidle's SCP1000 example for PIC:
 http://www.sparkfun.com/datasheets/Sensors/SCP1000-Testing.zip

 Circuit:
 SCP1000 sensor attached to pins 6, 7, 10 - 13:
 DRDY: pin 6
 CSB: pin 10
 MOSI: pin 11
 MISO: pin 12
 SCK: pin 13

 created 31 July 2010
 modified 14 August 2010
 by Tom Igoe
 */

// the sensor communicates using SPI, so include the library:
#include <SPI.h>


// pins used for the connection with the sensor
// the other you need are controlled by the SPI library):
const int chipSelectPin = 10;

void setup() {
  Serial.begin(9600);

  // start the SPI library:
  SPI.begin();

  pinMode(chipSelectPin, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

}

void loop() {
  for(byte x = 0; x<5; x++){
    for(byte y = 0; y < 4; y++) {
      PORTB = (7 << 5) | (x << 2) | y;
      delay(1000);
    }
  } 
}
