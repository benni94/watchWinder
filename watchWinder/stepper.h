#include <AccelStepper.h>
#include <Preferences.h>
Preferences preferences;

//# WINDER Define the steps for the winder
// AccelStepper class references: https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a8b76f1ae6a49d2c2887b9fea30b8d84f
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1 D5 // IN1 on the ULN2003 driver 1
#define motorPin2 D6 // IN2 on the ULN2003 driver 1
#define motorPin3 D7 // IN3 on the ULN2003 driver 1
#define motorPin4 D8 // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper winder(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

int buttonPressPin = D3; // button input pin
int buttonLED = D4;      // button led pin
bool buttonPressed;      // Define a Boolean to track a button press
int winderRounds = 2;    // Define the rounds for the winder.
int loadingState = 0;    // the current state of the winder in percent

boolean afterStart = false; // needed, because otherwise it is hard to find out if the winder has started or not
const int incr = 4096; // the increments of the winder


// checks the offset with the current position of the winder.
// if the winder is not finished with his round, the current position with a extra round gets returned
int checkOffset(int currentPosition) {
  // the plus two is needed, because the distance to the boarder value is needed for the extra round
  int checkCurrentPosition = currentPosition + 2;
  int checkCurrentPositionWithOffset = ((currentPosition / incr) + 1 ) * incr;
  if (checkCurrentPosition > checkCurrentPositionWithOffset) {
    return currentPosition;
  }
  return checkCurrentPositionWithOffset;
}

// reset the current position to handle a new start if the winder is finsished with incrementing
void resetCurrentPosition() {
  winder.stop(); //Stop
  winder.setCurrentPosition(0);
  afterStart = false;
}

void setupWinderConfig() {

  pinMode(buttonLED, OUTPUT);     // defines the button led pin mode output
  winder.setMaxSpeed(1000.0);     // max speed for the winder
  winder.setAcceleration(100.0);  // Make the acc quick
  winder.setSpeed(200);           // sets the speed for the winder

  // store data permanent in flash storrage of esp32 (arduino has eeprom)
  preferences.begin("inputValues", false);                 // creates or uses the folder "inputValues"
  // preferences.clear();                                  //  clear all folders with "inputValue"
  // preferences.remove("inputValue1");                    // clear file "inputValue1" from "inputValues"
  winderRounds = preferences.getUInt("inputValue1", 2);    // read file "inputValue1" from "inputValues" and write it in winderRounds, 2 is the default
  // preferences.end();                                    // end the preference, not sure for what this is
}
