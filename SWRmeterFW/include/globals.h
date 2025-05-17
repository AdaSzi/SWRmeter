#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "SWRmeter.h"

#define LED_PIN 8
#define BUTTON_PIN 9

#define ADC_I2C_ADDRESS 0x68

#define BUTTON_HOLD_RESET_TIME 10000

#define WEBSOCKET_UPDATE_INTERVAL 1000

struct GlobalData {
    float currentSWR = 0;
    bool isMoving = false;

    bool operator == (const GlobalData& other) const {
        return currentSWR == other.currentSWR
               //&& targetAzimuth == other.targetAzimuth
               //&& currentSpeed == other.currentSpeed
               //&& isMoving == other.isMoving
               ;
    }

    bool operator != (const GlobalData& other) const {
        return !(*this == other);
    }
};

extern GlobalData globalData;
extern DynamicJsonDocument wifiConfig;
extern DynamicJsonDocument mainConfigDoc;
extern char mainConfigDocString[8192];

extern SWRmeter swrMeter;

void applyLiveSettings();
void ledBlink(uint16_t period);
void restart();

#endif