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

int inputPin = A1;
int led1 = 5;
int led2 = 6;

// ------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter - delay() and millis() killed
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0

 
  //debugging LEDs
  pinMode(led1, OUTPUT);   
  pinMode(led2, OUTPUT);  
  
}

void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
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

    // Filtering for high magnitude in bin 47 and bin 80
    bool top1 = (fft_log_out[47] - fft_log_out[44]) > 20;
    bool bottom1 = (fft_log_out[47] - fft_log_out[49]) > 20;

    bool top2 = (fft_log_out[80] - fft_log_out[78]) > 20;
    bool bottom2 = (fft_log_out[80] - fft_log_out[82]) > 20;


///detecting LEDs
    if (top1 && bottom1){ // 7khz
      digitalWrite(led1, HIGH);}   // turn the LED on (HIGH is the voltage level)
    else{
      digitalWrite(led1, LOW);}
    
    if (top2 && bottom2){  // 12khz
      digitalWrite(led2, HIGH);}   // turn the LED on (HIGH is the voltage level)
    else{
      digitalWrite(led2, LOW);}

//      for (int i = 0; i<128; i++){
//        Serial.print(i);
//        Serial.print(", ");
//        Serial.println(fft_log_out[i]);
//      }

//    Serial.print("  45:   ");
//    Serial.print(fft_log_out[45]);
//    Serial.print("  46:   ");
//    Serial.print(fft_log_out[46]);
//    Serial.print("  47:   ");
//    Serial.print(fft_log_out[47]);
//    Serial.print("  48:   ");
//    Serial.print(fft_log_out[48]);
//    Serial.print("        78:   ");
//    Serial.print(fft_log_out[78]);
//    Serial.print("  80:   ");
//    Serial.print(fft_log_out[80]);
//    Serial.print("  82:   ");
//    Serial.print(fft_log_out[82]);
//    Serial.print(" Top: ");
//    Serial.print(top1, DEC);
//    Serial.print(" Bottom: ");
//    Serial.print(bottom1, DEC);
//    Serial.println("END");

    
  }
}

