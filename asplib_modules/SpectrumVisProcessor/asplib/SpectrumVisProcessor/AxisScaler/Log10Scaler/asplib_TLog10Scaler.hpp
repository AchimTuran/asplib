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
#include "SpectrumVisProcessor/AxisScaler/Log10Scaler/asplib_TLog10ScalerOptions.hpp"


namespace asplib
{
template<typename T>
class TLog10Scaler : public IProcess
{
public:
  TLog10Scaler() :
    IProcess(IProcess::PROCESS_AXIS_SCALER)
  {
    m_ScaleVal      = 20;
    m_FrameSize     = 0;
    m_InftyApprox   = -150;
    m_dBFS_ScaleVal = 1;
    m_dBFS_Scaling  = false;
  }


  virtual ASPLIB_ERR Create(uint32_t FrameSize, void *Options = NULL)
  {
    if (FrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    if (Options)
    {
      CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Options);
      if (extendedStruct->ID != ASPLIB_EXTENDED_STRUCT_TLog10ScalerOptions)
      {
        return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT;
      }

      TLog10ScalerOptions *options = static_cast<TLog10ScalerOptions*>(Options);

      m_ScaleVal     = static_cast<T>(options->ScaleVal);
      m_InftyApprox  = static_cast<T>(options->InftyApprox);
      if (options->dBFS_ScalingVal != 1.0)
      {
        m_dBFS_Scaling = true;
        m_dBFS_ScaleVal = static_cast<T>(options->dBFS_ScalingVal);
      }
    }

    m_FrameSize = FrameSize;

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Process(void *In, void *Out)
  {
    T *input = (T*)In;
    T *output = (T*)Out;

    if (m_dBFS_Scaling)
    {
      for (uint32_t ii = 0; ii < m_FrameSize; ii++)
      {
        if (input[ii] <= 0)
        {
          output[ii] = m_InftyApprox;
        }
        else
        {
          output[ii] = m_ScaleVal*log10(input[ii]*m_dBFS_ScaleVal);
        }
      }
    }
    else
    {
      for (uint32_t ii = 0; ii < m_FrameSize; ii++)
      {
        if (input[ii] <= 0)
        {
          output[ii] = m_InftyApprox;
        }
        else
        {
          output[ii] = m_ScaleVal*log10(input[ii]);
        }
      }
    }

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Destroy()
  {
    m_ScaleVal      = 20;
    m_FrameSize     = 0;
    m_InftyApprox   = -150;
    m_dBFS_ScaleVal = 1;
    m_dBFS_Scaling  = false;

    return ASPLIB_ERR_NO_ERROR;
  }


private:
  uint32_t  m_FrameSize;
  T         m_ScaleVal;
  T         m_InftyApprox;
  T         m_dBFS_ScaleVal;
  bool      m_dBFS_Scaling;
};
}
