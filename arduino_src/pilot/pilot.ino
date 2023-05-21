/*
 * SimpleSender.cpp
 *
 *  Demonstrates sending IR codes in standard format with address and command
 *  An extended example for sending can be found as SendDemo.
 *
 *  Copyright (C) 2020-2022  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 *  MIT License
 */
#include <Arduino.h>

#define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

/*
 * This include defines the actual pin number for pins like IR_RECEIVE_PIN, IR_SEND_PIN for many different boards and architectures
 */
#include "pindef.h"
#include <IRremote.hpp> // include the library

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(IR_SEND_PIN, OUTPUT);

    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);

    /*
     * The IR library setup. That's all!
     */
//    IrSender.begin(); // Start with IR_SEND_PIN as send pin and if NO_LED_FEEDBACK_CODE is NOT defined, enable feedback LED at default feedback LED pin
    IrSender.begin(DISABLE_LED_FEEDBACK); // Start with IR_SEND_PIN as send pin and disable feedback LED at default feedback LED pin
}

/*
 * Set up the data to be sent.
 * For most protocols, the data is build up with a constant 8 (or 16 byte) address
 * and a variable 8 bit command.
 * There are exceptions like Sony and Denon, which have 5 bit address.
 */
uint8_t sCommand = 0x34;
uint8_t sRepeats = 0;

const int BUFFER_SIZE = 16;
uint8_t buf[BUFFER_SIZE];
bool toggle = true;

void loop() {
  char command[BUFFER_SIZE];
  int rlen = 0;

  toggle = !toggle;
  memset(command, 0, BUFFER_SIZE);
  memset(buf, 0, BUFFER_SIZE);
  if (Serial.available() > 0) {
    // read the incoming bytes:
    
    rlen = Serial.readBytesUntil('\n', buf, BUFFER_SIZE);

    // prints the received data
    Serial.print("I received: ");
    for(int i = 0; i < rlen; i++) {
      Serial.print(buf[i]);
      command[i] = buf[i];
    }
  } else {
    rlen = 0;
  }
    
    
    // digitalWrite(IR_SEND_PIN, toggle ? HIGH : LOW);


    // Receiver output for the first loop must be: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
    if(rlen) {
      IrSender.sendNEC(0x4, 0x8, sRepeats); 
      Serial.print("sent ");
      Serial.print(rlen);
      Serial.print(" bytes");   
      Serial.print(F("Send now: address=0x00, command=0x"));
    for(int i = 0; i < rlen; i++) {
      Serial.print(command[i], HEX);
    }
      Serial.println();
      Serial.flush();
    }
    /*
     * Increment send values
     */
    //sCommand += 0x11;
    //sRepeats++;
    // clip repeats at 4
    //if (sRepeats > 4) {
    //    sRepeats = 4;
    //}

    delay(1000);  // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
}