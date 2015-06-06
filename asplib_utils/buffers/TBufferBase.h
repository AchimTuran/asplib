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
 * FOR A PARTICprotectedULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with asplib (Achim's Signal Processing LIBrary).
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



#include "asplib_utils/os/asplib_base_os.h"
#include "asplib_utils/exceptions/asplib_StringException.h"
#include "IBufferBase.h"
#if defined(TARGET_WINDOWS) || defined(TARGET_LINUX)
  #include <xmmintrin.h>
#endif

namespace asplib
{
template<typename T>
class TBufferBase : public IBufferBase<T>
{
public:
  TBufferBase(uint32_t MaxFrameLength, uint32_t MaxFrames, uint32_t Alignment=0) :
    IBufferBase<T>(MaxFrameLength, MaxFrames, Alignment)
  {
    Create();
  }

  virtual ~TBufferBase()
  {
    Destroy();
  }

  virtual void ResetBuffer()
  {
    memset(m_Buffer, 0, sizeof(T)*get_MaxFrameLength()*get_MaxFrames());
  }

private:
  virtual void Create()
  {
    if(get_Alignment() > 0)
    {
#if defined(TARGET_WINDOWS)
      m_Buffer = (T*)_aligned_malloc(sizeof(T)*get_MaxFrameLength()*get_MaxFrames(), (size_t)get_Alignment());
#elif defined(TARGET_LINUX)
      m_Buffer = (T*)_mm_malloc(sizeof(T)*get_MaxFrameLength()*get_MaxFrames(), (size_t)get_Alignment());
#else
      //#warning "Unsupported platform requested aligned memory, but this functionailty is not available"
      m_Buffer = new T[get_MaxFrameLength()*get_MaxFrames()];
#endif
    }
    else
    {
      m_Buffer = new T[get_MaxFrameLength()*get_MaxFrames()];
    }

    if(!m_Buffer)
    {
      throw ASPLIB_STRING_EXCEPTION_HANDLER("Couldn't create buffer! Not enough free dynamic memory?");
    }

    ResetBuffer();
  }

  virtual void Destroy()
  {
    if(m_Buffer)
    {
      if(get_Alignment() > 0)
      {
  #if defined(TARGET_WINDOWS)
        _aligned_free(m_Buffer);
  #else
        _mm_free(m_Buffer);
  #endif
        }
        else
        {
          delete [] m_Buffer;
        }

      m_Buffer = NULL;
    }
  }

};
}
