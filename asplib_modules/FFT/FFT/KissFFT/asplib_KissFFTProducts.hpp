#pragma once

/* Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
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

#include "KissFFT/asplib_TKissFFT.hpp"
#include "KissFFT/asplib_TRKissFFT.hpp"
#include "common/asplib_TZeroCpyConverter.hpp"

#include "asplib_FFTFactory.hpp"


namespace asplib
{
// complex Kiss FFT
typedef TKissFFT <TZeroCpyConverter<kiss_fft_cpx, asplibFmt_NativeCPXFloat>,
                  TZeroCpyConverter<kiss_fft_cpx, asplibFmt_NativeCPXFloat>,
                  TZeroCpyConverter<kiss_fft_cpx, asplibFmt_NativeCPXFloat>,
                  TZeroCpyConverter<kiss_fft_cpx, asplibFmt_NativeCPXFloat> > CKissFFT_Cpx;
CREATE_ASPLIB_FFT_CLASS(CKissFFT_NativeCPXFloat, ASPLIB_FFT_KISS_FFT_CPX, ASPLIB_FMT_NATIVE_CPX_FLOAT, ASPLIB_FMT_NATIVE_CPX_FLOAT, CKissFFT_Cpx)


// real Kiss FFT
typedef TRKissFFT<TZeroCpyConverter<float, float>,
                  TZeroCpyConverter<float, float>,
                  TZeroCpyConverter<kiss_fft_cpx, asplibFmt_NativeCPXFloat>,
                  TZeroCpyConverter<kiss_fft_cpx, asplibFmt_NativeCPXFloat> > CKissFFT_Real;
CREATE_ASPLIB_FFT_CLASS(CKissFFT_NativeFloat, ASPLIB_FFT_KISS_FFT_REAL, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_CPX_FLOAT, CKissFFT_Real)
}
