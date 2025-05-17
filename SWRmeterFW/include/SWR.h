/*
 *
 *
 *    SWR.h
 *
 *    SWR calculator.
 *
 *    License: GNU General Public License Version 3.0.
 *
 *    Copyright (C) 2014-2016 by Matthew K. Roberts, KK5JY. All rights reserved.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see: http://www.gnu.org/licenses/
 *
 *
 */

#ifndef SWR_H
#define SWR_H

#define USE_VOLTAGE_CALC    //use a voltage calculation (sensor is a voltage sensor, not a power sensor)

#include <Arduino.h>

class SWR {
    private:
        float* SWRvalue;
        long* SWRforwardADCInput;
        long* SWRreflectedADCInput;
        
        uint16_t forward, reflected;
        uint16_t WindowPWR, WindowSWR;
        uint16_t MinPower;
        float MaxSWR;

        float ScaleFWD, ScaleREF; //scale factors
        float AlphaFwd, AlphaRef; //smoothing constant (exponential smoother)

    public:
        SWR(long* ADCforward, long* ADCreflected, float* SWRoutput);

        void calculateSWR();

        float getAlphaForward() const { return AlphaFwd; }

        void setAlphaForward(float newValue) {
            if (newValue > 1.0)
                newValue = 1.0;
            if (newValue < 0.01)
                newValue = 0.01;
            AlphaFwd = newValue;
        }

        float AlphaReflected() const {
            return AlphaRef;
        }
        void setAlphaReflected(float newValue)
        {
            if (newValue > 1.0)
                newValue = 1.0;
            if (newValue < 0.01)
                newValue = 0.01;
            AlphaRef = newValue;
        }

        //get the maximum SWR value permitted
        float getMaxSWR() const { return MaxSWR; }
        void setMaxSWR(float value) { if (value >= 2) MaxSWR = value; }

        //query raw power A/D readings
        uint16_t getForwardRaw() const { return forward; }
        uint16_t getReflectedRaw() const { return reflected; }

        //query power levels (scaled voltage readings, not actual power in watts unless calibrated)
        float getForward() const { return forward * ScaleFWD; }
        float getReflected() const { return reflected * ScaleREF; }

        //get scaling factors
        float getScaleForward() const { return ScaleFWD; }
        float getScaleReflected() const { return ScaleREF; }

        //set scaling factors
        void setScaleForward(float value) { ScaleFWD = value; }
        void setScaleReflected(float value) { ScaleREF = value; }

        //query the minimum power value (raw A/D reading threshold for SWR calculation)
        uint16_t getMinPower() const { return MinPower; }
        void setMinPower(uint16_t val) { MinPower = val; }
};
#endif //SWR_H