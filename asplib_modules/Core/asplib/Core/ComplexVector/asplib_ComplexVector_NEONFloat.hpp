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

#if defined(ASPLIB_NEON)
  #include <arm_neon.h>
#else
  #include <NEON2SSE/NEON_2_SSE.h>
#endif


namespace asplib
{
class CComplexVector_NEONFloat;

template <>
struct complex_vector_type<CComplexVector_NEONFloat>
{
  typedef float complex_type;
};


class CComplexVector_NEONFloat : public TComplexVector<CComplexVector_NEONFloat>
{
public:
  inline CComplexVector_NEONFloat() : m_Value(setFloat4(0.0f, 0.0f, 0.0f, 0.0f))
  {
  }
  inline CComplexVector_NEONFloat(const float& R1, const float& R2) : m_Value(setFloat4(R1, 0.0f, R2, 0.0f))
  {
  }
  inline CComplexVector_NEONFloat(const float& R1, const float& I1, const float& R2, const float& I2) : m_Value(setFloat4(R1, I1, R2, I2)) {}
  inline CComplexVector_NEONFloat(const float32x4_t& rhs) : m_Value(rhs) {}

  inline void Scale(const float& Val) 
  {
    m_Value = vmulq_n_f32(m_Value, Val);
  }

  inline CComplexVector_NEONFloat& operator=(const float32x4_t& rhs)
  {
    m_Value = rhs;
    return *this;
  }

  static const uint32_t    vectorSize;
  static const asplibFmt_t typeID;

  inline operator float32x4_t() const { return m_Value; }

private:
  float32x4_t m_Value;

  inline float32x4_t setFloat4(const float& R1, const float& I1, const float& R2, const float& I2)
  {
#if defined(TARGET_WINDOWS) && !defined(ASPLIB_NEON)
    __declspec(align(16)) float
#else
    float __attribute__((aligned(16)))
#endif
    value[4] = { R1, I1, R2, I2 };
    return vld1q_f32(value);
  }
};

const uint32_t    CComplexVector_NEONFloat::vectorSize = 2;
const asplibFmt_t CComplexVector_NEONFloat::typeID = ASPLIB_FMT_NEON_CPX_FLOAT;

inline CComplexVector_NEONFloat operator+(const CComplexVector_NEONFloat& LHS, const CComplexVector_NEONFloat& RHS)
{
  return vaddq_f32(LHS, RHS);
}

inline CComplexVector_NEONFloat operator-(const CComplexVector_NEONFloat& LHS, const CComplexVector_NEONFloat& RHS)
{
  return vsubq_f32(LHS, RHS);
}

inline CComplexVector_NEONFloat operator*(const CComplexVector_NEONFloat& LHS, const CComplexVector_NEONFloat& RHS)
{
  // todo
  // add/subtract, scale and store operations
  // duplicate values
  // _A1 = _mm_moveldup_ps: src{ a1, b1, a2, b2 } --> val2 { a1, a1, a2, a2 }
  // _A2 = _mm_movehdup_ps: src{ a1, b1, a2, b2 } --> val3 { b1, b1, b2, b2 }
  // a = calc { a1*c1, a1*d1, a2*c2, a2*d2 } --> sse3 multiply
  // b = reorder im and re numbers { c1, d1, c2, d2 } --> { d1, c1, d2, c2 } and multiply { b1*d1, b1*c1, b2*d2, b2*c2 }
  // A = _mm_addsub_ps: ret { a1*c1 - b1*d1, j(a1*d1 + b1*c1), a2*c2 - b2*d2, j(a2*d2 + b2*c2) }
  // _mm_store_ps: C[0] = result0, C[1] = result1, C[2] = result2, C[3] = result3
  //return _mm_addsub_ps(_mm_mul_ps(_mm_moveldup_ps(LHS), RHS), _mm_mul_ps(_mm_movehdup_ps(LHS), _mm_shuffle_ps(RHS, RHS, _MM_SHUFFLE(2, 3, 0, 1))));
  return CComplexVector_NEONFloat();
}

inline CComplexVector_NEONFloat operator*(const CComplexVector_NEONFloat& LHS, const float& Val)
{
  return vmulq_n_f32(LHS, Val);
}

inline CComplexVector_NEONFloat operator/(const CComplexVector_NEONFloat& LHS, const CComplexVector_NEONFloat& RHS)
{
  // todo
  return CComplexVector_NEONFloat();
}
}
