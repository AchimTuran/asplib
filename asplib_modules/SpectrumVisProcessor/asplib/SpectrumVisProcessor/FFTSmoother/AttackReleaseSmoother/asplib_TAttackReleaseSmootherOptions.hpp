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
#include "Core/AutoFactory/asplib_TAutoFactory.hpp"
#include "Core/ExtendedStructs/asplib_ExtendedStructs.hpp"


namespace asplib
{
struct AttackReleaseSmootherOptions : ASPLIB_EXTENDED_STRUCT(AttackReleaseSmootherOptions)
{
  long double ReleaseTime;    // release time in seconds for smoothing values
  long double AttackTime;     // release time in seconds for smoothing values
  bool        UseAnalogConst; // set this to true if you want do use the analog release time definition 
                              // or to false if you want to use the digital release time definition

  AttackReleaseSmootherOptions() : ASPLIB_EXTENDED_STRUCT_REGISTER(ASPLIB_EXTENDED_STRUCT_TAttackReleaseSmootherOptions)
  {
    ReleaseTime    = 0.000000001;   // default value is 10ms
    AttackTime     = 0.00005;  // default value is 1ms
    UseAnalogConst = false;  // default is false to use the digital release time definition
  }
};
}
