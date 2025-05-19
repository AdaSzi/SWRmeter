#include "globals.h"
#include "SWR.h"
#include <ArduinoJson.h>

GlobalData globalData;
DynamicJsonDocument wifiConfig(8192);
DynamicJsonDocument mainConfigDoc(8192);
char mainConfigDocString[8192];

SWRmeter swrMeter = SWRmeter(ADC_I2C_ADDRESS, &globalData.currentSWR);

void applyLiveSettings() {  
  //rotator.initRotator(mainConfigDoc["settings"]["azOff"], mainConfigDoc["settings"]["rotRange"], mainConfigDoc["settings"]["potMin"], mainConfigDoc["settings"]["potMax"]);
}

void ledBlink(uint16_t period){
  static unsigned long nextLEDUpdate = 0;
  if (millis() > nextLEDUpdate) {
    nextLEDUpdate = millis() + period;
    digitalWrite(LED_PIN, !digitalRead(8));
  }
}

void restart(){
  #ifdef DEBUG
    Serial.println("\nRestarting...");
  #endif
  delay(1000);
  ESP.restart();
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
        deleteConfig("/config.json");
        deleteConfig("/WiFiConfig.json");
        delay(1000);
        restart();
        buttonHeld = false;
      }
    }
  }
  else buttonHeld = false;
}