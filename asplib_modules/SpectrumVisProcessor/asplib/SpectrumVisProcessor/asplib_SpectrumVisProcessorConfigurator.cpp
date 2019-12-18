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



#include "asplib_SpectrumVisProcessorConfigurator.hpp"
#include "SpectrumVisProcessor/SpectrumCalculation/asplib_SpectrumCalculationFactory.hpp"


namespace asplib
{
CSpectrumVisProcessorConfigurator::CSpectrumVisProcessorConfigurator()
{
  m_UserProcessingSteps = false;
  OnlyPositiveFreqBins  = true;

  // configure spectrum calculatio step
  m_ConfigSpectrumCalc.fmt.inFmt        = ASPLIB_FMT_NATIVE_FLOAT;
  m_ConfigSpectrumCalc.fmt.outFmt       = ASPLIB_FMT_NATIVE_FLOAT;
  m_ConfigSpectrumCalc.processCategory  = IProcess::PROCESS_SPECTRUM_CALC;
  m_ConfigSpectrumCalc.processID        = ASPLIB_SPECTRUM_CALCULATION_SquareRoot;
  m_ConfigSpectrumCalc.options.SaveStruct(NULL);

  // configure spectrum remapping step
  m_ConfigSpectrumRemapper.fmt.inFmt        = ASPLIB_FMT_NATIVE_FLOAT;
  m_ConfigSpectrumRemapper.fmt.outFmt       = ASPLIB_FMT_NATIVE_FLOAT;
  m_ConfigSpectrumRemapper.processCategory  = IProcess::PROCESS_REMAPPER;
  m_ConfigSpectrumRemapper.processID        = ASPLIB_REMAPPER_GammaCorrector;
  m_ConfigSpectrumRemapper.options.SaveStruct(NULL);

  AttackReleaseSmootherOptions attackReleaseOptions;
  AddPostProcessingStep(ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, IProcess::PROCESS_FFT_SMOOTHER, ASPLIB_FFT_SMOOTHER_AttackRelease, true, &attackReleaseOptions);

  TLog10ScalerOptions Log10ScalerOptions;
  Log10ScalerOptions.use_dBFS_Scaling = true;
  AddPostProcessingStep(ASPLIB_FMT_NATIVE_FLOAT, ASPLIB_FMT_NATIVE_FLOAT, IProcess::PROCESS_AXIS_SCALER, ASPLIB_AXIS_SCALER_Log10Scaler, true, &Log10ScalerOptions);    
}


CSpectrumVisProcessorConfigurator::~CSpectrumVisProcessorConfigurator()
{
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::ConfigFFTWindowing(asplibFmt_t InFmt, asplibFmt_t OutFmt, CFFTWindowFunctions::eFFT_WINDOW_ID FFTWindowID, void* Options/* = NULL*/)
{
  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, m_ConfigFFTWindowing.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  err = m_ConfigFFTWindowing.options.SaveStruct(Options);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    m_ConfigFFTWindowing.FFTWindowID = CFFTWindowFunctions::FFT_WINDOW_ID_INVALID;
    return err;
  }

  if (FFTWindowID <= CFFTWindowFunctions::FFT_WINDOW_ID_INVALID || FFTWindowID >= CFFTWindowFunctions::FFT_WINDOW_ID_MAX)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }


  m_ConfigFFTWindowing.FFTWindowID = FFTWindowID;
  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CSpectrumVisProcessorConfigurator::SetFFTWindowingOptions(void* Options)
{
  return m_ConfigFFTWindowing.options.SaveStruct(Options);
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::ConfigFFT(asplibFmt_t InFmt, asplibFmt_t OutFmt, FFTLibID_t FFTLibID, void* Options/* = NULL*/)
{
  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, m_ConfigFFT.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }
  
  err = m_ConfigFFT.options.SaveStruct(Options);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    m_ConfigFFT.FFTLibID = ASPLIB_FFT_INVALID_ID;
    return err;
  }

