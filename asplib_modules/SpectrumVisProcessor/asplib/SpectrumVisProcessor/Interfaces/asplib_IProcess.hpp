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

#include <vector>


namespace asplib
{
class IProcess
{
public:
  typedef enum ProcessTypes_t
  {
    PROCESS_INVALID_ID = -1,

    PROCESS_FFT_SMOOTHER,
    PROCESS_AXIS_SCALER,    
    PROCESS_REMAPPER,
    PROCESS_SPECTRUM_CALC,
    
    // reserved for future versions
    //PROCESS_FFT_WINDOWING,
    //PROCESS_FFT,

    PROCESS_MAX_ID
  }ProcessTypes_t;

  typedef std::vector<IProcess*> ProcessVector_t;

  IProcess(ProcessTypes_t Type)
  {
    m_ProcessType = Type;
  }

  virtual ASPLIB_ERR SetOutputFrameSize(uint32_t OutputFrameSize)
  {
    return ASPLIB_ERR_NOT_IMPLEMENTED;
  }
  virtual ASPLIB_ERR Create(uint32_t FrameSize, void *Options) = 0;
  virtual ASPLIB_ERR Process(void *In, void *Out) = 0;
  virtual ASPLIB_ERR Destroy() = 0;

  ProcessTypes_t GetProcessType()
  {
    return m_ProcessType;
  }

private:
  ProcessTypes_t m_ProcessType;
};
}
