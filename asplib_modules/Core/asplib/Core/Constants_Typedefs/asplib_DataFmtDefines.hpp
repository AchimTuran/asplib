#pragma once

/* Copyright (C) 2014-2016 Achim Turan, mail@achim-turan.de
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



namespace asplib
{
typedef enum
{
  ASPLIB_FMT_INVALID = -1,

  // native formats
  ASPLIB_FMT_NATIVE_FLOAT,                // float
  ASPLIB_FMT_NATIVE_CPX_FLOAT,            // float[2]
  ASPLIB_FMT_NATIVE_CPX_SPLIT_FLOAT,      // float*, float*
    
  // optimized formats
  ASPLIB_FMT_SSE3_CPX_FLOAT,              // TODO: description
  ASPLIB_FMT_AVX_CPX_FLOAT,               // TODO: description
  ASPLIB_FMT_CU_CPX_FLOAT,                // TODO: description
  ASPLIB_FMT_OpenCL_CPX_FLOAT,            // TODO: description

  ASPLIB_FMT_MAX
}asplibFmt_t;

typedef float asplibFmt_NativeFloat;

typedef struct asplibFmt_NativeCPXFloat
{
  float r;
  float i;
  inline asplibFmt_NativeCPXFloat(float R, float I) : r(R), i(I) {}
  inline asplibFmt_NativeCPXFloat() : r(0.0f), i(0.0f) {}
}asplibFmt_NativeCPXFloat;
}
