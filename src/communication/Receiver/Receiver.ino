#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */


/**
 * Example for Getting Started with nRF24L01+ radios.
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting
 * with the serial monitor and sending a 'T'.  The ping node sends the current
 * time to the pong node, which responds by sending the value back.  The ping
 * node can then see how long the whole cycle took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

byte position_load;
byte data_load;
unsigned char got_treasure[4][6];
unsigned char got_wall[4][6];
unsigned char got_done[4][6];
unsigned char data_array[4][6];
unsigned char position_array[4][6];


//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x000000001ELL, 0x000000001FLL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;

void setup(void)
{
  //
  // Print preamble
  // Serial communication
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}

void loop(void)
{
  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  if ( role == role_pong_back )
  {
    
    volatile unsigned short got_data;
    bool done = false;
    unsigned char x_coord;
    unsigned short y_coord;
    unsigned char treasure;
    unsigned short wall;
    unsigned char done_signal;  

    

     while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      done = radio.read( &got_data, sizeof(unsigned short) );
      // Interpret new data
      x_coord= (got_data & 0xC000) >> 14; //2 bits x coordinate data
      y_coord= (got_data & 0x3800) >> 11; //3 bits y coordinate data
      y_coord= y_coord-1;
      treasure= (got_data & 0x0600) >> 9; //2 bits treasure data
      wall= (got_data & 0x01E0 ) >> 5; //4 bits wall data
      done_signal= (got_data & 0x0010) >> 4; //1 bit done signal
  
      position_load= y_coord << 4 | x_coord << 2; 
      data_load= wall << 4 | treasure << 2;

      printf("%02X ", position_load);
      printf("%02X\n", data_load);
       if (done_signal == 1){  
        digitalWrite(8, HIGH);
        PORTD = B11111100;
       } else {
        digitalWrite(8, LOW);
       PORTD = position_load;
       delay(500);  
       digitalWrite(8, HIGH);
       PORTD = data_load;
       delay(500);
       }
      
      got_treasure[x_coord][y_coord] = treasure;
      got_wall[x_coord][y_coord] = wall;
      got_done[x_coord][y_coord] = done_signal;

      position_array[x_coord][y_coord] = position_load;
      data_array[x_coord][y_coord] = data_load;
      
      // Delay just a little bit to let the other unit
      // make the transition to receiver
      delay(20);
    
    }
    
    // if there is data ready
    if ( radio.available() )
    {
      // Print the treasure
     // for (int i=0; i < 5; i++) {
        //for (int j=0; j < 4; j++) {
          //printf("%02X ", position_array[j][i]);
          //printf("%02X ", data_array[j][i]);
          //printf("%d ", got_treasure[j][i]);
          //printf("%d ", got_wall[j][i]);
          //printf("%d ", got_done[j][i]);
          //printf("|");
       // }
        //printf("\n");
      //}

      //communicate to FPGA

       
      
      //printf("position: %02X, data: %02X\n", position_load, data_load);

      // Delay just a little bit to let the other unit
      // make the transition to receiver
      delay(20);
    

    printf("\n");
      // First, stop listening so we can talk
      /*radio.stopListening();

      // Send the final one back.
      radio.write( &got_time, sizeof(unsigned long) );
      printf("Sent response.\n\r");

      // Now, resume listening so we catch the next packets.
      radio.startListening();*/
    }
  }

  //
  // Change roles
  //

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      // Become the primary transmitter (ping out)
      role = role_ping_out;
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]);
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");

      // Become the primary receiver (pong back)
      role = role_pong_back;
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
    }
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
