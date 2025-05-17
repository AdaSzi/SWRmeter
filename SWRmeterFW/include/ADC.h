#ifndef ADC_H
#define ADC_H

#include <Wire.h>
#include "MCP342x.h" // Your MCP342x library header

class ADC {
public:
    ADC(uint8_t i2cAddress);

    bool initADC();      // Initializes the ADC and prepares for the first read
    void handleADC();    // Reads both channels if the update interval has passed

    long getValueCh1() const;
    long getValueCh2() const;
    MCP342x::Config getStatusCh1() const; // Optional: to get full status if needed
    MCP342x::Config getStatusCh2() const; // Optional: to get full status if needed
    MCP342x::error_t getLastError() const;

private:
    MCP342x adcDevice; // The MCP342x library object

    // Fixed configuration settings for this specific use-case
    const MCP342x::Mode adcMode = MCP342x::continuous;
    const MCP342x::Resolution adcResolution = MCP342x::resolution14; // Set to 14-bit
    const MCP342x::Gain adcGain = MCP342x::gain1;

    long rawValueCh1;
    long rawValueCh2;
    MCP342x::Config currentStatusCh1;
    MCP342x::Config currentStatusCh2;
    MCP342x::error_t lastOperationError;

    unsigned long conversionTimePerChannelUs;
    unsigned long lastUpdateTimeMs;
    unsigned long updateIntervalMs; // Time between full dual-channel read cycles
};

#endif // ADC_H