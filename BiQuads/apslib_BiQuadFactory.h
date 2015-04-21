#pragma once
/*
 * Copyright (C) 2014 Achim Turan, Achim.Turan@o2online.de
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



// this module implements basic biquad filters 
// for more details see: http://en.wikipedia.org/wiki/Digital_biquad_filter

#include "../constants_typedefs/asplib_constants.h"
#include "../constants_typedefs/asplib_typedefs.h"

namespace asplib
{
class CBiQuadFactory
{
public:
    // Handle BiQuads
    static ASPLIB_ERR            reset_BiQuads(ASPLIB_BIQUAD_HANDLE *BiQuads);
    static ASPLIB_ERR            destroy_BiQuads(ASPLIB_BIQUAD_HANDLE **BiQuads);
    static uint                  get_maxBiQuads(ASPLIB_BIQUAD_HANDLE *BiQuads);
    static ASPLIB_BIQUAD_HANDLE* get_BiQuads(uint BiQuadQuantity, float SampleFrequency, ASPLIB_OPT_MODULE OptModule);
    static ASPLIB_ERR            calc_BiQuadSample(ASPLIB_BIQUAD_HANDLE *BiQuads, float In, float *Out);
    static ASPLIB_ERR            calc_BiQuadSamples(ASPLIB_BIQUAD_HANDLE *BiQuads, float *In, float *Out, uint FrameSize);

    // set BiQuad Parameters
    static ASPLIB_ERR            set_BiQuadCoefficients(ASPLIB_BIQUAD_HANDLE *BiQuads, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, float C0 = 1.0f, float D0 = 0.0f);
    static ASPLIB_ERR            set_BiQuadCoefficients(ASPLIB_BIQUAD_HANDLE *BiQuads, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, uint BiQuadIdx, float C0 = 1.0f, float D0 = 0.0f);
    // Octave EQs, 1 Octave (9 to 11 bands), 2/3 octave (15 - 17 bands),  1/2 octave (17 to 22 bands), and 1/3 octave (30 - 31 bands)
    // frequency bands are defined in ISO R 266-1997 or ANSI S1.6-1984
    static ASPLIB_ERR            set_constQPeakingParams(ASPLIB_BIQUAD_HANDLE *BiQuads, float Gain);
    static ASPLIB_ERR            set_constQPeakingParams(ASPLIB_BIQUAD_HANDLE *BiQuads, float Gain, uint BiQuadIdx);
    static ASPLIB_ERR            get_constQPeakingBiQuadCoes(ASPLIB_BIQUAD_HANDLE *BiQuads, float Gain, uint BiQuadIdx, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients);
	static ASPLIB_ERR            get_constQPeakingBiquadCoes(uint SampleFrequency, uint MaxFreqBands, float Gain, uint BiquadIdx, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients);
};
}
