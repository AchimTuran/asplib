#pragma once

/* Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
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
    m_Gamma = 2.0;
  }


  virtual ASPLIB_ERR Create(uint32_t FrameSize, void *Options = NULL)
  {
    if (FrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }
    m_FrameSize = FrameSize;

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

    m_RemapperValues = new uint32_t[m_FrameSize];
    
    const long double num = 1.0 / (long double)m_FrameSize;
    const long double invGamma = 1.0 / m_Gamma;
    for (uint32_t ii = 0; ii < m_FrameSize; ii++)
    {
      m_RemapperValues[ii] = (uint32_t)(pow(((long double)ii) * num, m_Gamma)*((long double)m_FrameSize));
    }

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Process(void *In, void *Out)
  {
    T *input = (T*)In;
    T *output = (T*)Out;
    for (uint32_t ii = 0; ii < m_FrameSize; ii++)
    {
      output[ii] = input[m_RemapperValues[ii]];
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
  long double m_Gamma;
};
}
