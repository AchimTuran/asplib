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



#include "asplib_SpectrumVisProcessor.hpp"
#include "Core/Constants_Typedefs/asplib_DataFmtDefines.hpp"
#include "Interfaces/asplib_ProcessFactory.hpp"

#include <vector>

using namespace std;


namespace asplib
{
CSpectrumVisProcessor::CSpectrumVisProcessor()
{
  m_FFTWindow         = nullptr;
  m_FFT               = nullptr;
  m_SpectrumCalc      = nullptr;

  m_InBuf             = nullptr;
  m_OutFFTBuf         = nullptr;
  m_PostProcess       = nullptr;
  m_PostProcessBuf[0] = nullptr;
  m_PostProcessBuf[1] = nullptr;
  m_SpectrumRemapper  = nullptr;

  m_FrameSize             = 0;
  //m_PostProcessFrameSize  = 0;
  m_MaxProcessingSteps    = -1;
}


CSpectrumVisProcessor::~CSpectrumVisProcessor()
{
  Destroy();
}


ASPLIB_ERR CSpectrumVisProcessor::Create(CSpectrumVisProcessorConfigurator &Config, uint32_t FrameSize, uint32_t FFTFrameSize)
{
  if (FrameSize <= 0 || FFTFrameSize <= 0 || FrameSize > FFTFrameSize)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }
  Destroy();
  m_FrameSize     = FrameSize;
  m_FFTFrameSize  = FFTFrameSize;

  ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;

  // create FFT window
  err = CFFTWindowingFactory::CreateFFTWindow(m_FFTFrameSize, Config.m_ConfigFFTWindowing.FFTWindowID, m_FFTWindow);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }


  // create FFT library
  err = ASPLIB_FACTORY_FFT.Create(Config.m_ConfigFFT.FFTLibID, Config.m_ConfigFFT.fmt.inFmt, Config.m_ConfigFFT.fmt.outFmt, m_FFT);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }

  err = m_FFT->Create(m_FFTFrameSize, Config.m_ConfigFFT.options);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }
  
  if (Config.OnlyPositiveFreqBins && m_FFT->InternalFrameSize() == m_FFT->OutFrameSize())
  {
    m_RemapperFrameSize = m_FFT->InternalFrameSize() / 2;
  }
  else
  {
    m_RemapperFrameSize = m_FFT->OutFrameSize();
  }

  // create spectrum calculcation step
  err = CProcessFactoryCallbacks::Create(Config.m_ConfigSpectrumCalc.processCategory, Config.m_ConfigSpectrumCalc.processID, 
                                         Config.m_ConfigSpectrumCalc.fmt.inFmt, Config.m_ConfigSpectrumCalc.fmt.outFmt, m_SpectrumCalc);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }
  err = m_SpectrumCalc->Create(m_RemapperFrameSize, Config.m_ConfigSpectrumCalc.options);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }


  // create spectrum remapping step
  err = CProcessFactoryCallbacks::Create(Config.m_ConfigSpectrumRemapper.processCategory, Config.m_ConfigSpectrumRemapper.processID, 
                                         Config.m_ConfigSpectrumRemapper.fmt.inFmt, Config.m_ConfigSpectrumRemapper.fmt.outFmt, m_SpectrumRemapper);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }
  err = m_SpectrumRemapper->SetOutputFrameSize(m_FrameSize);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }
  err = m_SpectrumRemapper->Create(m_RemapperFrameSize, Config.m_ConfigSpectrumRemapper.options);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    Destroy();
    return err;
  }
  

  // create post processing steps
  for (int32_t ii = 0; ii < Config.m_PostProcessSteps.size(); ii++)
  {
    IProcess *p = NULL;

    err = CProcessFactoryCallbacks::Create(Config.m_PostProcessSteps[ii].processCategory, Config.m_PostProcessSteps[ii].processID,
                                           Config.m_PostProcessSteps[ii].fmt.inFmt, Config.m_PostProcessSteps[ii].fmt.outFmt, p);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      Destroy();
      return err;
    }

    CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>((void*)Config.m_PostProcessSteps[ii].options);
    if (extendedStruct && extendedStruct->ID == ASPLIB_EXTENDED_STRUCT_TLog10ScalerOptions)
    {
      TLog10ScalerOptions *options = static_cast<TLog10ScalerOptions*>((void*)Config.m_PostProcessSteps[ii].options);

      if (options->use_dBFS_Scaling)
      {
        err = options->Calc_dBFS_ScalingValue(m_FFT->InternalFrameSize());
        if (err != ASPLIB_ERR_NO_ERROR)
        {
          return err;
        }
      }
    }

    err = p->Create(m_FrameSize, Config.m_PostProcessSteps[ii].options);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      CProcessFactoryCallbacks::Destroy(p);
      Destroy();
      return err;
    }

    m_ProcessVector.push_back(p);
  }

  if (m_ProcessVector.size() > 0)
  {
    m_PostProcess = m_ProcessVector.data();
    m_MaxProcessingSteps = (int8_t)m_ProcessVector.size() -1;
  }
  else
  {
    m_PostProcess = NULL;
    m_MaxProcessingSteps = -1;
  }

  // create buffers
  m_OutFFTBuf = new FrameBuffer_NativeCpxFloat(m_FFTFrameSize, 1);
  m_InBuf     = new FrameBuffer_NativeFloat(m_FFTFrameSize, 1);
  m_PostProcessBuf[0] = new FrameBuffer_NativeFloat(m_FrameSize, 1);
  m_PostProcessBuf[1] = new FrameBuffer_NativeFloat(m_FrameSize, 1);
  
  return err;
}


