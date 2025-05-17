#include "ADC.h"
#include "Arduino.h"
#include "globals.h"

ADC::ADC(uint8_t i2cAddress) :
    adcDevice(i2cAddress), //Initialize the MCP342x object with the given address
    rawValueCh1(0),
    rawValueCh2(0),
    lastOperationError(MCP342x::errorNone),
    conversionTimePerChannelUs(0),
    lastUpdateTimeMs(0),
    updateIntervalMs(50) { //Default update interval, will be refined in initReader
    //Constructor
}

bool ADC::initADC() {
    //Wire.begin() should be called in main setup()

    conversionTimePerChannelUs = adcResolution.getConversionTime();

    updateIntervalMs = (2 * conversionTimePerChannelUs / 1000) + 10;
    if (updateIntervalMs < 1) {
        updateIntervalMs = 1;
    }

    #ifdef DEBUG //Changed Debug define name
        Serial.print("ADC: Initializing. Conversion time per channel: ");
        Serial.print(conversionTimePerChannelUs / 1000.0, 2);
        Serial.println(" ms");
        Serial.print("ADC: Update interval set to: ");
        Serial.print(updateIntervalMs);
        Serial.println(" ms");
    #endif

    MCP342x::Config initialCfgCh1(MCP342x::channel1, adcMode, adcResolution, adcGain);
    lastOperationError = adcDevice.configure(initialCfgCh1);

    if (lastOperationError != MCP342x::errorNone) {
        #ifdef DEBUG
            Serial.print("ADC: ERROR during initial CH1 config. Code: ");
            Serial.println(lastOperationError);
        #endif
        return false;
    }

    delayMicroseconds(conversionTimePerChannelUs + 2000); 

    #ifdef DEBUG
        Serial.println("ADC: Initialization complete. CH1 configured and first conversion awaited.");
    #endif
    
    lastUpdateTimeMs = millis();
    return true;
}

void ADC::handleADC() {
    if (millis() - lastUpdateTimeMs < updateIntervalMs) {
        return; 
    }
    lastUpdateTimeMs = millis();

    MCP342x::error_t currentOpError;

    //--- 1. Read Channel 1 ---
    currentOpError = adcDevice.read(rawValueCh1, currentStatusCh1);
    if (currentOpError != MCP342x::errorNone) {
        lastOperationError = currentOpError;

        #ifdef DEBUG
            Serial.print("ADC: Error reading CH1: "); Serial.println(lastOperationError);
        #endif

        MCP342x::Config recoveryCfgCh1(MCP342x::channel1, adcMode, adcResolution, adcGain);
        adcDevice.configure(recoveryCfgCh1);
        return;
    }
    if (!currentStatusCh1.isReady() || currentStatusCh1.getChannel() != MCP342x::channel1) {
        lastOperationError = MCP342x::errorConversionNotReady; 

        #ifdef DEBUG
            Serial.println("ADC: CH1 data not ready or channel mismatch.");
        #endif
    }

    //--- 2. Configure ADC for Channel 2 ---
    MCP342x::Config cfgCh2(MCP342x::channel2, adcMode, adcResolution, adcGain);
    currentOpError = adcDevice.configure(cfgCh2);
    if (currentOpError != MCP342x::errorNone) {
        lastOperationError = currentOpError;

        #ifdef DEBUG
            Serial.print("ADC: Error configuring for CH2: "); Serial.println(lastOperationError);
        #endif

        MCP342x::Config recoveryCfgCh1(MCP342x::channel1, adcMode, adcResolution, adcGain);
        adcDevice.configure(recoveryCfgCh1); //Attempt to recover
        return;
    }
    
    delayMicroseconds(conversionTimePerChannelUs);

    //--- 3. Read Channel 2 ---
    currentOpError = adcDevice.read(rawValueCh2, currentStatusCh2);
    if (currentOpError != MCP342x::errorNone) {
        lastOperationError = currentOpError;

        #ifdef DEBUG
            Serial.print("ADC: Error reading CH2: "); Serial.println(lastOperationError);
        #endif
    }
     if (!currentStatusCh2.isReady() || currentStatusCh2.getChannel() != MCP342x::channel2) {
         if (currentOpError == MCP342x::errorNone) { 
            lastOperationError = MCP342x::errorConversionNotReady;
        }

        #ifdef DEBUG
            Serial.println("ADC: CH2 data not ready or channel mismatch.");
        #endif
    }

    //--- 4. Configure ADC back to Channel 1 for the next reading cycle ---
    MCP342x::Config cfgCh1Next(MCP342x::channel1, adcMode, adcResolution, adcGain);
    currentOpError = adcDevice.configure(cfgCh1Next);
    if (currentOpError != MCP342x::errorNone) {
        lastOperationError = currentOpError;

        #ifdef DEBUG
            Serial.print("ADC: Error configuring for CH1 (next cycle): "); Serial.println(lastOperationError);
        #endif
    }

    newDataFlag = true;
}

long ADC::getValueCh1() const {
    return rawValueCh1;
}

long ADC::getValueCh2() const {
    return rawValueCh2;
}

MCP342x::Config ADC::getStatusCh1() const {
    return currentStatusCh1;
}

MCP342x::Config ADC::getStatusCh2() const {
    return currentStatusCh2;
}

MCP342x::error_t ADC::getLastError() const {
    return lastOperationError;
}