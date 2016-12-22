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
  asplibFmt_t FFTInFmt = ASPLIB_FMT_NATIVE_FLOAT;

public:
  TConvolution()
  {
    m_G = nullptr;
    m_X = nullptr;
    m_Y = nullptr;
    m_y = nullptr;
    m_x = nullptr;

    m_fft = nullptr;

    m_ScaleValue          = 1.0;

    m_MaxParts            = 0;
    m_MaxFrameSize        = 0;
    m_MaxInteralFrameSize = 0;
    m_MaxVectorFrameSize  = 0;
  }
  ~TConvolution() { Destroy(); }

  virtual ASPLIB_ERR Create(uint32_t FrameSize, FFTLibID_t FFTLibID, void *Filter, uint32_t FilterLength, void *Options = nullptr)
  {
    ASPLIB_ERR err = ASPLIB_FACTORY_FFT.Create(FFTLibID, FFTInFmt, T::typeID, m_fft);
    if(err != ASPLIB_ERR_NO_ERROR)
    {
      return err;
    }

    m_MaxFrameSize = FrameSize;
    err = m_fft->Create(m_MaxFrameSize*2);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      return err;
    }

    // set internal frame size and scale factor
    m_MaxInteralFrameSize = m_fft->InternalFrameSize();// / T::vectorSize;
    m_MaxVectorFrameSize = m_MaxInteralFrameSize / T::vectorSize;
    m_ScaleValue = 1.0 / (double)m_fft->InternalFrameSize();

    // store filter in internal buffer
    m_MaxParts = FilterLength / m_MaxFrameSize;
    if (m_MaxFrameSize*m_MaxParts < FilterLength)
    {
      m_MaxParts++;
    }

    TFrameBuffer<T::type> *filter = new TFrameBuffer<T::type>(m_fft->InternalFrameSize(), m_MaxParts);
    filter->ResetBuffer();
    for (uint32_t ii = 0; ii < m_MaxParts; ii++)
    {
      T::type *p = filter->get_Frame(ii);
      for (uint32_t jj = 0; jj < m_MaxFrameSize && ii*m_MaxParts + jj < FilterLength; jj++)
      {
        p[jj] = ((T::type*)Filter)[ii*m_MaxParts + jj];
      }
    }

    // instanciate buffers
    m_G = new TFrameBuffer<T>(m_MaxInteralFrameSize, m_MaxParts);
    m_X = new TFrameRingBuffer<T>(m_MaxInteralFrameSize, m_MaxParts);
    m_Y = new TFrameBuffer<T>(m_MaxInteralFrameSize, m_MaxParts);
    m_y = new TFrameBuffer<T::type>(m_MaxInteralFrameSize, 1);
    m_x = new TFrameBuffer<T::type>(m_MaxInteralFrameSize, 1);
    for (uint32_t ii = 0; ii < m_MaxParts; ii++)
    {
      m_fft->FFT(filter->get_Frame(ii), m_G->get_Frame(ii));
    }

    delete filter;

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR Convolve(void *x, void *y)
  {
    T::type *in = m_x->get_Frame(0);
    copy_n(in + m_MaxInteralFrameSize/2, m_MaxInteralFrameSize/2, in);
    copy_n((T::type*)x, m_MaxFrameSize, in + m_MaxInteralFrameSize/2);

    m_fft->FFT(m_x->get_Frame(0), m_X->get_NextFrame());

    for (uint32_t frame = 0; frame < m_MaxParts; frame++)
    {
      T *X = m_X->get_Frame(frame);
      T *G = m_G->get_Frame(frame);
      T *Y = m_Y->get_Frame(frame);

      for (uint32_t ii = 0; ii < m_MaxVectorFrameSize; ii++)
      {
        Y[ii] = G[ii]*X[ii];
      }
    }

    T *Y0 = m_Y->get_Frame(0);
    for (uint32_t frame = 1; frame < m_MaxParts; frame++)
    {
      T *Y = m_Y->get_Frame(frame);
      for (uint32_t ii = 0; ii < m_MaxVectorFrameSize; ii++)
      {
        Y0[ii] += Y[ii];
      }
    }

    for (uint32_t ii = 0; ii < m_MaxVectorFrameSize; ii++)
    {
      Y0[ii].Scale(m_ScaleValue);
    }

    m_fft->iFFT(Y0, m_y->get_Frame(0));

    copy_n(m_y->get_Frame(0) + m_MaxInteralFrameSize/2, m_MaxFrameSize, (T::type*)y);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR Destroy()
  {
    ASPLIB_FACTORY_FFT.Destroy(m_fft);

    delete m_G; m_G = nullptr;
    delete m_X; m_X = nullptr;
    delete m_Y; m_Y = nullptr;
    delete m_y; m_y = nullptr;
    delete m_x; m_x = nullptr;

    return ASPLIB_ERR_NO_ERROR;
  }

private:
  IFFT *m_fft;

  uint32_t m_MaxParts;
  uint32_t m_MaxFrameSize;
  uint32_t m_MaxInteralFrameSize;
  uint32_t m_MaxVectorFrameSize;
  typename T::type m_ScaleValue;

  TFrameBuffer<T>       *m_G;
  TFrameRingBuffer<T>   *m_X;
  TFrameBuffer<T>       *m_Y;
  TFrameBuffer<typename T::type> *m_y;
  TFrameBuffer<typename T::type> *m_x;
};
}
