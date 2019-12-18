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

#include "SpectrumVisProcessor/SpectrumCalculation/asplib_SpectrumCalculationFactory.hpp"

#include "SpectrumVisProcessor/SpectrumCalculation/SquareRootSpectrumCalculation/asplib_TSquareRoot.hpp"
#include "SpectrumVisProcessor/SpectrumCalculation/SquareSpectrumCalculation/asplib_TSquare.hpp"


namespace asplib
{
typedef TSquare<float> SquareSpectrumCalculationNativeFloat_t;
CREATE_ASPLIB_SPECTRUM_CALCULATION_CLASS(SquareSpectrumCalculationNativeFloat, ASPLIB_SPECTRUM_CALCULATION_Square, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, SquareSpectrumCalculationNativeFloat_t)

typedef TSquareRoot<float> SquareRootSpectrumCalculationNativeFloat_t;
CREATE_ASPLIB_SPECTRUM_CALCULATION_CLASS(SquareRootSpectrumCalculationNativeFloat, ASPLIB_SPECTRUM_CALCULATION_SquareRoot, ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, SquareRootSpectrumCalculationNativeFloat_t)
}
