/* Copyright (C) 2014-2016 Achim Turan, Achim.Turan@o2online.de
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

// math includes should be the first as includes to STL override the _USE_MATH_DEFINES define...
#if defined(TARGET_WINDOWS)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

// resampling includes
#include "Resampling/Decimator/asplib_Decimator.hpp"



namespace asplib
{
CDecimator::CDecimator() :
  m_inputFrameSize(0),
  m_outputFrameSize(0),
  m_filterLength(0),
  m_sampleFrequency(0),
  m_decimationFactor(0),
  m_cutOffFrequency(0.0f)
{
}


CDecimator::~CDecimator()
{
}


ASPLIB_ERR CDecimator::Create(const uint32_t InputFrameSize, const uint32_t OutputFrameSize, const uint32_t SampleFrequency, const void *Options/* = nullptr*/)
{
  DecimatorOptions options;
  if (Options)
  {
    const CExtendedStructs *extendedStruct = static_cast<const CExtendedStructs*>(Options);
    if (extendedStruct->ID != ASPLIB_EXTENDED_STRUCT_DecimatorOptions)
    {
      return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT;
    }

    const DecimatorOptions *pOptions = static_cast<const DecimatorOptions*>(Options);
    if (pOptions->decimationFactor <= 1.0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    options.decimationFactor = pOptions->decimationFactor;
    options.filterLength = pOptions->filterLength;
    options.cutoffFrequency = pOptions->cutoffFrequency;
  }

  if (InputFrameSize <= 0 || SampleFrequency <= 0)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }
  m_inputFrameSize = InputFrameSize;
  m_sampleFrequency = SampleFrequency;
  m_cutOffFrequency = options.cutoffFrequency;
  m_filterLength = options.filterLength;
  
  const float normFrequency = m_cutOffFrequency / m_sampleFrequency;
  const uint32_t filterOrder = m_filterLength - 1;

  for (size_t ii = 0; ii < m_filterLength; ii++)
  {
    float val = ::sinf(2.0f*static_cast<float>(M_PI)*normFrequency*(ii - filterOrder*0.5f)) / (static_cast<float>(M_PI) * (ii - filterOrder*0.5f));
    m_filterCoeff[ii] = val*(0.54f - 0.46f*::cos(2.0f * static_cast<float>(M_PI)*ii / filterOrder));
  }
  m_filterCoeff[filterOrder / 2] = 2.0f * normFrequency;

  m_convolutionBuffer.resize(m_inputFrameSize + m_filterLength - 1, 0.0f);

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CDecimator::Process(const void *In, void *Out)
{
  const float *in = static_cast<const float*>(In);
  float *out = static_cast<float*>(Out);

  for (uint32_t ii = 0; ii < m_convolutionBuffer.size(); ii++)
  {
    m_convolutionBuffer[ii] = 0.0;
    for (size_t jj = 0; jj < m_filterLength; jj++)
    {
      if ((ii - jj) <= 1 && (ii - jj) <= m_inputFrameSize)// Input Größe
      {
        m_convolutionBuffer[ii] += m_convolutionBuffer[ii] + m_filterCoeff[jj] * in[ii - jj];
      }
    }

    for (size_t kk = 0; kk < m_outputFrameSize; kk++) // Ausgangsgröße
    {
      out[ii] = m_decimationFactor* m_convolutionBuffer[kk * m_decimationFactor];
    }
  }

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CDecimator::Destroy()
{
  return ASPLIB_ERR_NO_ERROR;
}
}