ASPLIB_ERR CSpectrumVisProcessor::Process(float *In, float *Out)
{
  asplibFmt_NativeCPXFloat *cpxSamples = m_OutFFTBuf->get_Frame(0);
  asplibFmt_NativeFloat *inBuf = NULL;
  asplibFmt_NativeFloat *outBuf = m_PostProcessBuf[1]->get_Frame(0);

  if (m_MaxProcessingSteps >= 0)
  {
    inBuf = m_PostProcessBuf[0]->get_Frame(0);
  }
  else
  {
    inBuf = Out;
  }

  m_FFTWindow->Process(In, m_InBuf->get_Frame(0));

  m_FFT->FFT(m_InBuf->get_Frame(0), cpxSamples);

  m_SpectrumCalc->Process(cpxSamples, m_InBuf->get_Frame(0));
  m_SpectrumRemapper->Process(m_InBuf->get_Frame(0), inBuf);

  if (m_MaxProcessingSteps >= 0)
  {
    for (int8_t step = 0; step < m_MaxProcessingSteps; step++)
    {
      m_PostProcess[step]->Process(inBuf, outBuf);

      asplibFmt_NativeFloat *tmp = inBuf;
      inBuf = outBuf;
      outBuf = tmp;
    }

    m_PostProcess[m_MaxProcessingSteps]->Process(inBuf, Out);
  }

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessor::Destroy()
{
  ASPLIB_FACTORY_FFT.Destroy(m_FFT);
  CFFTWindowingFactory::DestroyFFTWindow(m_FFTWindow);
  CProcessFactoryCallbacks::Destroy(m_SpectrumCalc);
  CProcessFactoryCallbacks::Destroy(m_SpectrumRemapper);

  for (IProcess::ProcessVector_t::iterator iter = m_ProcessVector.begin(); iter != m_ProcessVector.end(); ++iter)
  {
    (*iter)->Destroy();
    CProcessFactoryCallbacks::Destroy(*iter);
  }
  m_ProcessVector.clear();
  m_PostProcess = NULL;

  delete m_InBuf;
  m_InBuf = NULL;

  delete m_PostProcessBuf[0];
  m_PostProcessBuf[0] = NULL;
  delete m_PostProcessBuf[1];
  m_PostProcessBuf[1] = NULL;

  delete m_OutFFTBuf;
  m_OutFFTBuf = NULL;

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessor::GetPostProcessFrameSize(uint32_t &FrameSize)
{
  FrameSize = m_FrameSize;
  return ASPLIB_ERR_NO_ERROR;
}
}
