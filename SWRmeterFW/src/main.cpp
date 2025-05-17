#include <Arduino.h>
#include <Wire.h>
#include "ADC.h"


const uint8_t MCP3426_ADDRESS = 0x68;

ADC adc(MCP3426_ADDRESS);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial);

  delay(5000);
  Serial.println("MCP3426 Dual Channel Reader Test");

  if (adc.initADC()) {
    Serial.println("MCP3426Reader initialized successfully.");
  } else {
    Serial.println("Failed to initialize MCP3426Reader. Halting.");
    Serial.print("Last Error Code: "); Serial.println(adc.getLastError());
    while (1);
  }
}

void loop() {
  adc.handleADC();
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime > 200) { // Print values every 200ms
    lastPrintTime = millis();

    long ch1_val = adc.getValueCh1();
    long ch2_val = adc.getValueCh2();
    MCP342x::error_t err = adc.getLastError();

    Serial.print("CH1: ");
    Serial.print(ch1_val);
    Serial.print(" (Status: ");
    if (adc.getStatusCh1().isReady() && adc.getStatusCh1().getChannel() == MCP342x::channel1) {
      Serial.print("OK, Res:"); Serial.print(adc.getStatusCh1().getResolution());
    } else {
      Serial.print("ERR/NRdy");
    }
    
    Serial.print(") | CH2: ");
    Serial.print(ch2_val);
    Serial.print(" (Status: ");
    if (adc.getStatusCh2().isReady() && adc.getStatusCh2().getChannel() == MCP342x::channel2) {
      Serial.print("OK, Res:"); Serial.print(adc.getStatusCh2().getResolution());
    } else {
      Serial.print("ERR/NRdy");
    }
    Serial.print(")");

    if (err != MCP342x::errorNone && err != MCP342x::errorConversionNotReady) { // Don't flood with 'not ready' if that's handled by status check
        Serial.print(" | Last ADC Op Error: ");
        Serial.print(err);
    }
    Serial.println();
  }
}
