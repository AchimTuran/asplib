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



#include "asplib_utils/os/asplib_base_os.h"
#include "asplib_utils/exceptions/asplib_StringException.h"
#include "ITFrameBuffer.h"

namespace asplib
{
template<typename T>
class TFrameRingBuffer : ITFrameBuffer<T>
{
  public:
    TFrameRingBuffer(int MaxFrameLength, int MaxFrames, uint Alignment=0) :
      ITFrameBuffer<T>(MaxFrameLength, MaxFrames, Alignment)
    {
      m_IsEmpty = true;
      m_CurrentFrame = get_MaxFrames();
    }

    T* get_Frame(uint32_t Frame)
    {
      if(Frame >= get_MaxFrames() || m_IsEmpty)
      {
        return NULL;
      }

      int32_t tempFrame = m_CurrentFrame - (int32_t)Frame;
      if(tempFrame < 0)
      {
        tempFrame = m_CurrentFrame + get_MaxFrames() - (int32_t)Frame;
      }

      return m_Buffer + tempFrame*get_MaxFrameLength();
    }

    T* get_NextFrame()
    {
      m_CurrentFrame++;
      if(m_CurrentFrame >= m_MaxFrames)
      {
        m_CurrentFrame = 0;
      }

      if(m_IsEmpty)
      {
        m_IsEmpty = false;
      }

      return m_Buffer + m_CurrentFrame*m_MaxFrameLength;
    }

    bool      m_IsEmpty;
};
}
