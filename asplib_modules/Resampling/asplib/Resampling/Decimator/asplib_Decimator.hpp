#pragma once

/* Copyright (C) 2014-2016 Achim Turan, Achim.Turan@o2online.de
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

#include "Resampling/Decimator/asplib_DecimatorOptions.hpp"

#include "Resampling/Interfaces/asplib_IResampling.hpp"


namespace asplib
{
  class CDecimator : public IResampling
  {
  public:
    CDecimator();
    ~CDecimator();

    virtual ASPLIB_ERR Create(uint32_t FrameSize, uint32_t SampleFrequency, void *Options = nullptr);
    virtual ASPLIB_ERR Process(void *In, void *Out);
    virtual ASPLIB_ERR Destroy();

  private:
    uint32_t m_FrameSize;
    uint32_t m_SampleFrequency;
    float m_CutOffFrequency;

    uint32_t m_DecimationFactor;
    uint32_t m_FilterLength;
    float *m_FilterCoeff;

  };
}
#pragma once
