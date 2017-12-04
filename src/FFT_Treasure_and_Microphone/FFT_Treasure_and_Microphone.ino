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
int led1 = 5; //red
int led2 = 6; //green
int led3 = 3; //blue

int gain = 28;
int thresh = 68;
int sound_thresh = 188;

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
  pinMode(led3, OUTPUT);   
  
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

// Filtering for high magnitude in bin 47, 80, and 114


///detecting LEDs
    if (fft_log_out[47] > thresh){ // 7khz
      digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
      Serial.println("GOT 7");}
    else{
      digitalWrite(led1, LOW);}
    
    if (fft_log_out[80] > thresh){  // 12khz
      digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
      Serial.println("GOT 12");}
      
    else{
      digitalWrite(led2, LOW);}

    if (fft_log_out[114] > thresh){  // 17khz
      Serial.println("GOT 17");
      digitalWrite(led3, HIGH);}   // turn the LED on (HIGH is the voltage level)
    else{
      digitalWrite(led3, LOW);}

 //tone detection
  if (fft_log_out[4] > sound_thresh){ // 660Hz Tone
      Serial.println("Tone Detected");}

////excel output code
//      for (int i = 0; i<128; i++){
//        Serial.print(i);
//        Serial.print(", ");
//        Serial.println(fft_log_out[i]);
//        Serial.print(", ");
//      }
//
//    Serial.print("  110:   ");
//    Serial.print(fft_log_out[110]);
//    Serial.print("  111:   ");
//    Serial.print(fft_log_out[111]);
//    Serial.print("  112:   ");
//    Serial.print(fft_log_out[112]);
//    Serial.print("  113:   ");
//    Serial.print(fft_log_out[113]);
//    Serial.print("  114:   ");
//    Serial.print(fft_log_out[114]);
//    Serial.print("  115:   ");
//    Serial.print(fft_log_out[115]);
//    Serial.print("  116:   ");
//    Serial.print(fft_log_out[116]);
//    Serial.print(" Top: ");
//    Serial.print(top17, DEC);
//    Serial.print(" Bottom: ");
//    Serial.print(bottom17, DEC);
//    Serial.println("END");

    
  }
}

