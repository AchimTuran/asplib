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



#include "Dynamics/Compressor/asplib_DynamicsCompressor.hpp"

#include <cmath>


namespace asplib
{
CCompressor::CCompressor()
{
  m_FrameSize       = 0;
  m_SampleFrequency = 0;

  m_AlphaRelease      = 0.0f;
  m_AlphaAttack       = 0.0f;
  m_Threshold        = 0.0f;
  m_CompressionRatio = 0.0f;
}


CCompressor::~CCompressor()
{
}


ASPLIB_ERR CCompressor::Create(uint32_t FrameSize, uint32_t SampleFrequency, void *Options/* = nullptr*/)
{
  CompressorOptions options;
  if (Options)
  {
    CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Options);
    if (extendedStruct->ID != ASPLIB_EXTENDED_STRUCT_CompressorOptions)
    {
      return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT;
    }

    // TODO mtk2003: value checking
    CompressorOptions *pOptions = static_cast<CompressorOptions*>(Options);
    if (pOptions->compressionRatio <= 1.0 || pOptions->tauRelease <= 0.0 || pOptions->tauAttack <= 0.0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    options.tauRelease        = pOptions->tauRelease;
    options.tauAttack         = pOptions->tauAttack;
    options.threshold         = pOptions->threshold;
    options.compressionRatio  = pOptions->compressionRatio;
    m_GainCurve               = pOptions->gainCurve;
  }

  if (FrameSize <= 0 || SampleFrequency <= 0)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }
  m_FrameSize       = FrameSize;
  m_SampleFrequency = SampleFrequency;

  m_AlphaAttack  = (float)exp(-1.0 / (options.tauAttack*((long double)m_SampleFrequency)));
  m_AlphaRelease = (float)exp(-1.0 / (options.tauRelease*((long double)m_SampleFrequency)));

  m_Threshold        = (float)options.threshold;
  m_CompressionRatio = (float)options.compressionRatio;
  m_GainCurve        = options.gainCurve;

  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CCompressor::Process(void *In, void *Out)
{
  float *in = static_cast<float*>(In);
  float *out = static_cast<float*>(Out);

  float xT  = 0.0f;
  float eps = E_m12;
  float xG  = 0.0f;
  float yG  = 0.0f;
  float yL  = 0.0f;
  float xL = 0.0f;

  for (uint32_t ii = 0; ii < m_FrameSize; ii++)
  {
    xT = abs(in[ii]);
    if (xT <= eps)
    {
      xT = eps;
    }
    xG = 20.0f * log10f(xT);

    // gain computation
    if (xG <= m_Threshold)
    {
      yG = xG;
    }
    else
    {
      yG = m_Threshold + (xG - m_Threshold) / m_CompressionRatio;
    }

    switch (m_GainCurve)
    {
      case CompressorOptions::COMPRESSION_HARD_CLIPPING:
	      // TODO
        //yG = function();
      break;
    }

    // peak detection
    xL = xG - yG;
    if (xL > m_yL_old)
    {
      yL = m_AlphaAttack*m_yL_old +  (1.0f - m_AlphaAttack)*xL;
    }
    else
    {
      yL = m_AlphaRelease*m_yL_old + (1.0f - m_AlphaRelease)*xL;
    }

    if (xT <= eps)
    {
      out[ii] = in[ii];
    }
    else
    {
      out[ii] = in[ii] * powf(10.0f, (-yL / 20.0f));
    }
    m_yL_old = yL;
  }

  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CCompressor::Destroy()
{
  return ASPLIB_ERR_NO_ERROR;
}
}
