#pragma once

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



#include "Core/os/asplib_os.h"
#include "Core/Constants_Typedefs/asplib_Typedefs.h"

#include "SpectrumVisProcessor/Interfaces/asplib_IProcess.hpp"
#include "SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_dlbeerSmootherOptions.hpp"


namespace asplib
{
  // this implementation is based on the ideas from http://dlbeer.co.nz/articles/fftvis.html
template<typename T, class TSmoother>
class TdlbeerFFTSmoother : public IProcess
{
public:
  TdlbeerFFTSmoother() :
    IProcess(IProcess::PROCESS_FFT_SMOOTHER)
  {
    m_FilterCoe = 0.0;
    m_SmoothConstant = 1.0;
    m_SliceRate = 1.0;
    m_Smoother = NULL;
    m_FrameSize = 0;
  }

  virtual ASPLIB_ERR Create(uint32_t FrameSize, void *Options)
  {
    if (FrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    if (Options)
    {
      CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Options);
      if (extendedStruct->ID != ASPLIB_EXTENDED_STRUCT_dlbeerSmootherOptions)
      {
        return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT;
      }

      dlbeerSmootherOptions *options = static_cast<dlbeerSmootherOptions*>(Options);
      if (options->SliceRate == 0.0)
      {
        return ASPLIB_ERR_INVALID_INPUT;
      }

      m_SliceRate       = options->SliceRate;
      m_SmoothConstant  = options->SmoothConstant;
    }
    else
    {
      m_SliceRate = 10.0;
      m_SmoothConstant = 0.1;
    }

    m_FrameSize = FrameSize;
    m_Smoother  = new TSmoother[m_FrameSize];

    m_FilterCoe = (T)pow(m_SmoothConstant, (long double)1.0 / m_SliceRate);
    for (uint32_t ii = 0; ii < m_FrameSize; ii++)
    {
      m_Smoother[ii].SetCoefficient(m_FilterCoe);
    }

    return ASPLIB_ERR_NO_ERROR;
  }
  
  virtual ASPLIB_ERR Process(void *In, void *Out)
  {
    T *in = static_cast<T*>(In);
    T *out = static_cast<T*>(Out);

    for (uint32_t ii = 0; ii < m_FrameSize; ii++)
    {
      out[ii] = m_Smoother[ii].Filter(in[ii]);
    }

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR Destroy()
  {
    if (m_Smoother)
    {
      delete [] m_Smoother;
      m_Smoother = NULL;
    }

    return ASPLIB_ERR_NO_ERROR;
  }

private:
  T           m_FilterCoe;
  long double m_SmoothConstant;
  long double m_SliceRate;
  TSmoother   *m_Smoother;
  uint32_t    m_FrameSize;
};
}
