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

#include "Core/ExtendedStructs/asplib_ExtendedStructContainer.hpp"
#include "Core/Constants_Typedefs/asplib_DataFmtDefines.hpp"

#include "FFT/FFTWindowing/asplib_FFTWindowingFactory.h"
#include "FFT/asplib_FFTFactory.hpp"
#include "SpectrumVisProcessor/AxisScaler/asplib_AxisScalerFactory.hpp"
#include "SpectrumVisProcessor/Remapper/asplib_RemapperFactory.hpp"
#include "SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.hpp"

// include options
#include "FFT/asplib_FFTOptions.hpp"
#include "SpectrumVisProcessor/AxisScaler/asplib_AxisScalerOptions.hpp"
#include "SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherOptions.hpp"
#include "SpectrumVisProcessor/Remapper/asplib_RemapperOptions.hpp"


namespace asplib
{
class CSpectrumVisProcessor;

class CSpectrumVisProcessorConfigurator
{
friend class CSpectrumVisProcessor;

private: // internal used struct typedefs
  typedef struct ConfigFmt_t
  {
    asplibFmt_t inFmt;
    asplibFmt_t outFmt;

    ConfigFmt_t()
    {
      inFmt  = ASPLIB_FMT_INVALID;
      outFmt = ASPLIB_FMT_INVALID;
    }
  }ConfigFmt_t;


  typedef struct ConfigFFTWindowing_t
  {
    ConfigFmt_t                         fmt;
    CFFTWindowFunctions::eFFT_WINDOW_ID FFTWindowID;
    CExtendedStructContainer            options;

    ConfigFFTWindowing_t()
    {
      FFTWindowID = CFFTWindowFunctions::FFT_WINDOW_ID_HANN;
      fmt.inFmt   = ASPLIB_FMT_NATIVE_FLOAT;
      fmt.outFmt  = ASPLIB_FMT_NATIVE_FLOAT;
    }
  }ConfigFFTWindowing_t;

  typedef struct ConfigFFT_t
  {
    ConfigFmt_t fmt;
    FFTLibID_t  FFTLibID;
    CExtendedStructContainer options;

    ConfigFFT_t()
    {
      FFTLibID    = ASPLIB_FFT_KISS_FFT_REAL;
      fmt.inFmt   = ASPLIB_FMT_NATIVE_FLOAT;
      fmt.outFmt  = ASPLIB_FMT_NATIVE_CPX_FLOAT;
    }
  }ConfigFFT_t;

  typedef struct ProcessConfig_t
  {
    ConfigFmt_t               fmt;
    IProcess::ProcessTypes_t  processCategory;
    int32_t                   processID;
    CExtendedStructContainer  options;

    ProcessConfig_t()
    {
      fmt.inFmt  = ASPLIB_FMT_INVALID;
      fmt.outFmt = ASPLIB_FMT_INVALID;

      processCategory = IProcess::PROCESS_INVALID_ID;

      processID = -1;
    }

    ProcessConfig_t(const ProcessConfig_t &Config)
    {
      fmt.inFmt  = Config.fmt.inFmt;
      fmt.outFmt = Config.fmt.outFmt;

      processCategory = Config.processCategory;
      
      processID = Config.processID;

      options = Config.options;
    }
  }ProcessConfig_t;

  typedef std::vector<ProcessConfig_t> ProcessConfigVector_t;


public:
  CSpectrumVisProcessorConfigurator();
  ~CSpectrumVisProcessorConfigurator();

  /*!
   * @brief Call this method to process one frame. 
   *
   * @param InFmt Configures the input data format. Fore more details see asplibFmt_t.
   * @param OutFmt Configures the output data format. Fore more details see asplibFmt_t.
   * @param FFTWindowID Sets the corresponding FFT Window function. For more details see CFFTWindowFunctions::eFFT_WINDOW_ID.
   * @param Options This parameter is reserved for future versions.
   * @return asplib error code. For more details see ASPLIB_ERR.
   * @remarks Make sure you have enough memory reserved for In and Out. For higher performance this method does no error checking.
   */
  ASPLIB_ERR ConfigFFTWindowing(asplibFmt_t InFmt, asplibFmt_t OutFmt, CFFTWindowFunctions::eFFT_WINDOW_ID FFTWindowID, void* Options = NULL);
  
  /*!
   * @brief Sets the options for the configured FFT window module.
   *
   * @param Options FFT window options.
   * @return asplib error code. For more details see ASPLIB_ERR.
   */
  ASPLIB_ERR SetFFTWindowingOptions(void* Options);
  
