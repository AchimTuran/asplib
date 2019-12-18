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


namespace asplib
{
template<typename T>
class TSquare : public IProcess
{
public:
  TSquare() :
    IProcess(PROCESS_SPECTRUM_CALC)
  {
  }


  virtual ASPLIB_ERR Create(uint32_t FrameSize, void *Options = NULL)
  {
    if (FrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }
    m_FrameSize = FrameSize*2;

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Process(void *In, void *Out)
  {
    T *input = (T*)In;
    T *output = (T*)Out;
    for (uint32_t ii = 0; ii < m_FrameSize; ii+=2)
    {
      output[ii] = input[ii]*input[ii] + input[ii + 1]*input[ii + 1];
    }

    return ASPLIB_ERR_NO_ERROR;
  }


  virtual ASPLIB_ERR Destroy()
  {
    m_FrameSize       = 0;

    return ASPLIB_ERR_NO_ERROR;
  }


private:
  uint32_t    m_FrameSize;
};
}
