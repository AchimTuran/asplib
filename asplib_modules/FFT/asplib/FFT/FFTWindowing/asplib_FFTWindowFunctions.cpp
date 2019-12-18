/* Copyright (C) 2014-2015 Achim Turan, mail@achim-turan.de
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



#include <memory.h>
#if defined(TARGET_WINDOWS)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "FFT/FFTWindowing/asplib_FFTWindowFunctions.h"

namespace asplib
{
CFFTWindowFunctions::CFFTWindowFunctions()
{
  m_A = 0.0;
  m_B = 0.0;
  m_C = 0.0;
  m_D = 0.0;

  m_WindowCoe = NULL;
  m_FrameSize = 0;
  m_WindowID = FFT_WINDOW_ID_INVALID;
}

CFFTWindowFunctions::~CFFTWindowFunctions()
{
  Destroy();
}

ASPLIB_ERR CFFTWindowFunctions::Init(uint32_t FrameSize, eFFT_WINDOW_ID WindowID)
{
  if(FrameSize <= 0 || WindowID <= FFT_WINDOW_ID_INVALID || WindowID >= FFT_WINDOW_ID_MAX)
  {
    // TODO: error code
    return ASPLIB_ERR_INVALID_INPUT;
  }

  if (m_WindowCoe)
  {
    Destroy();
  }

  // set window properties
  m_FrameSize = FrameSize;
  m_WindowID = WindowID;

  // create window values array
  ASPLIB_ERR err = Create();
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    // TODO: error code
    return err;
  }

  // set window coefficients
  switch(m_WindowID)
  {
    case FFT_WINDOW_ID_HAMMING:
    {
      m_A = 0.54;
      m_B = 0.46;
    }
    break;

    case FFT_WINDOW_ID_HANN:
    {
      m_A = 0.5;
      m_B = 0.5;
    }
    break;

    case FFT_WINDOW_ID_BLACKMAN:
    {
      m_A = 0.42;
      m_B = 0.5;
      m_C = 0.08;
    }
    break;

    case FFT_WINDOW_ID_BLACKMAN_HARRIS:
    {
      m_A = 0.35875;
      m_B = 0.48829;
      m_C = 0.14128;
      m_D = 0.01168;
    }
    break;

    default:
      // TODO: error code
      return ASPLIB_ERR_INVALID_INPUT;
    break;
  }

  double denum = (double)m_FrameSize - 1.0;
  double M_2PI = 2.0*M_PI;
  double M_4PI = 4.0*M_PI;
  double M_6PI = 6.0*M_PI;
  // calculate window coefficients
  switch(m_WindowID)
  {
    case FFT_WINDOW_ID_HAMMING:
    case FFT_WINDOW_ID_HANN:
    {
      for (uint32_t ii = 0; ii < m_FrameSize; ii++)
      {
        double kk = (double)ii;
        m_WindowCoe[ii] = (float)(m_A - m_B*cos(M_2PI*kk/denum));
      }
    }
    break;

    case FFT_WINDOW_ID_BLACKMAN:
    {
      for (uint32_t ii = 0; ii < m_FrameSize; ii++)
      {
        double kk = (double)ii;
        m_WindowCoe[ii] = (float)(m_A
                                  - m_B*cos(M_2PI*kk/denum)
                                  + m_C*cos(M_4PI*kk/denum));
      }
    }
    break;

    case FFT_WINDOW_ID_BLACKMAN_HARRIS:
    {
      for (uint32_t ii = 0; ii < m_FrameSize; ii++)
      {
        double kk = (double)ii;
        m_WindowCoe[ii] = (float)(m_A
                                  - m_B*cos(M_2PI*kk/denum)
                                  + m_C*cos(M_4PI*kk/denum)
                                  - m_D*cos(M_6PI*kk/denum));
      }
    }
    break;

    default:
      return ASPLIB_ERR_INVALID_INPUT;
    break;
  }


  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CFFTWindowFunctions::Process(float *In, float *Out)
{
  if (!In || !Out)
  {
    // TODO: error code
    return ASPLIB_ERR_INVALID_INPUT;
  }

  for (uint32_t ii = 0; ii < m_FrameSize; ii++)
  {
    Out[ii] = m_WindowCoe[ii]*In[ii];
  }

  return ASPLIB_ERR_NO_ERROR;
}

void CFFTWindowFunctions::Destroy()
{
  if (m_WindowCoe)
  {
    delete [] m_WindowCoe;
    m_WindowCoe = NULL;
  }

  m_FrameSize = 0;
  m_WindowID = FFT_WINDOW_ID_INVALID;
}

ASPLIB_ERR CFFTWindowFunctions::Create()
{
  m_WindowCoe = new float[m_FrameSize];
  if (!m_WindowCoe)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  return ASPLIB_ERR_NO_ERROR;
}
}