  /*!
   * @brief Call this method to process one frame. 
   *
   * @param In Input array pointer.
   * @param Out Output array pointer.
   * @return asplib error code. For more details see ASPLIB_ERR.
   * @remarks Make sure you have enough memory reserved for In and Out. For higher performance this method does no error checking.
   */
  ASPLIB_ERR ConfigFFT(asplibFmt_t InFmt, asplibFmt_t OutFmt, FFTLibID_t FFTLibID, void* Options = NULL);
  
  /*!
   * @brief Sets the options for the configured FFT module.
   *
   * @param Options FFT options.
   * @return asplib error code. For more details see ASPLIB_ERR.
   */
  ASPLIB_ERR SetFFTOptions(void *Options);
  
  /*!
   * @brief Call this method to process one frame. 
   *
   * @param In Input array pointer.
   * @param Out Output array pointer.
   * @return asplib error code. For more details see ASPLIB_ERR.
   * @remarks Make sure you have enough memory reserved for In and Out. For higher performance this method does no error checking.
   */
  ASPLIB_ERR ConfigSpectrumCalc(asplibFmt_t InFmt, asplibFmt_t OutFmt, int32_t ProcessID, void* Options = NULL);
  
  /*!
   * @brief Sets the options for the configured spectrum calculation module.
   *
   * @param Options FFT window options.
   * @return asplib error code. For more details see ASPLIB_ERR.
   */
  ASPLIB_ERR SetSpectrumCalcOptions(void* Options);

  /*!
   * @brief Call this method to process one frame. 
   *
   * @param In Input array pointer.
   * @param Out Output array pointer.
   * @return asplib error code. For more details see ASPLIB_ERR.
   * @remarks Make sure you have enough memory reserved for In and Out. For higher performance this method does no error checking.
   */
  ASPLIB_ERR ConfigSpectrumRemapper(asplibFmt_t InFmt, asplibFmt_t OutFmt, int32_t ProcessID, void* Options = NULL);
  
  /*!
   * @brief Sets the options for the configured spectrum remapper module.
   *
   * @param Options Spectrum remapping options.
   * @return asplib error code. For more details see ASPLIB_ERR.
   */
  ASPLIB_ERR SetSpectrumRemapperOptions(void* Options);
  
  /*!
   * @brief Call this method to process one frame. 
   *
   * @param In Input array pointer.
   * @param Out Output array pointer.
   * @return asplib error code. For more details see ASPLIB_ERR.
   * @remarks Make sure you have enough memory reserved for In and Out. For higher performance this method does no error checking.
   */
  ASPLIB_ERR AddPostProcessingStep(asplibFmt_t InFmt, asplibFmt_t OutFmt, IProcess::ProcessTypes_t ProcessCategory, int32_t ProcessID, void* Options = NULL);

  /*!
   * @brief Sets the options for a specific post processing step. 
   *
   * @param Step Post processing step. Valid values are in the range of 1 ... added post processing steps.
   * @param Options Post processing step options.
   * @return asplib error code. For more details see ASPLIB_ERR.
   */
  ASPLIB_ERR SetPostProcessingOptions(uint32_t Step, void* Options);

  // If true, only the positive frequency bins are feed into the post processing chain.
  // Otherwise the whole spectrum is processed, which is symmetrical for real signals.
  // Default is false.
  // NOTE: This option is ignored if the internal and output frame size from the FFT library is different.
  bool OnlyPositiveFreqBins;


private:
  ASPLIB_ERR AddPostProcessingStep(asplibFmt_t InFmt, asplibFmt_t OutFmt, IProcess::ProcessTypes_t ProcessCategory, int32_t ProcessID, bool SetDefaults, void* Options = NULL);
  ASPLIB_ERR SetFmt(asplibFmt_t InFmt, asplibFmt_t OutFmt, ConfigFmt_t &Fmt);
  ASPLIB_ERR SetProcessCategory(IProcess::ProcessTypes_t ProcessCategory, IProcess::ProcessTypes_t &OutProcessCategory);

  ConfigFFTWindowing_t  m_ConfigFFTWindowing;
  ConfigFFT_t           m_ConfigFFT;
  ProcessConfig_t       m_ConfigSpectrumCalc;
  ProcessConfig_t       m_ConfigSpectrumRemapper;
  ProcessConfigVector_t m_PostProcessSteps;
  bool                  m_UserProcessingSteps;
};
}
