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


namespace asplib
{
class IFFT
{
public:

  IFFT() {}

  virtual ~IFFT() {}

  inline int32_t InFrameSize()
  {
    return m_InFrameSize;
  }

  inline int32_t InternalFrameSize()
  {
    return m_InternalFrameSize;
  }

  inline int32_t OutFrameSize()
  {
    return m_OutFrameSize;
  }

  ASPLIB_ERR Create(int32_t FrameSize, void *Options = NULL)
  {
    if (FrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    m_InFrameSize       = FrameSize;
    m_InternalFrameSize = FrameSize;
    m_OutFrameSize      = FrameSize;
    return CreateFFTLib(Options);
  }
  
  virtual ASPLIB_ERR FFT(void *In, void *Out) = 0;
  virtual ASPLIB_ERR iFFT(void *In, void *Out) = 0;
  virtual ASPLIB_ERR FFT(void *RIn, void *CIn, void *Out) = 0;
  virtual ASPLIB_ERR iFFT(void *In, void *ROut, void *COut) = 0;

  virtual ASPLIB_ERR Destroy() = 0;

protected:
  virtual ASPLIB_ERR CreateFFTLib(void *Options = NULL) = 0;
  uint32_t m_InFrameSize;
  uint32_t m_InternalFrameSize;
  uint32_t m_OutFrameSize;
};
}
