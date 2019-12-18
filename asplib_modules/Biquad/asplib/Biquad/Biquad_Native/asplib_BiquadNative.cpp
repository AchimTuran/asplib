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
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */



#include <iostream>
using namespace std;

#include "Biquad/Biquad_Native/asplib_BiquadNative.h"

#define MAX_PARAM_PER_BIQUAD  12

namespace asplib
{

CBiquad_Native::CBiquad_Native(uint32_t Amount, float SampleFrequency) : 
    IBiquad<float>(Amount, SampleFrequency)
{
    if (!Amount)
    {
        // ToDo: throw error!
    }

    this->m_parameters = new float[MAX_PARAM_PER_BIQUAD*Amount];
    if (!m_parameters)
    {
        // ToDo: throw error!
    }

    for (uint32_t ii = 0; ii < MAX_PARAM_PER_BIQUAD*Amount; ii++)
    {
        m_parameters[ii] = 0.0f;
    }
}

CBiquad_Native::~CBiquad_Native()
{
    if (this->m_parameters)
    {
        delete this->m_parameters;
        this->m_parameters = NULL;
    }
}

ASPLIB_ERR CBiquad_Native::updateCoefficients(ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, float D0)
{
    ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;
    const uint32_t maxAmount = getMaxBiquads();
    for (uint32_t ii = 0; ii < maxAmount && err == ASPLIB_ERR_NO_ERROR; ii++)
    {
        err = updateCoefficients(Coefficients, D0, ii);
    }

    return err;
}

ASPLIB_ERR CBiquad_Native::updateCoefficients(ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, float D0, uint32_t BiquadIdx)
{
    if (BiquadIdx >= getMaxBiquads() || !Coefficients)
    {
        // ToDo: throw error!
        return ASPLIB_ERR_INVALID_INPUT;
    }

    float *filterParam = m_parameters + MAX_PARAM_PER_BIQUAD*BiquadIdx;

    filterParam[0] = D0;
    filterParam[1] = Coefficients->a0;
    filterParam[2] = Coefficients->a1;
    filterParam[3] = Coefficients->a2;
    filterParam[4] = Coefficients->b1;
    filterParam[5] = Coefficients->b2;
    filterParam[6] = 0.0f;
    filterParam[7] = 0.0f;

    return ASPLIB_ERR_NO_ERROR;
}

// Set all past values (x[k-1], x[k-2], y[k-1] & y[k-2]) to zero.
void CBiquad_Native::resetState()
{
    const uint32_t maxBiquads = getMaxBiquads();
    for (uint32_t ii = 0; ii < maxBiquads; ii++)
    {
        // only destroy the past values of the filter!
        float *filterParam = m_parameters + MAX_PARAM_PER_BIQUAD*ii + 8;
        for (uint32_t kk = 0; kk < 4; ii++)
        {
            filterParam[kk] = 0.0f;
        }
    }
}

// calculate one output sample with the following difference equation
// y[k] = a0*x[k] + a1*x[k-1] + a2*x[k-2] - (b1*y[k-1] + b2*y[k-2])
float CBiquad_Native::calcSample(float In)
{
    const uint32_t maxBiquads = getMaxBiquads();
    float out = In;
    for (uint32_t ii = 0; ii < maxBiquads; ii++)
    {
        float *filterParam = m_parameters + MAX_PARAM_PER_BIQUAD*ii;

        // y[k] = d0*x[k] + c0*a0*x[k] + c0*a1*x[k-1] + c0*a2*x[k-2] - c0*b1*y[k-1] - c0*b2*y[k-2]
        float tempOut = (filterParam[0] + filterParam[1])*out   // d0*x[k] + c0*a0*x[k]
                        + filterParam[2]*filterParam[8]         // c0*a1*x[k-1]
                        + filterParam[3]*filterParam[9]         // c0*a2*x[k-2]
                        + filterParam[4]*filterParam[10]        // c0*b1*y[k-1]
                        + filterParam[5]*filterParam[11];       // c0*b2*y[k-2]

        // shift past input values
        filterParam[9] = filterParam[8];
        filterParam[8] = out;

        // shift past output values
        filterParam[11] = filterParam[10];
        filterParam[10] = tempOut;
        out = tempOut;
    }

    return out;
}

ASPLIB_ERR CBiquad_Native::calcSamples(float *In, float *Out, uint32_t N)
{
    if (!In || !Out || !N)
    {
        return ASPLIB_ERR_INVALID_INPUT;
    }

    for (uint32_t ii = 0; ii < N; ii++)
    {
        const uint32_t maxBiquads = getMaxBiquads();
        float out = In[ii];
        for (uint32_t jj = 0; jj < maxBiquads; jj++)
        {
            float *filterParam = m_parameters + MAX_PARAM_PER_BIQUAD*jj;

            // y[k] = d0*x[k] + c0*a0*x[k] + c0*a1*x[k-1] + c0*a2*x[k-2] - c0*b1*y[k-1] - c0*b2*y[k-2]
            float tempOut = (filterParam[0] + filterParam[1])*out   // d0*x[k] + c0*a0*x[k]
                + filterParam[2]*filterParam[8]                     // c0*a1*x[k-1]
                + filterParam[3]*filterParam[9]                     // c0*a2*x[k-2]
                + filterParam[4]*filterParam[10]                    // -c0*b1*y[k-1]
                + filterParam[5]*filterParam[11];                   // -c0*b2*y[k-2]

            // shift past input values
            filterParam[9] = filterParam[8];
            filterParam[8] = out;

            // shift past output values
            filterParam[11] = filterParam[10];
            filterParam[10] = tempOut;
            out = tempOut;
        }

        Out[ii] = out;
    }

    return ASPLIB_ERR_NO_ERROR;
}
};
