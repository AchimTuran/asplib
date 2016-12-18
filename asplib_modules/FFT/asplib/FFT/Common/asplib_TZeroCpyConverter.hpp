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
// This template converts (casts) between the fft-library, extern and intern used data format.
// FFT: data format used by the fft-library
// FMT: this indicates the data format that is used by the application
template<typename FFT, typename FMT>
class TZeroCpyConverter
{
public:
  // convert FFT to FMT
  inline void fmtc_FFT_TO_Fmt(void *Fmt)
  {// not used by zero copy converter
  }

  // convert FMT to FFT
  inline FFT* operator () (void *Fmt)
  {
    m_FFTPtr = static_cast<FFT*>(Fmt);
    return m_FFTPtr;
  }

  inline FFT* GetFFTPtr()
  {
    return m_FFTPtr;
  }

  inline void fmtc_FFT_TO_SplitFmt(void *Extern1, void *Extern2)
  {
  }

  // TODO: implement functionality for split arrays (real and complex part are separate pointers)
  inline FFT* operator () (void *Fmt1, void *Fmt2)
  {
    return static_cast<FFT*>(Fmt1);
  }

  // create converter
  inline ASPLIB_ERR Create(int InFrameSize, int OutFrameSize, void *Options = NULL)
  {// not used by zero copy converter
    return ASPLIB_ERR_NO_ERROR;
  }

  // destroy converter
  inline ASPLIB_ERR Destroy()
  {// not used by zero copy converter
    return ASPLIB_ERR_NO_ERROR;
  }

private:
  FFT *m_FFTPtr;
};
}
