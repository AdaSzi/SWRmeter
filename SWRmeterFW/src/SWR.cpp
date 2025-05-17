/*
 *
 *
 *    SWR.cpp
 *
 *    SWR calculator - Implementation.
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

#include "SWR.h"

SWR::SWR(long* ADCforward, long* ADCreflected, float* SWRoutput) {
    this->SWRvalue = SWRoutput;
    this->SWRforwardADCInput = ADCforward;
    this->SWRreflectedADCInput = ADCreflected;

    this->forward = 0; // Initialize raw readings
    this->reflected = 0; // Initialize raw readings
    this->MaxSWR = 25.0;
    this->MinPower = 0;
    this->ScaleFWD = 1.0;
    this->ScaleREF = 1.0;
    this->AlphaFwd = 0.5;
    this->AlphaRef = 0.5;
}

void SWR::calculateSWR() {
    //read voltages and apply exponential moving average
    forward = (uint16_t)((AlphaFwd * (*SWRforwardADCInput)) + ((1.0 - AlphaFwd) * forward));
    reflected = (uint16_t)((AlphaRef * (*SWRreflectedADCInput)) + ((1.0 - AlphaRef) * reflected));
    
    // compute SWR
    float wf; // Working float for SWR calculation
    if (reflected == 0 || forward < MinPower) {
        // If no reflected power or forward power is below minimum threshold, SWR is 1.0
        wf = 1.0;
    } else if (reflected >= forward) {
        // If reflected is greater than or equal to forward, SWR is at maximum (infinite or capped)
        wf = MaxSWR;
    } else {
        #ifdef USE_VOLTAGE_CALC
            // SWR = (Vfwd + Vref) / (Vfwd - Vref)
            wf = (float)(forward + reflected) / (float)(forward - reflected);
        #else
            // Or if Forward and Reflected were true power readings (which they aren't without more scaling):
            float rho_power = sqrt((float)Reflected / (float)Forward); // Reflection coefficient from power
            wf = (1.0f + rho_power) / (1.0f - rho_power);
        #endif
        wf = abs(wf); 
    }
        
    // clip the SWR at the configured maximum (or if it calculated as NaN/inf)
    if (wf > MaxSWR || isnan(wf) || isinf(wf)) {
        wf = MaxSWR;
    }
    if (wf < 1.0) { // SWR cannot be less than 1.0
        wf = 1.0;
    }

    // store the final result
    *this->SWRvalue = wf;
}