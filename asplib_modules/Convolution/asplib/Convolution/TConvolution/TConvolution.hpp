#pragma once

/* Copyright (C) 2014-2016 Achim Turan, mail@achim-turan.de
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


#include "Core/os/asplib_os.h"
#include "Core/Constants_Typedefs/asplib_Typedefs.h"
#include "Core/Constants_Typedefs/asplib_DataFmtDefines.hpp"
#include "Core/Buffers/asplib_TFrameBuffer.h"
#include "Core/Buffers/asplib_TFrameRingBuffer.h"

#include "Convolution/Interfaces/asplib_IConvolution.hpp"

#include "FFT/asplib_FFTFactory.hpp"


namespace asplib
{
template<class T>
class TConvolution : public IConvolution
{
  asplibFmt_t internalFmt = ASPLIB_FMT_NATIVE_CPX_FLOAT;
  asplibFmt_t FFTInFmt = ASPLIB_FMT_NATIVE_FLOAT;

public:
  TConvolution() {}
  ~TConvolution() {}

  virtual ASPLIB_ERR Create(uint32_t FrameSize, FFTLibID_t FFTLibID, void *Filter, uint32_t FilterLength, uint32_t MaxParts, void *Options = nullptr)
  {
    IFFT *fft = nullptr;
    ASPLIB_ERR err = ASPLIB_FACTORY_FFT.Create(FFTLibID, FFTInFmt, internalFmt, fft);
    if(err != ASPLIB_ERR_NO_ERROR)
    {
      return err;
    }

    err = fft->Create(FrameSize);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      return err;
    }

    float *filter = (float*)Filter;
    m_MaxParts = MaxParts;
    uint32_t filterPartSize = FilterLength / m_MaxParts;
    if (filterPartSize*m_MaxParts < FilterLength)
    {
      m_MaxParts++;
    }

    m_G = new TFrameBuffer<T>(filterPartSize, MaxParts);
    for (uint32_t ii = 0; ii < MaxParts; ii++)
    {
      m_fft->FFT(&filter[ii*filterPartSize], m_G->get_Frame(ii));
    }

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR Convolve(void *x, void *y)
  {
    m_fft->FFT(x, m_X->get_NextFrame());

    for (uint32_t frame = 0; frame < m_MaxParts; frame++)
    {
      T *X = m_X->get_Frame(frame);
      T *G = m_G->get_Frame(frame);
      T *Y = m_Y->get_Frame(frame);

      for (uint32_t ii = 0; ii < m_MaxFrameSize; ii++)
      {
        Y[ii] = G[ii]*X[ii];
      }
    }

    T *Y0 = m_Y->get_Frame(0);
    for (uint32_t frame = 1; frame < m_MaxParts; frame++)
    {
      T *Y = m_Y->get_Frame(frame);
      for (uint32_t ii = 0; ii < m_MaxFrameSize; ii++)
      {
        Y0[ii] += Y[ii];
      }
    }

    m_fft->iFFT(Y0, y);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR Destroy()
  {
    ASPLIB_FACTORY_FFT.Destroy(m_fft);

    delete m_G; m_G = nullptr;
    delete m_X; m_X = nullptr;
    delete m_Y; m_Y = nullptr;

    return ASPLIB_ERR_NO_ERROR;
  }

private:
  IFFT *m_fft;

  uint32_t m_MaxParts;
  uint32_t m_MaxFrameSize;

  TFrameBuffer<T>      *m_G;
  TFrameRingBuffer<T>  *m_X;
  TFrameBuffer<T>      *m_Y;
};
}
