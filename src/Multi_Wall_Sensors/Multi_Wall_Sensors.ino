/* This minimal example shows how to get single-shot range
measurements from the VL6180X.

The range readings are in units of mm. */

#include <Wire.h>
#include <VL6180X.h>

VL6180X sensor1;
VL6180X sensor2;
VL6180X sensor3;
int enable1 = 42;
int enable2 = 44;
int enable3 = 46;

void setup() ///lots of setup, but while loop is simple
{
  pinMode(enable1,OUTPUT);
  pinMode(enable2,OUTPUT);
  pinMode(enable3,OUTPUT);
  Serial.begin(9600);
  Wire.begin();

  
  digitalWrite(enable1,HIGH);//turn on sensor1
  digitalWrite(enable2,LOW);//turn off sensor2
  digitalWrite(enable2,LOW);//turn off sensor3
  sensor1.init();
  sensor1.configureDefault();
  sensor1.setTimeout(500);
  delay(500);
  sensor1.setAddress(0b1111011);//change I2C address

  digitalWrite(enable2,HIGH); //turn on sensor2
  sensor2.init();
  sensor2.configureDefault();
  sensor2.setTimeout(500);
  delay(500);
  sensor2.setAddress(0b1011011); //change I2C address

  digitalWrite(enable3,HIGH); //turn on sensor3
  sensor3.init();
  sensor3.configureDefault();
  sensor3.setTimeout(500);
  delay(500);

  sensor1.setScaling(1);
  sensor2.setScaling(1);
  sensor3.setScaling(1);
  
  //set convergence time and integration values for all sensors
  sensor1.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  sensor1.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  sensor2.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  sensor2.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  sensor3.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  sensor3.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  
  //stop continous readings
  sensor1.stopContinuous();
  sensor2.stopContinuous();
  sensor3.stopContinuous();
  delay(300);
  
  //start continous ranging for all sensors
  sensor1.startInterleavedContinuous(100);
  sensor2.startInterleavedContinuous(100);
  sensor3.startInterleavedContinuous(100);
}

void loop() 
{ 
  Serial.print("\tRange: ");
  Serial.print(sensor1.readRangeContinuousMillimeters());
  Serial.print("\tSensor 2: ");
  Serial.print(sensor2.readRangeContinuousMillimeters());
  Serial.print("\tSensor 3: ");
  Serial.print(sensor3.readRangeContinuousMillimeters());
  Serial.println();
  Serial.print(analogRead(A3));
}
