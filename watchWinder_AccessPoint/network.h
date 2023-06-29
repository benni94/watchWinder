#include "stepper.h"

// https://randomnerdtutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/
// https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "WatchWinder";
const char* password = "12345678";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

const char* PARAM_INPUT_1 = "input1";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// transform the int into a usable processor data for the html. This could be used with the synt of %DATA% in the html dom
// https://stackoverflow.com/questions/72636991/how-to-upload-a-value-to-espasyncwebserver
String processor(const String & var) {
  if (var == "DATA") {
    // create a JSON object for the initial values
    return "{winderRounds:" + String(winderRounds) + ",loadingState:" + String(loadingState) + "}";
  }
  return String();
}

void setupNetworkConfig(int buttonLED) {



  WiFi.softAP(ssid, password);


  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1; // the param from the id of the input field
      winderRounds = inputMessage.toInt();
      preferences.putUInt("inputValue1", inputMessage.toInt()); // store the variable inputMessage in "file" inputValue1 permanently
      request->redirect("/"); // go back to parent page after redirect to input...number
    }
  });

  // add the event handler for the /events
  server.addHandler(&events);
  // shows a not found page
  server.onNotFound(notFound);
  server.begin();
}

// Timer variables
// this should be used insted of delay because avoids break in code
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

void sendLoadingState(int loadingState) {
  if ((millis() - lastTime) > timerDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(String(loadingState).c_str(), "loadingState", millis());
    lastTime = millis();
  }
}

// displays the current position of the winder
// also sends the loading state to the ui
// https://randomnerdtutorials.com/esp32-web-server-sent-events-sse/#:~:text=JavaScript%20%E2%80%93%20EventSource
void displayCurrentPosition(int currentPos, boolean pressed) {
  if (pressed == false) {
    // for the first round. otherwise, the loading state will received the current position after the first round
    if (currentPos < incr) {
      loadingState = 1;
    }
    else {
      loadingState = ( currentPos / incr) * (100.00 / winderRounds);
    }
  }
  else {
    loadingState = 0;
  }
  sendLoadingState(loadingState); // send the current loading state to the ui
}
