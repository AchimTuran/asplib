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
#include "SpectrumVisProcessor/Remapper/GammaCorrectorRemapper/asplib_TGammaCorrectorOptions.hpp"


namespace asplib
{
template<typename T>
class TGammaCorrector : public IProcess
{
public:
  TGammaCorrector() :
    IProcess(IProcess::PROCESS_REMAPPER)
  {
    m_FrameSize       = 0;
    m_OutputFrameSize = 0;
  }


  virtual ASPLIB_ERR SetOutputFrameSize(uint32_t OutputFrameSize)
  {
    if (OutputFrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }
    m_OutputFrameSize = OutputFrameSize;

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Create(uint32_t FrameSize, void *Options = NULL)
  {
    if (FrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }
    m_FrameSize = FrameSize;
    if (m_OutputFrameSize <= 0)
    {
      m_OutputFrameSize = m_FrameSize;
    }

    if (Options)
    {
      CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Options);
      if (extendedStruct->ID != ASPLIB_EXTENDED_STRUCT_TGammaCorrectorOptions)
      {
        return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT;
      }

      TGammaCorrectorOptions *options = static_cast<TGammaCorrectorOptions*>(Options);

      m_Gamma = options->Gamma;
    }
    else
    {
      TGammaCorrectorOptions options;
      m_Gamma = options.Gamma;
    }

    m_RemapperValues = new uint32_t[m_OutputFrameSize*2];
    long double *tmpValues = new long double[m_OutputFrameSize];
    
    const long double num = 1.0 / (long double)m_OutputFrameSize;
    const long double invGamma = 1.0 / m_Gamma;
    for (uint32_t ii = 0; ii < m_OutputFrameSize; ii++)
    {
      tmpValues[ii] = (pow(((long double)ii)*num, invGamma)*((long double)m_FrameSize));
    }

    for (uint32_t ii = 0; ii < m_OutputFrameSize -1; ii++)
    {
      m_RemapperValues[ii*2]    = (uint32_t)(tmpValues[ii] + (long double)0.5);
      m_RemapperValues[ii*2 +1] = (uint32_t)(tmpValues[ii+1]);
    }
    m_RemapperValues[m_OutputFrameSize*2 -2] = tmpValues[m_OutputFrameSize -1];
    m_RemapperValues[m_OutputFrameSize*2 -1] = m_FrameSize;

    delete tmpValues;
    tmpValues = NULL;

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Process(void *In, void *Out)
  {
    T *input = (T*)In;
    T *output = (T*)Out;
    // TODO implement max search for a specific frequency band
    for (uint32_t ii = 0; ii < m_OutputFrameSize; ii++)
    {
      T maxVal = 0;
      for (uint32_t jj = m_RemapperValues[ii*2]; jj < m_RemapperValues[ii*2 +1]; jj++)
      {
        if (input[jj] > maxVal)
        {
          maxVal = input[jj];
        }
      }

      output[ii] = maxVal;
    }

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Destroy()
  {
    m_FrameSize       = 0;
    m_Gamma           = 2.0;

    if (m_RemapperValues)
    {
      delete m_RemapperValues;
      m_RemapperValues = NULL;
    }

    return ASPLIB_ERR_NO_ERROR;
  }


private:
  uint32_t    *m_RemapperValues;
  uint32_t    m_FrameSize;
  uint32_t    m_OutputFrameSize;
  long double m_Gamma;
};
}
