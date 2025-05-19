#include "SWRmeter.h"

SWRmeter::SWRmeter(uint8_t adcI2Caddr, float* swrOutput):
    adc(adcI2Caddr),
    swr(&adcForward, &adcReflected, swrOutput) {
    
    #ifdef DEBUG
        Serial.println("Starting ADC...");
    #endif
    if (adc.initADC()) {
        #ifdef DEBUG
            Serial.println("ADC initialized successfully.");
        #endif
    } else {
        #ifdef DEBUG
            Serial.println("Failed to initialize ADC. Last Error Code: "); Serial.println(adc.getLastError());
        #endif
    }
}

void SWRmeter::initSWRmeter(){
    adc.initADC();
}

void SWRmeter::handleSWRmeter(){
    adc.handleADC();

    if(adc.getNewDataFlag()){
        adc.resetNewDataFlag();
        adcForward = adc.getValueCh1();
        adcReflected = adc.getValueCh2();
        swr.calculateSWR();
    }
}