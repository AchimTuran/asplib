#pragma once

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



#include "Resampling/Decimator/asplib_Decimator.hpp"

#if defined(TARGET_WINDOWS)
#define _USE_MATH_DEFINES
#endif
#include <cmath>


namespace asplib
{
CDecimator::CDecimator()
{
  m_FrameSize         = 0;
  m_FilterLength      = 0;
  m_SampleFrequency   = 0;
  m_DecimationFactor  = 0;
  m_CutOffFrequency   = 0.0f;
}


CDecimator::~CDecimator()
{
}


ASPLIB_ERR CDecimator::Create(uint32_t FrameSize, uint32_t SampleFrequency, void *Options/* = nullptr*/)
{
  DecimatorOptions options;
  if (Options)
  {
    CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Options);
    if (extendedStruct->ID != ASPLIB_EXTENDED_STRUCT_DecimatorOptions)
    {
      return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT;
    }

    DecimatorOptions *pOptions = static_cast<DecimatorOptions*>(Options);
    if (pOptions->decimationFactor <= 1.0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    options.decimationFactor = pOptions->decimationFactor;
    options.filterLength = pOptions->filterLength;
    options.cutoffFrequency = pOptions->cutoffFrequency;
  }

  if (FrameSize <= 0 || SampleFrequency <= 0)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }
  m_FrameSize = FrameSize;
  m_SampleFrequency = SampleFrequency;

  float val = 0.0f;
  float normFrequency = m_CutOffFrequency / m_SampleFrequency;
  uint32_t filterOrder = m_FilterLength - 1;

  for (size_t ii = 0; ii < m_FilterLength; ii++)
  {
    val = sinf(2.0f*M_PI*normFrequency*(ii - filterOrder*0.5f)) / (M_PI * (ii - filterOrder*0.5f));
    m_FilterCoeff[ii] = val*(0.54 - 0.46*cos(2 * M_PI*ii / filterOrder));
  }
  m_FilterCoeff[filterOrder / 2] = 2.0f * normFrequency;

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CDecimator::Process(void *In, void *Out)
{
  float *in = static_cast<float*>(In);
  float *out = static_cast<float*>(Out);

  // Die Laenge des Ausgangspuffers ist ja kleiner als des Eingangspuffers. Wie komme ich an diese Länge?
  uint32_t convolutionLength = m_FrameSizeInput + m_FilterLength - 1;
  float outTemp[convolutionLength];

  for (uint32_t ii = 0; ii < convolutionLength; ii++)
  {
    outTemp[ii] = 0.0;
    for (size_t jj = 0; jj < m_FilterLength; jj++)
    {
      if ((ii - jj) <= 1 && (ii - jj) <= m_FrameSizeInput)// Input Größe
      {
        outTemp[ii] += outTemp[ii] + m_FilterCoeff[jj] * in[ii - jj];
      }
    }

    for (size_t kk = 0; kk < m_FrameSizeTemp; kk++) // Ausgangsgröße
    {
      out[ii] = m_DecimationFactor*outTemp[kk * m_DecimationFactor];
    }
  }

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CDecimator::Destroy()
{
  return ASPLIB_ERR_NO_ERROR;
}
}
