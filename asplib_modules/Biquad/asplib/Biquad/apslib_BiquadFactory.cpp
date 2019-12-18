/*
 * Copyright (C) 2014 Achim Turan, mail@achim-turan.de
 * https://github.com/AchimTuran/asplib
 *
 * This file is part of asplib (Achim's Signal Processing LIBrary)
 *
 * asplib (Achim's Signal Processing LIBrary) is free software:
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * asplib (Achim's Signal Processing LIBrary) is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with asplib (Achim's Signal Processing LIBrary).
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



// this module implements basic biquad filters 
// for more details see: http://en.wikipedia.org/wiki/Digital_biquad_filter

#define _USE_MATH_DEFINES
#include <cmath>

#include "Core/os/asplib_os.h"
#include "Core/Constants_Typedefs/asplib_Constants.h"
#include "Core/Constants_Typedefs/asplib_Typedefs.h"

#include "Biquad/apslib_BiquadFactory.h"
#include "Biquad/Biquad_Native/asplib_BiquadNative.h"

#include <iostream>
using namespace std;

namespace asplib
{
// helper function prototypes
ASPLIB_ERR helper_calcConstQPeakingParam(ASPLIB_CONST_Q_PEAKING_PARAM *ConstQPeakingParam, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients);
ASPLIB_ERR helper_checkBiquadIdx(ASPLIB_BIQUAD_HANDLE *Biquads, uint32_t BiQaudIdx);


// Handle Biquads
ASPLIB_ERR CBiquadFactory::reset_Biquads(ASPLIB_BIQUAD_HANDLE *Biquads)
{
    if(!Biquads)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CBiquadFactory::destroy_Biquads(ASPLIB_BIQUAD_HANDLE **Biquads)
{
    ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;
    if(Biquads && (*Biquads))
    {
        if((*Biquads)->Biquads)
        {
            switch((*Biquads)->optModule)
            {
                case ASPLIB_OPT_NATIVE:
                    delete ((CBiquad_Native*)(*Biquads)->Biquads);
                break;

                default:
                    // ToDo: return some warning code!
                    // err = ;
                break;
            }

            (*Biquads)->Biquads = NULL;
        }

        delete *Biquads;
        *Biquads = NULL;
    }

    return err;
}

uint32_t CBiquadFactory::get_maxBiquads(ASPLIB_BIQUAD_HANDLE *Biquads)
{
    if(!Biquads)
    {
        // ToDo: show error message
        return 0;
    }

    switch(Biquads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            return static_cast<CBiquad_Native*>(Biquads->Biquads)->getMaxBiquads();
        break;

        default:
            // ToDo: throw error!
            return 0;
        break;
    }
}

ASPLIB_BIQUAD_HANDLE* CBiquadFactory::get_Biquads(uint32_t Amount, float SampleFrequency, ASPLIB_OPT_MODULE OptModule)
{
    ASPLIB_BIQUAD_HANDLE *p = new ASPLIB_BIQUAD_HANDLE;
    if(!p)
    {
        // ToDo: throw error!
        return NULL;
    }

    switch(OptModule)
    {
        case ASPLIB_OPT_NATIVE:
            p->optModule = OptModule;
            p->Biquads = new CBiquad_Native(Amount, SampleFrequency);
        break;

        default:
            p->Biquads = NULL;
            p->optModule = ASPLIB_OPT_MIN;
        break;
    }

    if(!p->Biquads)
    {
        // ToDo: throw error!
        // ToDo: show error: couldn't create Biquads, becuase of dynamic memory!
        delete p;
        p = NULL;
        return NULL;
    }
    else
    {
        return p;
    }
}

ASPLIB_ERR CBiquadFactory::calc_BiquadSample(ASPLIB_BIQUAD_HANDLE *Biquads, float In, float *Out)
{
    if(!Biquads || !Out)
    {
        return ASPLIB_ERR_INVALID_INPUT;
    }
    
    switch(Biquads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            *Out = static_cast<CBiquad_Native*>(Biquads->Biquads)->calcSample(In);
        break;

        default:
            // ToDo: throw error!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }

    return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CBiquadFactory::calc_BiquadSamples(ASPLIB_BIQUAD_HANDLE *Biquads, float *In, float *Out, uint32_t FrameSize)
{
    if(!Biquads)
    {
        return ASPLIB_ERR_INVALID_INPUT;
    }

    ASPLIB_ERR err = ASPLIB_ERR_INVALID_INPUT;

    switch(Biquads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            err = static_cast<CBiquad_Native*>(Biquads->Biquads)->calcSamples(In, Out, FrameSize);
        break;

        default:
            // ToDo: throw error!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }

    return err;
}

// set Biquad Parameters
ASPLIB_ERR CBiquadFactory::set_BiquadCoefficients(ASPLIB_BIQUAD_HANDLE *Biquads, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, float C0, float D0)
{
    ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;
    if(C0 != 1.0f)
    {
        Coefficients->a0 *= C0;
        Coefficients->a1 *= C0;
        Coefficients->a2 *= C0;
        Coefficients->b1 *= C0;
        Coefficients->b2 *= C0;
    }

    switch(Biquads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            err = static_cast<CBiquad_Native*>(Biquads->Biquads)->updateCoefficients(Coefficients, D0);
        break;

        default:
            // ToDo: throw error! Unsupported optModule!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }


    return err;
}

ASPLIB_ERR CBiquadFactory::set_BiquadCoefficients(ASPLIB_BIQUAD_HANDLE *Biquads, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, uint32_t BiquadIdx, float C0, float D0)
{
    ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;

    if(C0 != 1.0f)
    {
        Coefficients->a0 *= C0;
        Coefficients->a1 *= C0;
        Coefficients->a2 *= C0;
        Coefficients->b1 *= C0;
        Coefficients->b2 *= C0;
    }

    switch(Biquads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            err = static_cast<CBiquad_Native*>(Biquads->Biquads)->updateCoefficients(Coefficients, D0, BiquadIdx);

        break;

        default:
            // ToDo: throw error! Unsupported optModule!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }


    return err;
}

ASPLIB_ERR CBiquadFactory::set_constQPeakingParams(ASPLIB_BIQUAD_HANDLE *Biquads, float Gain)
{
    if(!Biquads)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    uint32_t maxBands = 0;
    switch(Biquads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            maxBands = static_cast<CBiquad_Native*>(Biquads->Biquads)->getMaxBiquads();
        break;

        default:
            // ToDo: throw error! Unsupported optModule!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }

    ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;
    for (uint32_t ii = 0; ii < maxBands && err == ASPLIB_ERR_NO_ERROR; ii++)
    {
        err = CBiquadFactory::set_constQPeakingParams(Biquads, Gain, ii);
    }

    return err;
}

ASPLIB_ERR CBiquadFactory::set_constQPeakingParams(ASPLIB_BIQUAD_HANDLE *Biquads, float Gain, uint32_t BiquadIdx)
{
    if(!Biquads)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    if(helper_checkBiquadIdx(Biquads, BiquadIdx) != ASPLIB_ERR_NO_ERROR)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    float maxBands = 0.0f;
    switch (Biquads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            maxBands = (float)static_cast<CBiquad_Native*>(Biquads->Biquads)->getMaxBiquads();
        break;

        default:
            // ToDo: throw error! Unsupported optModule!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }
    
    ASPLIB_CONST_Q_PEAKING_PARAM ConstQPeakingParam;
    // calculate quality factor
    // ToDo make octaveEQ variabel
    float octaveEQ = 1.0f;
    ConstQPeakingParam.Gain = Gain;
    ConstQPeakingParam.Q = sqrtf(powf(2.0f, octaveEQ)) / (powf(2.0f, octaveEQ) - 1.0f);

    // calculate center frequency of BiquadIdx
    // ToDo: add functions for calculated centers base 2 & 10 (ISO), preferred centers base 2 & 10 (non ISO), calculated centers - contiguous (non-ISO), Preferred centers - contiguous (non-ISO)
    // ToDo: make maxFrequency variable and dependend from sample frequency
    // ToDo: make baseFrequency variable
    float baseFrequency = E_p3;         // 1000Hz
    float maxFrequency = 20.0f*E_p3;    // 20kHz
    float bandsFactor = maxBands / 10;
    float positiveBands = (float)((int)(bandsFactor*log2f(maxFrequency / baseFrequency))); // round down to next complete number
    float frequencyBandIdx = -maxBands + positiveBands +1 + BiquadIdx;
    ConstQPeakingParam.fc = baseFrequency*powf(2.0f, frequencyBandIdx/bandsFactor);

    // get current sample frequency
    ConstQPeakingParam.fs = 0.0f;
    switch (Biquads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            ConstQPeakingParam.fs = static_cast<CBiquad_Native*>(Biquads->Biquads)->getSampleFrequency();
        break;

        default:
            // ToDo: throw error! Unsupported optModule!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }
    

    ASPLIB_BIQUAD_COEFFICIENTS coefficients;
    ASPLIB_ERR err = helper_calcConstQPeakingParam(&ConstQPeakingParam, &coefficients);
    if(err != ASPLIB_ERR_NO_ERROR)
    {
        // ToDo: throw error!
        return err;
    }

    err = CBiquadFactory::set_BiquadCoefficients(Biquads, &coefficients, BiquadIdx);
    if(err != ASPLIB_ERR_NO_ERROR)
    {
        // ToDo: throw error!
        return err;
    }

    return err;
}

ASPLIB_ERR CBiquadFactory::get_constQPeakingBiquadCoes(ASPLIB_BIQUAD_HANDLE *Biquads, float Gain, uint32_t BiquadIdx, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients)
{
  if(!Biquads || !Coefficients || Gain < 0.0f)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  if(helper_checkBiquadIdx(Biquads, BiquadIdx) != ASPLIB_ERR_NO_ERROR)
  {
      // ToDo: throw error!
      return ASPLIB_ERR_INVALID_INPUT;
  }

  ASPLIB_CONST_Q_PEAKING_PARAM ConstQPeakingParam;
  float maxBands = 0.0f;
  switch (Biquads->optModule)
  {
      case ASPLIB_OPT_NATIVE:
          maxBands = (float)static_cast<CBiquad_Native*>(Biquads->Biquads)->getMaxBiquads();
      break;

      default:
          // ToDo: throw error! Unsupported optModule!
          return ASPLIB_ERR_INVALID_INPUT;
      break;
  }

  float octaveEQ = 1.0f;
  ConstQPeakingParam.Gain = Gain;
  ConstQPeakingParam.Q = sqrtf(powf(2.0f, octaveEQ)) / (powf(2.0f, octaveEQ) - 1.0f);

  // calculate center frequency of BiquadIdx
  // ToDo: add functions for calculated centers base 2 & 10 (ISO), preferred centers base 2 & 10 (non ISO), calculated centers - contiguous (non-ISO), Preferred centers - contiguous (non-ISO)
  // ToDo: make maxFrequency variable and dependend from sample frequency
  // ToDo: make baseFrequency variable
  float baseFrequency = E_p3;         // 1000Hz
  float maxFrequency = 20.0f*E_p3;    // 20kHz
  float bandsFactor = maxBands / 10;
  float positiveBands = (float)((int)(bandsFactor*log2f(maxFrequency / baseFrequency))); // round down to next complete number
  float frequencyBandIdx = -maxBands + positiveBands +1 + BiquadIdx;
  ConstQPeakingParam.fc = baseFrequency*powf(2.0f, frequencyBandIdx/bandsFactor);

  // get current sample frequency
  ConstQPeakingParam.fs = 0.0f;
  switch (Biquads->optModule)
  {
      case ASPLIB_OPT_NATIVE:
          ConstQPeakingParam.fs = static_cast<CBiquad_Native*>(Biquads->Biquads)->getSampleFrequency();
      break;

      default:
          // ToDo: throw error! Unsupported optModule!
          return ASPLIB_ERR_INVALID_INPUT;
      break;
  }

  ConstQPeakingParam.Gain = Gain;

  return helper_calcConstQPeakingParam(&ConstQPeakingParam, Coefficients);
}

ASPLIB_ERR CBiquadFactory::get_constQPeakingBiquadCoes(uint32_t SampleFrequency, uint32_t MaxFreqBands, float Gain, uint32_t BiquadIdx, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients)
{
  if(SampleFrequency <= 0 || MaxFreqBands <= 0 || BiquadIdx >= MaxFreqBands || !Coefficients)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  ASPLIB_CONST_Q_PEAKING_PARAM ConstQPeakingParam;
  float maxBands = (float)MaxFreqBands;
  float octaveEQ = 1.0f;
  ConstQPeakingParam.fs = (float)SampleFrequency;
  ConstQPeakingParam.Gain = Gain;
  ConstQPeakingParam.Q = sqrtf(powf(2.0f, octaveEQ)) / (powf(2.0f, octaveEQ) - 1.0f);
  
  // calculate center frequency of BiquadIdx
  // ToDo: add functions for calculated centers base 2 & 10 (ISO), preferred centers base 2 & 10 (non ISO), calculated centers - contiguous (non-ISO), Preferred centers - contiguous (non-ISO)
  // ToDo: make maxFrequency variable and dependend from sample frequency
  // ToDo: make baseFrequency variable
  float baseFrequency = E_p3;         // 1000Hz
  float maxFrequency = 20.0f*E_p3;    // 20kHz
  float bandsFactor = maxBands / 10;
  float positiveBands = (float)((int)(bandsFactor*log2f(maxFrequency / baseFrequency))); // round down to next complete number
  float frequencyBandIdx = -maxBands + positiveBands +1 + BiquadIdx;
  ConstQPeakingParam.fc = baseFrequency*powf(2.0f, frequencyBandIdx/bandsFactor);

  return helper_calcConstQPeakingParam(&ConstQPeakingParam, Coefficients);
}

//-------------------------
//--- helper functions ----
//-------------------------
ASPLIB_ERR helper_calcConstQPeakingParam(ASPLIB_CONST_Q_PEAKING_PARAM *ConstQPeakingParam, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients)
{
    if(!ConstQPeakingParam || !Coefficients || ConstQPeakingParam->fs <= 0.0f ||
        ConstQPeakingParam->Q <= 0.0f || ConstQPeakingParam->fc <= 0.0f)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    // ToDo: Clamp values like vlc Biquads!
    const float Q = ConstQPeakingParam->Q;
    const float fc = ConstQPeakingParam->fc;
    const float fs = ConstQPeakingParam->fs;
    const float Gain = ConstQPeakingParam->Gain;

    // ToDo: check values to avoid division by zero!
    float K = tanf((float)M_PI*fc/fs);
    float V0 = powf(10.0f, Gain/20.0f);
    float d0 = 1 + K/Q + K*K;
    float e0 = 1 + K/(Q*V0) + K*K;

    float alpha = 1 + K*V0/Q + K*K;
    float beta = 2.0f*(K*K - 1);
    float gamma = 1 - V0*K / Q + K*K;
    float delta = 1 - K/Q + K*K;
    float eta = 1 - K/(V0*Q) + K*K;


    if(Gain > 0.0f)
    { // boost design equations
        Coefficients->a0 = alpha/d0;
        Coefficients->a1 = beta/d0;
        Coefficients->a2 = gamma/d0;

        Coefficients->b1 = -beta/d0;
        Coefficients->b2 = -delta/d0;
    }
    else
    { // cut design equations
        Coefficients->a0 = d0/e0;
        Coefficients->a1 = beta/e0;
        Coefficients->a2 = delta/e0;

        Coefficients->b1 = -beta/e0;
        Coefficients->b2 = -eta/e0;
    }

    return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR helper_checkBiquadIdx(ASPLIB_BIQUAD_HANDLE *Biquads, uint32_t BiQaudIdx)
{
    if(!Biquads)
    {
    // ToDo: throw error!
    return ASPLIB_ERR_INVALID_INPUT;
    }
    
    switch(Biquads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            if(BiQaudIdx >= static_cast<CBiquad_Native*>(Biquads->Biquads)->getMaxBiquads())
            {
                // ToDo: throw error!
                return ASPLIB_ERR_INVALID_INPUT;
            }
        break;

        default:
            // ToDo: throw error! Unsupported optModule!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }

    return ASPLIB_ERR_NO_ERROR;
}
}
