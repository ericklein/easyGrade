/*
  Project Name:   easyGrade box controller
  Developer:      Eric Klein Jr. (temp2@ericklein.com)
  Description:    Control analog LED strips in the easyGrade card capture box

  See README.md for target information, revision history, feature requests, etc.
*/

// hardware and internet configuration parameters
#include "config.h"

// LED strip globals
uint16_t ledStripBrightness = ledStripMinBrightness; // uint16_t to prevent compiler generated wraparound at high end of range
enum ledStripCurrentPin ledActiveStrip = ledNone;

// Rotary encoder
// #include "RotaryEncoder.h" // nrf52 specific library
#include <Encoder.h>
// generic rotary encoder support
Encoder rotaryEncoderOne(rotaryEncoderPin1, rotaryEncoderPin2);
int16_t encoderPosition = 0; // int16_t in case user rotates one direction many times

// button support
#include <ezButton.h>
ezButton buttonOne(buttonPin);
// ezButton buttonOne(buttonPin,INPUT_PULLDOWN);

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    // wait for serial port connection
    while (!Serial)
      delay(10);
    debugMessage("easyGrade box controller started",1);
  #endif

  // setup LED strips
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
  // pinMode(ledTop, OUTPUT);
  // pinMode(ledBottom, OUTPUT);

  // Setup button
  buttonOne.setDebounceTime(buttonDebounceDelay);

  // setup nrf52 encoder
  // RotaryEncoder.begin(rotaryEncoderPin1, rotaryEncoderPin2);
  // RotaryEncoder.start();
}

void loop() {
  buttonOne.loop();
  // check if buttons were pressed
  if (buttonOne.isReleased()) {
    debugMessage("button short press",2);
    // changeLEDBank();
    cycleLEDBanks(5000);
  }
  resolveRotaryEncoder();
}

void resolveRotaryEncoder()
// maps user input changes from rotary encoder into a brightness level for the current LED strip(s)
{
  int16_t newEncoderPosition;
  newEncoderPosition = rotaryEncoderOne.read() >> 1;

  if (newEncoderPosition != encoderPosition) 
  {
    debugMessage(String("encoder previous = ") + encoderPosition + ", new = " + newEncoderPosition,2);
    debugMessage(String("previous brightness = ") + ledStripBrightness,2);
    ledStripBrightness = ledStripBrightness + ((newEncoderPosition - encoderPosition) * ledStripBrightnessStepChange);
    ledStripBrightness = constrain(ledStripBrightness, ledStripMinBrightness, ledStripMaxBrightness);
    debugMessage(String("new brightness = ") + ledStripBrightness,2);
    if (ledActiveStrip != ledNone) {
      if (ledActiveStrip != ledAll)
      analogWrite(ledActiveStrip, ledStripBrightness);
      else {
        analogWrite(ledLeft, ledStripBrightness);
        analogWrite(ledRight, ledStripBrightness);
        analogWrite(ledTop,ledStripBrightness);
        analogWrite(ledBottom,ledStripBrightness);
      }
    }
    encoderPosition = newEncoderPosition;
  }
}

// nrf52 encoder
// void resolveRotaryEncoder()
// // maps user input changes from rotary encoder into a brightness level for the current LED strip(s)
// {
//   uint8_t newEncoderPosition = RotaryEncoder.read();

//   if (newEncoderPosition){
//     if (newEncoderPosition > 1) {
//       ledStripBrightness += ledStripBrightnessStepChange;
//     }
//     else {
//       ledStripBrightness -=ledStripBrightnessStepChange;
//     }
//     ledStripBrightness = constrain(ledStripBrightness, ledStripMinBrightness, ledStripMaxBrightness);
//     debugMessage(String("new brightness value: ") + ledStripBrightness,1);
//     if (ledActiveStrip != ledNone) {
//       if (ledActiveStrip != ledAll)
//       analogWrite(ledActiveStrip, ledStripBrightness);
//       else {
//         analogWrite(ledLeft, ledStripBrightness);
//         analogWrite(ledTop,ledStripBrightness);
//         analogWrite(ledRight, ledStripBrightness);
//       }
//     }
//   }
// }

void changeLEDBank()
{
  switch (ledActiveStrip) 
  {
    case ledNone:
      // turn on LED strip left of card
      analogWrite(ledLeft,ledStripBrightness);
      ledActiveStrip = ledLeft;
      debugMessage(String("LED strip left of card only at ") + ledStripBrightness + " brightness" ,1);
      break;
    case ledLeft:
      // switch from left to LED strip right of card
      analogWrite(ledLeft, 0);
      analogWrite(ledRight, ledStripBrightness);
      ledActiveStrip = ledRight;
      debugMessage(String("LED strip right of card only at ") + ledStripBrightness + " brightness",1);
      break;
    case ledRight:
      // switch from right to LED strip top of card
      analogWrite(ledRight, 0);
      analogWrite(ledTop, ledStripBrightness);
      ledActiveStrip = ledTop;
      debugMessage(String("LED strip top of card only at ") + ledStripBrightness + " brightness",1);
      break;
    case ledTop:
      // switch from top to LED strip bottom of card
      analogWrite(ledTop, 0);
      analogWrite(ledBottom, ledStripBrightness);
      ledActiveStrip = ledBottom;
      debugMessage(String("LED strip bottom of card only at ") + ledStripBrightness + " brightness",1);
      break;
    case ledBottom:
      // turn on all LED strips
      analogWrite(ledLeft, ledStripBrightness);
      analogWrite(ledRight, ledStripBrightness);
      analogWrite(ledTop, ledStripBrightness);
      analogWrite(ledBottom, ledStripBrightness);
      ledActiveStrip = ledAll;
      debugMessage(String("All LED strips at ") + ledStripBrightness + " brightness",1);
      break;      
    case ledAll:
      // turn off all LED strips
      analogWrite(ledLeft, 0);
      analogWrite(ledRight, 0);
      analogWrite(ledTop, 0);
      analogWrite(ledBottom, 0);
      ledActiveStrip = ledNone;
      debugMessage("All LED strips are off",1);
      break;
  }
}

void cycleLEDBanks(uint16_t interval)
// cycle all LED banks at a fixed interval
{
  // ledActiveStrip = ledNone;
  do 
  {
    changeLEDBank();
    delay(interval);
  }
  while (ledActiveStrip < ledAll);
  changeLEDBank();
}

void debugMessage(String messageText, int messageLevel)
// wraps Serial.println as #define conditional
{
  #ifdef DEBUG
    if (messageLevel <= DEBUG) {
      Serial.println(messageText);
      Serial.flush();      // Make sure the message gets output (before any sleeping...)
    }
  #endif
}