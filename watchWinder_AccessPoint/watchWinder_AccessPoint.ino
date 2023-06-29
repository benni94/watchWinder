#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "index_html.h"
#include "network.h"

void setup() {
  Serial.begin(115200);           // this is needed for the serial monitor output
  setupWinderConfig();
  setupNetworkConfig(buttonLED);
}

void loop() {

  buttonPressed = digitalRead(buttonPressPin); // read the current state of the button
  displayCurrentPosition(winder.currentPosition(), buttonPressed); // calculate the current position to display the data

  if (buttonPressed == false) {
    // START
    afterStart = true;
    digitalWrite(buttonLED, HIGH); // LED on

    //winder is moving to position. one round === 4096 increments
    winder.moveTo(incr * winderRounds);
    winder.run();  //Start
  }

  else {
    // STOP
    digitalWrite(buttonLED, LOW); // LED off

    // if the winder is on the end position, reset it
    if (winder.currentPosition() == incr * winderRounds) {
      resetCurrentPosition();
    }

    // otherwise drive end the last round
    else {
      // only if the afterstart is true. otherwise the winder would drive endless
      if (afterStart) {
        int stopOffset = checkOffset(winder.currentPosition());
        // if the current position is smaller than the stopOffset drive to the stop offset
        if (winder.currentPosition() < stopOffset) {
          winder.moveTo(stopOffset);
          winder.run();  //Start
        }
        // if the currentPosition is equal to the stopOffset reset the current position. Otherwise the winder would remember the position for the next start.
        if (winder.currentPosition() == stopOffset) {
          resetCurrentPosition();
        }
      }
    }
  }
}
