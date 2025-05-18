/*
  Project Name:   lightbox analog
  Developer:      Eric Klein Jr. (temp2@ericklein.com)
  Description:    Control analog LED strips in the easyGrade card capture box

  See README.md for target information, revision history, feature requests, etc.
*/

// Configuration Step 1: Set debug message output
// comment out to turn off; 1 = summary, 2 = verbose
#define DEBUG 2

// Configuration variables that are less likely to require changes

// Hardware configuration

// LED strips
const uint8_t ledStripMinBrightness = 252;
const uint8_t ledStripMaxBrightness = 252; 
const uint8_t ledStripBrightnessStepChange = 4;
const uint8_t ledStripCycleTime = 5; // seconds
// Pins associated with ledActiveStrip
// enum ledStripCurrentPin {
//   ledNone = 0,
//   ledLeft = 27,
//   ledRight = 30,
//   ledFront = 11,
//   // ledBack = ,
//   ledAll = 255
// };

enum ledStripCurrentPin {
  ledNone = 0,
  ledLeft = 8,
  ledRight = 9,
  // ledFront = 10,
  ledBack = 95,
  ledAll = 255
};

// rotary encoder
// #define rotaryEncoderPin1 A0  // counter-clockwise, used to change brighness
// #define rotaryEncoderPin2 A1  // clockwise, used to change brightness
#define rotaryEncoderPin1 11  // counter-clockwise, used to change brighness
#define rotaryEncoderPin2 12  // clockwise, used to change brightness

// button
// const uint8_t buttonPin = 7;
const uint8_t buttonPin = 13;
const uint16_t buttonDebounceDelay = 50; // time in milliseconds to debounce button
