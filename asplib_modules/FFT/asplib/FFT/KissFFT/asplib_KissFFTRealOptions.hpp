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

#include "Core/ExtendedStructs/asplib_ExtendedStructs.hpp"


namespace asplib
{
struct KissFFT_RealOptions : ASPLIB_EXTENDED_STRUCT(KissFFT_RealOptions)
{
  bool symmetricSpectrum; // Kiss FFT real only outputs the positive spectrum which is from 0 ... N/2 +1,
                          // but some applications require (like convolution) the full spectrum from 0 ... N-1.
                          // This option mirrors the positive spectrum and maps it to N/2 +1 ... N-1.
  
  bool enableZeroPadding; // This options lets Kiss FFT real choose a frame size that is a power of two.
                          // This is achieved by inserting zeros at the end of the array, 
                          // which will have the highest performance. The resulting spectrum will have this frame size.

  KissFFT_RealOptions() : ASPLIB_EXTENDED_STRUCT_REGISTER(ASPLIB_EXTENDED_STRUCT_KissFFTRealOptions)
  {
    symmetricSpectrum = false;
    enableZeroPadding = false;
  }
};
}