  if (FFTLibID <= ASPLIB_FFT_INVALID_ID || FFTLibID >= ASPLIB_FFT_MAX_ID)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  m_ConfigFFT.FFTLibID = FFTLibID;
  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CSpectrumVisProcessorConfigurator::SetFFTOptions(void *Options)
{
  return m_ConfigFFT.options.SaveStruct(Options);
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::ConfigSpectrumCalc(asplibFmt_t InFmt, asplibFmt_t OutFmt, int32_t ProcessID, void* Options/* = NULL*/)
{
  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, m_ConfigSpectrumCalc.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  err = m_ConfigSpectrumCalc.options.SaveStruct(Options);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  m_ConfigSpectrumCalc.processCategory  = IProcess::PROCESS_SPECTRUM_CALC;
  m_ConfigSpectrumCalc.processID        = ProcessID;
  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CSpectrumVisProcessorConfigurator::SetSpectrumCalcOptions(void* Options)
{
  return m_ConfigSpectrumCalc.options.SaveStruct(Options);
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::ConfigSpectrumRemapper(asplibFmt_t InFmt, asplibFmt_t OutFmt, int32_t ProcessID, void* Options/* = NULL*/)
{
  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, m_ConfigSpectrumRemapper.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  err = m_ConfigSpectrumRemapper.options.SaveStruct(Options);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  m_ConfigSpectrumRemapper.processCategory  = IProcess::PROCESS_REMAPPER;
  m_ConfigSpectrumRemapper.processID        = ProcessID;
  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CSpectrumVisProcessorConfigurator::SetSpectrumRemapperOptions(void* Options)
{
  return m_ConfigSpectrumRemapper.options.SaveStruct(Options);
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::AddPostProcessingStep(asplibFmt_t InFmt, asplibFmt_t OutFmt, IProcess::ProcessTypes_t ProcessCategory, int32_t ProcessID, void* Options/* = NULL*/)
{
  return AddPostProcessingStep(InFmt, OutFmt, ProcessCategory, ProcessID, false, Options);
}

ASPLIB_ERR CSpectrumVisProcessorConfigurator::SetPostProcessingOptions(uint32_t Step, void* Options)
{
  if (Step > m_PostProcessSteps.size() || Step <= 0)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  return m_PostProcessSteps[Step -1].options.SaveStruct(Options);
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::AddPostProcessingStep(asplibFmt_t InFmt, asplibFmt_t OutFmt, IProcess::ProcessTypes_t ProcessCategory, int32_t ProcessID, bool SetDefaults, void* Options/* = NULL*/)
{
  if (!SetDefaults)
  { // delete all default processing steps and set user steps
    if (!m_UserProcessingSteps)
    { // clear current list and use user defined steps
      m_UserProcessingSteps = true;
      m_PostProcessSteps.clear();
    }
  }

  ProcessConfig_t config;

  ASPLIB_ERR err = SetFmt(InFmt, OutFmt, config.fmt);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  err = SetProcessCategory(ProcessCategory, config.processCategory);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  err = config.options.SaveStruct(Options);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  config.processID = ProcessID;
  m_PostProcessSteps.push_back(config);
  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::SetFmt(asplibFmt_t InFmt, asplibFmt_t OutFmt, ConfigFmt_t &Fmt)
{
  if (InFmt <= ASPLIB_FMT_INVALID || InFmt >= ASPLIB_FMT_MAX || OutFmt <= ASPLIB_FMT_INVALID || OutFmt >= ASPLIB_FMT_MAX)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  Fmt.inFmt  = InFmt;
  Fmt.outFmt = OutFmt;
  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CSpectrumVisProcessorConfigurator::SetProcessCategory(IProcess::ProcessTypes_t ProcessCategory, IProcess::ProcessTypes_t &OutProcessCategory)
{
  if (ProcessCategory <= IProcess::PROCESS_INVALID_ID || ProcessCategory >= IProcess::PROCESS_MAX_ID)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  OutProcessCategory = ProcessCategory;
  return ASPLIB_ERR_NO_ERROR;
}
}
