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


namespace asplib
{
template<class T> struct complex_vector_type;

template<class T>
class TComplexVector
{
public:
  typedef typename complex_vector_type<T>::complex_type value_type;

  // downcast operators
  inline T& operator()() { return *static_cast<T*>(this); }
  inline const T& operator()() const { return *static_cast<const T*>(this); }

  // + assignment operator
  inline T& operator+=(const T& rhs)
  {
      (*this)() = (*this)() + rhs;
      return (*this)();
  }

  inline T& operator+=(const value_type& rhs)
  {
      (*this)() = (*this)() + T(rhs);
      return (*this)();
  }

  // - assignment operator
  inline T& operator-=(const T& rhs)
  {
    (*this)() = (*this)() - rhs;
    return (*this)();
  }

  inline T& operator-=(const value_type& rhs)
  {
    (*this)() = (*this)() - T(rhs);
    return (*this)();
  }


  // * assignment operator
  inline T& operator*=(const T& rhs)
  {
    (*this)() = (*this)() * rhs;
    return (*this)();
  }

  inline T& operator*=(const value_type& rhs)
  {
    (*this)() = (*this)() * T(rhs);
    return (*this)();
  }

  // \ assignment operator
  inline T& operator/=(const T& rhs)
  {
    (*this)() = (*this)() / rhs;
    return (*this)();
  }

  inline T& operator/=(const value_type& rhs)
  {
    (*this)() = (*this)() / T(rhs);
    return (*this)();
  }


  // increment operator
  inline T operator++(int)
  {
      T tmp = (*this)();
      (*this) += value_type(1);
      return tmp;
  }

  inline T& operator++()
  {
      (*this)() += value_type(1);
      return (*this)();
  }

  // decrement operator
  inline T operator--(int)
  {
    T tmp = (*this)();
    (*this) += value_type(1);
    return tmp;
  }

  inline T& operator--()
  {
    (*this)() += value_type(1);
    return (*this)();
  }

protected:
  // Only inheriting classes can instantiate, copy and assign TComplexVector.
  inline TComplexVector() {}
  inline ~TComplexVector() {}

  inline TComplexVector(const TComplexVector&) {}
  inline TComplexVector& operator=(const  TComplexVector&) { return *this; }
};

template<class T>
inline TComplexVector<T> operator+(const TComplexVector<T>& lhs, const typename complex_vector_type<T>::type& rhs)
{
  return lhs() + T(rhs);
}

template<class T>
inline TComplexVector<T> operator+(const typename complex_vector_type<T>::type& lhs, const TComplexVector<T>& rhs)
{
  return T(lhs) + rhs();
}

template<class T>
inline TComplexVector<T> operator-(const TComplexVector<T>& lhs, const typename complex_vector_type<T>::type& rhs)
{
  return lhs() - T(rhs);
}

template<class T>
inline TComplexVector<T> operator-(const typename complex_vector_type<T>::type& lhs, const TComplexVector<T>& rhs)
{
  return T(lhs) - rhs();
}

template<class T>
inline TComplexVector<T> operator*(const TComplexVector<T>& lhs, const typename complex_vector_type<T>::type& rhs)
{
  return lhs() * T(rhs);
}

template<class T>
inline TComplexVector<T> operator*(const typename complex_vector_type<T>::type& lhs, const TComplexVector<T>& rhs)
{
  return T(lhs) * rhs();
}

template<class T>
inline TComplexVector<T> operator/(const TComplexVector<T>& lhs, const typename complex_vector_type<T>::type& rhs)
{
  return lhs() / T(rhs);
}

template<class T>
inline TComplexVector<T> operator/(const typename complex_vector_type<T>::type& lhs, const TComplexVector<T>& rhs)
{
  return T(lhs) / rhs();
}
}
