#pragma once

/*
 * Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
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
class TFrameBuffer : public ITFrameBuffer<T>
{
public:
  TFrameBuffer(uint32_t MaxFrameLength, uint32_t MaxFrames, uint16_t Alignment=0) :
    ITFrameBuffer<T>(MaxFrameLength, MaxFrames, Alignment)
  {
    m_CurrentFrame = 0;
  }

  T *get_Frame(int Frame)
  {
    if(Frame < 0 || Frame >= get_MaxFrames())
    {
      return NULL;
    }

    return m_Buffer + Frame*get_MaxFrameLength();
  }

  T *get_NextFrame()
  {
    uint32_t returnFrame = m_CurrentFrame;
    m_CurrentFrame++;
    if(m_CurrentFrame >= get_MaxFrames())
    {
      m_CurrentFrame = 0;
    }

    return m_Buffer + returnFrame*get_MaxFrameLength();
  }

  virtual void  reset_CurrentFrameIdx()
  {
    m_CurrentFrame = 0;
  }
};
}
