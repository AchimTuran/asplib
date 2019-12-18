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

#include "SpectrumVisProcessor/Interfaces/asplib_IProcess.hpp"
#include "SpectrumVisProcessor/FFTSmoother/AttackReleaseSmoother/asplib_TAttackReleaseSmootherOptions.hpp"


namespace asplib
{
template<typename T, class TSmoother>
class TAttackReleaseSmoother : public IProcess
{
public:
  TAttackReleaseSmoother() :
    IProcess(IProcess::PROCESS_FFT_SMOOTHER)
  {
    // set default values from options structure
    AttackReleaseSmootherOptions options;
    m_AttackTime     = options.AttackTime;
    m_ReleaseTime    = options.ReleaseTime;
    m_UseAnalogConst = options.UseAnalogConst;

    m_Smoother = NULL;
    
    m_FrameSize = 0;
    m_SampleFrequency = 44100.0; // TODO: How to set this from outside?
  }

  virtual ASPLIB_ERR Create(uint32_t FrameSize, void *Options)
  {
    if (FrameSize <= 0)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    Destroy();
    m_FrameSize = FrameSize;
    m_Smoother  = new TSmoother[m_FrameSize];

    return SetOptions(Options);
  }

  virtual ASPLIB_ERR SetOptions(void * Options)
  {
    if (Options)
    {
      CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Options);
      if (extendedStruct->ID != ASPLIB_EXTENDED_STRUCT_TAttackReleaseSmootherOptions)
      {
        return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT;
      }

      AttackReleaseSmootherOptions *options = static_cast<AttackReleaseSmootherOptions*>(Options);
      if (options->AttackTime == 0.0 || options->ReleaseTime == 0.0)
      {
        return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT; // TODO: define better error codes
      }

      m_AttackTime     = options->AttackTime;
      m_ReleaseTime    = options->ReleaseTime;
      m_UseAnalogConst = options->UseAnalogConst;
    }

    long double filterCoeAttackTime = 0.0;
    long double filterCoeReleaseTime = 0.0;
    if (m_UseAnalogConst)
    {
      filterCoeAttackTime  = exp(((long double)m_TauAnalog) / (m_AttackTime*((long double)m_SampleFrequency)));
      filterCoeReleaseTime = exp(((long double)m_TauAnalog) / (m_ReleaseTime*((long double)m_SampleFrequency)));
    }
    else
    {
      filterCoeAttackTime  = exp(((long double)m_TauDigital) / (m_AttackTime*((long double)m_SampleFrequency)));
      filterCoeReleaseTime = exp(((long double)m_TauDigital) / (m_ReleaseTime*((long double)m_SampleFrequency)));
    }
    for (uint32_t ii = 0; ii < m_FrameSize; ii++)
    {
      m_Smoother[ii].SetCoefficients(filterCoeAttackTime, filterCoeReleaseTime);
    }

    return ASPLIB_ERR_NO_ERROR;
  }
  
  virtual ASPLIB_ERR Process(void *In, void *Out)
  {
    T *in  = static_cast<T*>(In);
    T *out = static_cast<T*>(Out);

    for (uint32_t ii = 0; ii < m_FrameSize; ii++)
    {
      out[ii] = m_Smoother[ii].Filter(in[ii]);
    }

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR Destroy()
  {
    delete [] m_Smoother;
    m_Smoother = NULL;

    return ASPLIB_ERR_NO_ERROR;
  }

private:
  static const long double m_TauAnalog;
  static const long double m_TauDigital;

  bool        m_UseAnalogConst;
  long double m_AttackTime;
  long double m_ReleaseTime;

  TSmoother   *m_Smoother;
  uint32_t    m_FrameSize;
  long double m_SampleFrequency;
};

template<typename T, class TSmoother>
const long double TAttackReleaseSmoother<T, TSmoother>::m_TauAnalog  = log10((long double)0.368);
template<typename T, class TSmoother>
const long double TAttackReleaseSmoother<T, TSmoother>::m_TauDigital = log10((long double)0.01);
}
