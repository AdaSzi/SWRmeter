#include <Arduino.h>
#include "main.h"
#include "globals.h"
#include "ADC.h"


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  digitalWrite(LED_PIN, HIGH);
  delay(250);
  digitalWrite(LED_PIN, LOW);
  delay(250);
  digitalWrite(LED_PIN, HIGH);
  delay(250);
  digitalWrite(LED_PIN, LOW);
  delay(250);

  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("Starting SWR meter");
  #endif

  Wire.begin();


  /*if(!initStorage()){
    setupMode();
  }

  applyLiveSettings();
  initWifi();

  initWebServer();
*/


}

void loop() {
  swrMeter.handleSWRmeter();
  
  Serial.println(globalData.currentSWR);
}

void checkButton() {
  static unsigned long buttonStartTime = 0;
  static bool buttonHeld = false;

  if (!digitalRead(BUTTON_PIN)) {
    if (!buttonHeld) {
      buttonStartTime = millis();
      buttonHeld = true;
    }
    else {
      unsigned long currentTime = millis();
      if (currentTime - buttonStartTime >= BUTTON_HOLD_RESET_TIME) {
        //deleteConfig("/config.json");
        //deleteConfig("/WiFiConfig.json");
        restart();
        buttonHeld = false;
      }
    }
  }
  else buttonHeld = false;
}
