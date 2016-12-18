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

#include "SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TdlbeerSmoother.hpp"
#include "SpectrumVisProcessor/FFTSmoother/dlbeerSmoother/asplib_TFeedbackSmoother.hpp"

#include "SpectrumVisProcessor/FFTSmoother/AttackReleaseSmoother/asplib_TAttackReleaseSmoother.hpp"
#include "SpectrumVisProcessor/FFTSmoother/AttackReleaseSmoother/asplib_TAttackReleaseFilter.hpp"

#include "SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.hpp"


namespace asplib
{
typedef TdlbeerFFTSmoother<float, TdlbeerFeedbackTimeSmoother<float> > dlbeerFFTSmootherNativeFloat_t;
CREATE_ASPLIB_FFT_SMOOTHER_CLASS(CdlbeerFFTSmoother_NativeFloat, ASPLIB_FFT_SMOOTHER_dlbeer, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, dlbeerFFTSmootherNativeFloat_t)

typedef TAttackReleaseSmoother<float, TAttackReleaseFilter<float> > AttackReleaseFFTSmootherNativeFloat_t;
CREATE_ASPLIB_FFT_SMOOTHER_CLASS(CAttackReleaseFFTSmoother_NativeFloat, ASPLIB_FFT_SMOOTHER_AttackRelease, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, AttackReleaseFFTSmootherNativeFloat_t)
}
