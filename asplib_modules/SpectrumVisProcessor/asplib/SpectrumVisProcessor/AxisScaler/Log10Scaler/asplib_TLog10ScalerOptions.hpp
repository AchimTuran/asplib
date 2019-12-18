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
struct TLog10ScalerOptions : ASPLIB_EXTENDED_STRUCT(TLog10ScalerOptions)
{
  long double ScaleVal;         /*!< @brief dB scaling value. Most dB definitions use 20.0 */
  long double InftyApprox;      /*!< @brief log(0.0) --> is - infinity. This values clamps to value to InftyApprox dB. */
  long double dBFS_ScalingVal;  /*!< @brief Set this value manually to scale the linear values. */
  bool        use_dBFS_Scaling; /*!< @brief If true the SpectrumVisProcessor scales the linear values. This results in dBFS scaling (0dB ... -Infinity dB). */

  /*!
   * @brief Calculates the normalization (2.0/OutputFFTFrameSize) value for dBFS.
   *        For more informations please read: http://www.kvraudio.com/forum/viewtopic.php?t=276092
   *
   * @param FrameSize The corresponding FFT output frame size.
   * @remarks A frame size with 0 will return the error code ASPLIB_ERR_INVALID_INPUT, because division by 0 is not allowed.
   * @return asplib error code. For more details see ASPLIB_ERR.
   */
  ASPLIB_ERR Calc_dBFS_ScalingValue(uint32_t FrameSize)
  {
    if (FrameSize == 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    dBFS_ScalingVal = 2.0/FrameSize;
    return ASPLIB_ERR_NO_ERROR;
  }

  TLog10ScalerOptions() : ASPLIB_EXTENDED_STRUCT_REGISTER(ASPLIB_EXTENDED_STRUCT_TLog10ScalerOptions)
  {
    ScaleVal         = 20.0;
    InftyApprox      = -150.0;
    dBFS_ScalingVal  = 1.0;
    use_dBFS_Scaling = false;
  }
};
}
