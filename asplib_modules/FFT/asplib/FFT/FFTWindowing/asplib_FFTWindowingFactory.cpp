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



#include <memory.h>

#include "FFT/FFTWindowing/asplib_FFTWindowingFactory.h"

namespace asplib
{
ASPLIB_ERR CFFTWindowingFactory::CreateFFTWindow(int FrameSize, CFFTWindowFunctions::eFFT_WINDOW_ID FFTWindowID, CFFTWindowFunctions *&FFTWindowFunction)
{
  FFTWindowFunction = new CFFTWindowFunctions();
  ASPLIB_ERR err = FFTWindowFunction->Init(FrameSize, FFTWindowID);
  if(err != ASPLIB_ERR_NO_ERROR)
  {
    DestroyFFTWindow(FFTWindowFunction);
    // TODO: error code
    return err;
  }

  // TODO: error code
  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CFFTWindowingFactory::DestroyFFTWindow(CFFTWindowFunctions *&FFTWindowFunction)
{
  if(FFTWindowFunction)
  {
    delete FFTWindowFunction;
    FFTWindowFunction = NULL;
  }

  // TODO: error code
  return ASPLIB_ERR_NO_ERROR;
}
}
