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

#include "Core/ComplexVector/asplib_TComplexVector.hpp"
#include "Core/Constants_Typedefs/asplib_DataFmtDefines.hpp"

#include <zmmintrin.h>


namespace asplib
{
class CComplexVector_AVX512Float;

template <>
struct complex_vector_type<CComplexVector_AVX512Float>
{
  typedef float complex_type;
};


class CComplexVector_AVX512Float : public TComplexVector<CComplexVector_AVX512Float>
{
public:
  inline CComplexVector_AVX512Float() : m_Value(_mm512_setr_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)) {}
  inline CComplexVector_AVX512Float(const float& R1, const float& R2, const float& R3, const float& R4, const float& R5, const float& R6, const float& R7, const float& R8) : m_Value(_mm512_setr_ps(R1, 0.0f, R2, 0.0f, R3, 0.0f, R4, 0.0f, R5, 0.0f, R6, 0.0f, R7, 0.0f, R8, 0.0f)) {}
  inline CComplexVector_AVX512Float(const float& R1, const float& I1, const float& R2, const float& I2, const float& R3, const float& I3, const float& R4, const float& I4, const float& R5, const float& I5, const float& R6, const float& I6, const float& R7, const float& I7, const float& R8, const float& I8) : m_Value(_mm512_setr_ps(R1, I1, R2, I2, R3, I3, R4, I4, R5, I5, R6, I6, R7, I7, R8, I8)) {}
  inline CComplexVector_AVX512Float(const __m512& rhs) : m_Value(rhs) {}

  inline void Scale(const float& Val) 
  {
    __m512 _scale = _mm512_broadcast_ss(&Val);
    m_Value = _mm512_mul_ps(m_Value, _scale);
  }

  inline CComplexVector_AVX512Float& operator=(const __m512& rhs)
  {
    m_Value = rhs;
    return *this;
  }

  static const uint32_t    vectorSize;
  static const asplibFmt_t typeID;

  inline operator __m512() const { return m_Value; }

private:
  __m512 m_Value;
};

const uint32_t    CComplexVector_AVX512Float::vectorSize = 8;
const asplibFmt_t CComplexVector_AVX512Float::typeID = ASPLIB_FMT_AVX_CPX_FLOAT;

inline CComplexVector_AVX512Float operator+(const CComplexVector_AVX512Float& LHS, const CComplexVector_AVX512Float& RHS)
{
  return _mm512_add_ps(LHS, RHS);
}

inline CComplexVector_AVX512Float operator-(const CComplexVector_AVX512Float& LHS, const CComplexVector_AVX512Float& RHS)
{
  return _mm512_sub_ps(LHS, RHS);
}

inline CComplexVector_AVX512Float operator*(const CComplexVector_AVX512Float& LHS, const CComplexVector_AVX512Float& RHS)
{
  // add/subtract, scale and store operations
  // duplicate values
  // _A1 = _mm_moveldup_ps: src{ a1, b1, a2, b2 } --> val2 { a1, a1, a2, a2 }
  // _A2 = _mm_movehdup_ps: src{ a1, b1, a2, b2 } --> val3 { b1, b1, b2, b2 }
  // a = calc { a1*c1, a1*d1, a2*c2, a2*d2 } --> sse3 multiply
  // b = reorder im and re numbers { c1, d1, c2, d2 } --> { d1, c1, d2, c2 } and multiply { b1*d1, b1*c1, b2*d2, b2*c2 }
  // A = _mm_addsub_ps: ret { a1*c1 - b1*d1, j(a1*d1 + b1*c1), a2*c2 - b2*d2, j(a2*d2 + b2*c2) }
  // _mm_store_ps: C[0] = result0, C[1] = result1, C[2] = result2, C[3] = result3
  return _mm512_addsub_ps(_mm512_mul_ps(_mm512_moveldup_ps(LHS), RHS), _mm512_mul_ps(_mm512_movehdup_ps(LHS), _mm512_shuffle_ps(RHS, RHS, _MM_SHUFFLE(2, 3, 0, 1))));
}

inline CComplexVector_AVX512Float operator*(const CComplexVector_AVX512Float& LHS, const float& Val)
{
  __m512 _scale = _mm512_broadcast_ss(&Val);
  return _mm512_mul_ps(LHS, _scale);
}

inline CComplexVector_AVX512Float operator/(const CComplexVector_AVX512Float& LHS, const CComplexVector_AVX512Float& RHS)
{
  // todo
  return CComplexVector_AVX512Float();
}
}
