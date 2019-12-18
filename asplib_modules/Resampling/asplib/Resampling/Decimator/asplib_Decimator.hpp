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



// core includes
#include "Core/os/asplib_os.h"
#include "Core/Constants_Typedefs/asplib_Typedefs.h"

// resampling includes
#include "Resampling/Decimator/asplib_DecimatorOptions.hpp"
#include "Resampling/Interfaces/asplib_IResampling.hpp"

// STL includes
#include <vector>


namespace asplib
{
  class CDecimator : public IResampling
  {
  public:
    CDecimator();
    ~CDecimator();

    ASPLIB_ERR Create(const uint32_t InputFrameSize, const uint32_t OutputFrameSize, const uint32_t SampleFrequency, const void *Options = nullptr) override;
    ASPLIB_ERR Process(const void *In, void *Out) override;
    ASPLIB_ERR Destroy() override;

  private:
    uint32_t m_inputFrameSize;
    uint32_t m_outputFrameSize;
    uint32_t m_sampleFrequency;
    float m_cutOffFrequency;

    uint32_t m_decimationFactor;
    uint32_t m_filterLength;
    float *m_filterCoeff;

    std::vector<float> m_convolutionBuffer;
  };
}
#pragma once
