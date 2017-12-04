/*
fft_adc.pde
 guest openmusiclabs.com 8.18.12
 example sketch for testing the fft library.
 it takes in data on ADC0 (Analog0) and processes them
 with the fft. the data is sent out over the serial
 port at 115.2kb.  there is a pure data patch for
 visualizing the data.
 */

// 9615 Hz

// do #defines BEFORE #includes
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

// Moving average variables
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A0;

// ------------------------------------------------------------------------------

// function that prints whether the microphone detects the tone

void detectingTone(int value){
  if(value > 8000){
      Serial.println("Yes");
    }
    else{
      Serial.println("No");
    }
}

void setup() {
  Serial.begin(9600); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter - delay() and millis() killed
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0

  // Moving average algorithm
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  
}

void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf7; // restart adc (128 prescalar)
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    // window data, then reorder, then run, then take output
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei(); // turn interrupts back on
    //Serial.write(255); // send a start byte

    // Moving average algorithm --------------------------------------
    
    // subtract the last reading:
    total = total - readings[readIndex];
    
    // read from the sensor:
    readings[readIndex] = fft_log_out[18];
    
    // add the reading to the total:
    total = total + readings[readIndex];
    
    // advance to the next position in the array:
    readIndex = readIndex + 1;
  
    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }
  
    // calculate the average:
    average = (int)(total / numReadings);

    // Digital Filter Algorithm
    average = (average - 50) * 100;
    
    // Producing average and tone detector output
    Serial.print("Average: ");
    Serial.print(average);
    Serial.print(" | Detectable Tone: ");

    detectingTone(average);
    
  }
}

