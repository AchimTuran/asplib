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



#include "asplib_utils/os/asplib_base_os.h"
#include "asplib_utils/constants_typedefs/asplib_typedefs.h"

#include "SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TdlbeerSmoother.hpp"

#include "SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.hpp"
#include "SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TFeedbackSmoother.hpp"


namespace asplib
{
// dlbeer link! spectrum smoother
typedef TdlbeerFFTSmoother<float, TdlbeerFeedbackTimeSmoother<float> > dlbeerFFTSmootherNativeFloat_t;
CREATE_ASPLIB_FFT_SMOOTHER_CLASS(CdlbeerFFTSmoother_NativeFloat, ASPLIB_FFT_SMOOTHER_dlbeer, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, dlbeerFFTSmootherNativeFloat_t)
}
