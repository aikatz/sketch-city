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
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

}

void loop() {
  PORTB = B00000000;
  delay(1000);
  PORTB = B00000001;
  delay(1000);
  PORTB = B00000010;
  delay(1000);
  PORTB = B00000011;
  delay(1000);
  
  PORTB = B00000100;
  delay(1000);
  PORTB = B00000101;
  delay(1000);
  PORTB = B00000110;
  delay(1000);
  PORTB = B00000111;
  delay(1000);

  PORTB = B00001000;
  delay(1000);
  PORTB = B00001001;
  delay(1000);
  PORTB = B00001010;
  delay(1000);
  PORTB = B00001011;
  delay(1000);

  PORTB = B00001100;
  delay(1000);
  PORTB = B00001101;
  delay(1000);
  PORTB = B00001110;
  delay(1000);
  PORTB = B00001111;
  delay(1000);

  PORTB = B00010000;
  delay(1000);
  PORTB = B00010001;
  delay(1000);
  PORTB = B00010010;
  delay(1000);
  PORTB = B00010011;
  delay(1000);
}
