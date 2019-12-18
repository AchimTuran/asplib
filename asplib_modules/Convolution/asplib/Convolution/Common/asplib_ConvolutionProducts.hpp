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



#include "Core/os/asplib_os.h"
#include "Core/Constants_Typedefs/asplib_Typedefs.h"

#include "Convolution/asplib_ConvolutionFactory.hpp"
#include "Convolution/TConvolution/TConvolution.hpp"
#include "Core/ComplexVector/asplib_ComplexVector_NativeFloat.hpp"
#include "Core/ComplexVector/asplib_ComplexVector_SSE3Float.hpp"
#include "Core/ComplexVector/asplib_ComplexVector_AVXFloat.hpp"
//! @todo reserved for future optimizations
//#include "Core/ComplexVector/asplib_ComplexVector_AVX512Float.hpp"
//#include "Core/ComplexVector/asplib_ComplexVector_NEONFloat.hpp"


namespace asplib
{
  CREATE_ASPLIB_CONVOLUTION_CLASS(CConvolution_NativeFloat, ASPLIB_Convolution_NativeFloat, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, TConvolution<CComplexVector_NativeFloat>);
  CREATE_ASPLIB_CONVOLUTION_CLASS(CConvolution_SSE3Float, ASPLIB_Convolution_SSE3Float, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, TConvolution<CComplexVector_SSE3Float>);
  //! @todo reserved for future optimizations
  //CREATE_ASPLIB_CONVOLUTION_CLASS(CConvolution_AVXFloat, ASPLIB_Convolution_AVXFloat, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, TConvolution<CComplexVector_AVXFloat>);
  //CREATE_ASPLIB_CONVOLUTION_CLASS(CConvolution_NEONFloat, ASPLIB_Convolution_NEONFloat, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, TConvolution<CComplexVector_NEONFloat>);
}
