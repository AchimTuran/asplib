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
class CFFTWindowFunctions
{
public:
  typedef enum eFFT_WINDOW_ID
  {
    FFT_WINDOW_ID_INVALID = -1,

    FFT_WINDOW_ID_HAMMING,
    FFT_WINDOW_ID_HANN,
    FFT_WINDOW_ID_BLACKMAN,
    
    FFT_WINDOW_ID_BLACKMAN_HARRIS,
    // reserved for future versions
    // WINDOW_ID_BARTLETT,
    
    FFT_WINDOW_ID_MAX
  }eFFT_WINDOW_ID;

  CFFTWindowFunctions();
  ~CFFTWindowFunctions();

  ASPLIB_ERR Init(uint32_t FrameSize, eFFT_WINDOW_ID FFTWindowID);
  ASPLIB_ERR Process(float *In, float *Out);
  
  int get_FrameSize();
  eFFT_WINDOW_ID get_WindowID();

protected:
  void Destroy();
  ASPLIB_ERR Create();

private:
  double  m_A;
  double  m_B;
  double  m_C;
  double  m_D;

  uint32_t        m_FrameSize;
  float           *m_WindowCoe;
  eFFT_WINDOW_ID  m_WindowID;
};
}
