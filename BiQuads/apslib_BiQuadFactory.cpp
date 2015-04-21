/*
 * Copyright (C) 2014 Achim Turan, Achim.Turan@o2online.de
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

#include "../constants_typedefs/asplib_constants.h"
#include "../constants_typedefs/asplib_typedefs.h"

#include "apslib_BiQuadFactory.h"
#include "BiQuad_Native/asplib_BiQuad_Native.h"

#include <iostream>
using namespace std;

namespace asplib
{
// helper function prototypes
ASPLIB_ERR helper_calcConstQPeakingParam(ASPLIB_CONST_Q_PEAKING_PARAM *ConstQPeakingParam, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients);
ASPLIB_ERR helper_checkBiQuadIdx(ASPLIB_BIQUAD_HANDLE *BiQuads, uint BiQaudIdx);


// Handle BiQuads
ASPLIB_ERR CBiQuadFactory::reset_BiQuads(ASPLIB_BIQUAD_HANDLE *BiQuads)
{
    if(!BiQuads)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CBiQuadFactory::destroy_BiQuads(ASPLIB_BIQUAD_HANDLE **BiQuads)
{
    ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;
    if(BiQuads && (*BiQuads))
    {
        if((*BiQuads)->BiQuads)
        {
            switch((*BiQuads)->optModule)
            {
                case ASPLIB_OPT_NATIVE:
                    delete ((CBiQuad_Native*)(*BiQuads)->BiQuads);
                break;

                default:
                    // ToDo: return some warning code!
                    // err = ;
                break;
            }

            (*BiQuads)->BiQuads = NULL;
        }

        delete *BiQuads;
        *BiQuads = NULL;
    }

    return err;
}

uint CBiQuadFactory::get_maxBiQuads(ASPLIB_BIQUAD_HANDLE *BiQuads)
{
    if(!BiQuads)
    {
        // ToDo: show error message
        return 0;
    }

    switch(BiQuads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            return static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->getMaxBiquads();
        break;

        default:
            // ToDo: throw error!
            return 0;
        break;
    }
}

ASPLIB_BIQUAD_HANDLE* CBiQuadFactory::get_BiQuads(uint Quantity, float SampleFrequency, ASPLIB_OPT_MODULE OptModule)
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
            p->BiQuads = new CBiQuad_Native(Quantity, SampleFrequency);
        break;

        default:
            p->BiQuads = NULL;
            p->optModule = ASPLIB_OPT_MIN;
        break;
    }

    if(!p->BiQuads)
    {
        // ToDo: throw error!
        // ToDo: show error: couldn't create BiQuads, becuase of dynamic memory!
        delete p;
        p = NULL;
        return NULL;
    }
    else
    {
        return p;
    }
}

ASPLIB_ERR CBiQuadFactory::calc_BiQuadSample(ASPLIB_BIQUAD_HANDLE *BiQuads, float In, float *Out)
{
    if(!BiQuads || !Out)
    {
        return ASPLIB_ERR_INVALID_INPUT;
    }
    
    switch(BiQuads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            *Out = static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->calcSample(In);
        break;

        default:
            // ToDo: throw error!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }

    return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CBiQuadFactory::calc_BiQuadSamples(ASPLIB_BIQUAD_HANDLE *BiQuads, float *In, float *Out, uint FrameSize)
{
    if(!BiQuads)
    {
        return ASPLIB_ERR_INVALID_INPUT;
    }

    ASPLIB_ERR err = ASPLIB_ERR_INVALID_INPUT;

    switch(BiQuads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            err = static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->calcSamples(In, Out, FrameSize);
        break;

        default:
            // ToDo: throw error!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }

    return err;
}

// set BiQuad Parameters
ASPLIB_ERR CBiQuadFactory::set_BiQuadCoefficients(ASPLIB_BIQUAD_HANDLE *BiQuads, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, float C0, float D0)
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

    switch(BiQuads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            err = static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->updateCoefficients(Coefficients, D0);
        break;

        default:
            // ToDo: throw error! Unsupported optModule!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }


    return err;
}

ASPLIB_ERR CBiQuadFactory::set_BiQuadCoefficients(ASPLIB_BIQUAD_HANDLE *BiQuads, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, uint BiQuadIdx, float C0, float D0)
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

    switch(BiQuads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            err = static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->updateCoefficients(Coefficients, D0, BiQuadIdx);

        break;

        default:
            // ToDo: throw error! Unsupported optModule!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }


    return err;
}

ASPLIB_ERR CBiQuadFactory::set_constQPeakingParams(ASPLIB_BIQUAD_HANDLE *BiQuads, float Gain)
{
    if(!BiQuads)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    uint maxBands = 0;
    switch(BiQuads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            maxBands = static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->getMaxBiquads();
        break;

        default:
            // ToDo: throw error! Unsupported optModule!
            return ASPLIB_ERR_INVALID_INPUT;
        break;
    }

    ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;
    for(uint ii = 0; ii < maxBands && err == ASPLIB_ERR_NO_ERROR; ii++)
    {
        err = CBiQuadFactory::set_constQPeakingParams(BiQuads, Gain, ii);
    }

    return err;
}

ASPLIB_ERR CBiQuadFactory::set_constQPeakingParams(ASPLIB_BIQUAD_HANDLE *BiQuads, float Gain, uint BiQuadIdx)
{
    if(!BiQuads)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    if(helper_checkBiQuadIdx(BiQuads, BiQuadIdx) != ASPLIB_ERR_NO_ERROR)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    float maxBands = 0.0f;
    switch (BiQuads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            maxBands = (float)static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->getMaxBiquads();
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

    // calculate center frequency of BiQuadIdx
    // ToDo: add functions for calculated centers base 2 & 10 (ISO), preferred centers base 2 & 10 (non ISO), calculated centers - contiguous (non-ISO), Preferred centers - contiguous (non-ISO)
    // ToDo: make maxFrequency variable and dependend from sample frequency
    // ToDo: make baseFrequency variable
    float baseFrequency = E_p3;         // 1000Hz
    float maxFrequency = 20.0f*E_p3;    // 20kHz
    float bandsFactor = maxBands / 10;
    float positiveBands = (float)((int)(bandsFactor*log2f(maxFrequency / baseFrequency))); // round down to next complete number
    float frequencyBandIdx = -maxBands + positiveBands +1 + BiQuadIdx;
    ConstQPeakingParam.fc = baseFrequency*powf(2.0f, frequencyBandIdx/bandsFactor);

    // get current sample frequency
    ConstQPeakingParam.fs = 0.0f;
    switch (BiQuads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            ConstQPeakingParam.fs = static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->getSampleFrequency();
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

    err = CBiQuadFactory::set_BiQuadCoefficients(BiQuads, &coefficients, BiQuadIdx);
    if(err != ASPLIB_ERR_NO_ERROR)
    {
        // ToDo: throw error!
        return err;
    }

    return err;
}

ASPLIB_ERR CBiQuadFactory::get_constQPeakingBiQuadCoes(ASPLIB_BIQUAD_HANDLE *BiQuads, float Gain, uint BiQuadIdx, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients)
{
  if(!BiQuads || !Coefficients || Gain < 0.0f)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  if(helper_checkBiQuadIdx(BiQuads, BiQuadIdx) != ASPLIB_ERR_NO_ERROR)
  {
      // ToDo: throw error!
      return ASPLIB_ERR_INVALID_INPUT;
  }

  ASPLIB_CONST_Q_PEAKING_PARAM ConstQPeakingParam;
  float maxBands = 0.0f;
  switch (BiQuads->optModule)
  {
      case ASPLIB_OPT_NATIVE:
          maxBands = (float)static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->getMaxBiquads();
      break;

      default:
          // ToDo: throw error! Unsupported optModule!
          return ASPLIB_ERR_INVALID_INPUT;
      break;
  }

  float octaveEQ = 1.0f;
  ConstQPeakingParam.Gain = Gain;
  ConstQPeakingParam.Q = sqrtf(powf(2.0f, octaveEQ)) / (powf(2.0f, octaveEQ) - 1.0f);

  // calculate center frequency of BiQuadIdx
  // ToDo: add functions for calculated centers base 2 & 10 (ISO), preferred centers base 2 & 10 (non ISO), calculated centers - contiguous (non-ISO), Preferred centers - contiguous (non-ISO)
  // ToDo: make maxFrequency variable and dependend from sample frequency
  // ToDo: make baseFrequency variable
  float baseFrequency = E_p3;         // 1000Hz
  float maxFrequency = 20.0f*E_p3;    // 20kHz
  float bandsFactor = maxBands / 10;
  float positiveBands = (float)((int)(bandsFactor*log2f(maxFrequency / baseFrequency))); // round down to next complete number
  float frequencyBandIdx = -maxBands + positiveBands +1 + BiQuadIdx;
  ConstQPeakingParam.fc = baseFrequency*powf(2.0f, frequencyBandIdx/bandsFactor);

  // get current sample frequency
  ConstQPeakingParam.fs = 0.0f;
  switch (BiQuads->optModule)
  {
      case ASPLIB_OPT_NATIVE:
          ConstQPeakingParam.fs = static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->getSampleFrequency();
      break;

      default:
          // ToDo: throw error! Unsupported optModule!
          return ASPLIB_ERR_INVALID_INPUT;
      break;
  }

  ConstQPeakingParam.Gain = Gain;

  return helper_calcConstQPeakingParam(&ConstQPeakingParam, Coefficients);
}

ASPLIB_ERR CBiQuadFactory::get_constQPeakingBiquadCoes(uint SampleFrequency, uint MaxFreqBands, float Gain, uint BiquadIdx, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients)
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
  
  // calculate center frequency of BiQuadIdx
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

    // ToDo: Clamp values like vlc BiQuads!
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

ASPLIB_ERR helper_checkBiQuadIdx(ASPLIB_BIQUAD_HANDLE *BiQuads, uint BiQaudIdx)
{
    if(!BiQuads)
    {
    // ToDo: throw error!
    return ASPLIB_ERR_INVALID_INPUT;
    }
    
    switch(BiQuads->optModule)
    {
        case ASPLIB_OPT_NATIVE:
            if(BiQaudIdx >= static_cast<CBiQuad_Native*>(BiQuads->BiQuads)->getMaxBiquads())
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
