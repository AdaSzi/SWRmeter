#ifndef SWRMETER_H
#define SWRMETER_H
#include "ADC.h"
#include "SWR.h"

class SWRmeter{
    public:
        SWRmeter(uint8_t adcI2Caddr, float* swrOutput);
        void initSWRmeter();
        void handleSWRmeter();


    private:
        ADC adc;
        SWR swr;

        long adcForward, adcReflected;

};
#endif
