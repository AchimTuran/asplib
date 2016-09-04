#pragma once

/* Copyright (C) 2014-2016 Achim Turan, Achim.Turan@o2online.de
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
#include "Core/Constants_Typedefs/asplib_Constants.h"
#include "Core/ExtendedStructs/asplib_ExtendedStructs.hpp"


namespace asplib
{
struct CompressorOptions : ASPLIB_EXTENDED_STRUCT(CompressorOptions)
{
  typedef enum
  {
    COMPRESSION_INVALID = -1,
    
    COMPRESSION_HARD_CLIPPING,
    COMPRESSION_SOFT_CLIPPING,
    COMPRESSION_TANH,

    COMPRESSION_MAX
  } eGainCurve_t;

  // todo mtk2003 Doxygen documentation
  long double tauRelease;            // [s]
  long double tauAttack;             // [s]
  long double threshold;             // [dB] = 20*log10
  long double compressionRatio;      // compression characteristic
  long double kneeWidth;             // [dB] = 20*log10
  eGainCurve_t gainCurve;

  CompressorOptions() : ASPLIB_EXTENDED_STRUCT_REGISTER(ASPLIB_EXTENDED_STRUCT_CompressorOptions)
  {
    tauRelease        = 4.0*E_m3;
    tauAttack         = 1.0*E_m3;
    threshold         = -3;
    compressionRatio  = 10.0;
    kneeWidth         = 0.0;
    gainCurve         = COMPRESSION_HARD_CLIPPING;
  }
};
}
