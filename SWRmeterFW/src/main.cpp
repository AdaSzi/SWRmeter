#include <Arduino.h>
#include "main.h"
#include "globals.h"
#include "ADC.h"
#include "storage.h"
#include "wifiHandler.h"
#include "webserverHandler.h"


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


    if(!initStorage()){
        setupMode();
    }

    applyLiveSettings();
    initWifi();

    initWebServer();

    Wire.begin();
    swrMeter.initSWRmeter();
}

void loop() {
  swrMeter.handleSWRmeter();

  handleWifi();
  handleWebServer();
  checkButton();
  
  ledBlink(500);
}

