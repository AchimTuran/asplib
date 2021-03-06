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


template<class Product>
class TProductStatistics
{
public:
  TProductStatistics()
  {
    m_CreatedProducts++;
    m_ActiveProducts++;
  }

  ~TProductStatistics()
  {
    m_DestroyedProducts++;
    m_ActiveProducts--;
  }

  static int64_t GetCreatedProducts()
  {
    return m_CreatedProducts;
  }

  static int64_t GetDestroyedProducts()
  {
    return m_DestroyedProducts;
  }

  static int64_t GetActiveProducts()
  {
    return m_ActiveProducts;
  }

private:
  static int64_t m_CreatedProducts;
  static int64_t m_DestroyedProducts;
  static int64_t m_ActiveProducts;
};

template<class Product>
int64_t TProductStatistics<Product>::m_CreatedProducts;

template<class Product>
int64_t TProductStatistics<Product>::m_DestroyedProducts;

template<class Product>
int64_t TProductStatistics<Product>::m_ActiveProducts;
