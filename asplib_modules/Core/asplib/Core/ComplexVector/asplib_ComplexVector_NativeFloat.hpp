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


namespace asplib
{
class CComplexVector_NativeFloat;

template <>
struct complex_vector_type<CComplexVector_NativeFloat>
{
  typedef float complex_type;
};


class CComplexVector_NativeFloat : public TComplexVector<CComplexVector_NativeFloat>
{
public:
  inline CComplexVector_NativeFloat() {}
  inline CComplexVector_NativeFloat(const float& f) : m_Value(f, f) {}
  inline CComplexVector_NativeFloat(const float& R, const float& I) : m_Value(R, I) {}
  inline CComplexVector_NativeFloat(const asplibFmt_NativeCPXFloat& rhs) : m_Value(rhs) {}

  inline CComplexVector_NativeFloat& operator=(const asplibFmt_NativeCPXFloat& rhs)
  {
    m_Value = rhs;
    return *this;
  }

  inline operator asplibFmt_NativeCPXFloat() const { return m_Value; }

private:
  asplibFmt_NativeCPXFloat m_Value;
};

inline CComplexVector_NativeFloat operator+(const CComplexVector_NativeFloat& LHS, const CComplexVector_NativeFloat& RHS)
{
  const asplibFmt_NativeCPXFloat &lhs = LHS;
  const asplibFmt_NativeCPXFloat &rhs = RHS;
  
  return CComplexVector_NativeFloat(lhs.r + rhs.r,
                                    lhs.i + rhs.i);
}

inline CComplexVector_NativeFloat operator-(const CComplexVector_NativeFloat& LHS, const CComplexVector_NativeFloat& RHS)
{
  const asplibFmt_NativeCPXFloat &lhs = LHS;
  const asplibFmt_NativeCPXFloat &rhs = RHS;

  return CComplexVector_NativeFloat(lhs.r - rhs.r,
                                    lhs.i - rhs.i);
}

inline CComplexVector_NativeFloat operator*(const CComplexVector_NativeFloat& LHS, const CComplexVector_NativeFloat& RHS)
{
  const asplibFmt_NativeCPXFloat &lhs = LHS;
  const asplibFmt_NativeCPXFloat &rhs = RHS;

  return CComplexVector_NativeFloat(lhs.r*rhs.r - lhs.i*rhs.i,
                                    lhs.r*rhs.i + lhs.i*rhs.r);
}

inline CComplexVector_NativeFloat operator/(const CComplexVector_NativeFloat& LHS, const CComplexVector_NativeFloat& RHS)
{
  const asplibFmt_NativeCPXFloat &lhs = LHS;
  const asplibFmt_NativeCPXFloat &rhs = RHS;

  const float numerator = rhs.r*rhs.r + rhs.i*rhs.i;

  return CComplexVector_NativeFloat((lhs.r*rhs.r + lhs.i*rhs.i)/numerator,
                                    (lhs.r*rhs.i - lhs.r*rhs.i)/numerator);
}
}
