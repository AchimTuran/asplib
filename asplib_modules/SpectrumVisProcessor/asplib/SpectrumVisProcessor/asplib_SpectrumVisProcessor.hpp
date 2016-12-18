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

#include "SpectrumVisProcessor/asplib_SpectrumVisProcessorConfigurator.hpp"

#include "FFT/asplib_FFTFactory.hpp"
#include "FFT/FFTWindowing/asplib_FFTWindowingFactory.h"
#include "Core/Buffers/asplib_BufferTypes.hpp"
#include "Interfaces/asplib_IProcess.hpp"

#include <vector>


namespace asplib
{
class CSpectrumVisProcessor
{
public:
  CSpectrumVisProcessor();
  ~CSpectrumVisProcessor();

  /*!
   * @brief Creates the SpectrumVisProcessor.
   *
   * @param Config SpectrumVisProcessor configuration. For more detaily see CSpectrumVisProcessorConfigurator.
   * @param FrameSize Configures the output frame size.
   * @param FFTFrameSize Configures the frame size that is used by the FFT library.
   * @return asplib error code. For more details see ASPLIB_ERR.
   */
  ASPLIB_ERR Create(CSpectrumVisProcessorConfigurator &Config, uint32_t FrameSize, uint32_t FFTFrameSize=4096);
  
  /*!
   * @brief Call this method to process one frame. 
   *
   * @param In Input array pointer.
   * @param Out Output array pointer.
   * @return asplib error code. For more details see ASPLIB_ERR.
   * @remarks Make sure you have enough memory reserved for In and Out. For higher performance this method does no error checking.
   */
  ASPLIB_ERR Process(float *In, float *Out);
  
  /*!
   * @brief Destroys the SpectrumVisProcessor.
   *
   * @return asplib error code. For more details see ASPLIB_ERR.
   */
  ASPLIB_ERR Destroy();


  /*!
   * @brief Get the used post processing chain frame size.
   *
   * @param FrameSize Amount of bars the spectrum should have. This excludes the mirrored parts
   * @return asplib error code. For more details see ASPLIB_ERR.
   */
  ASPLIB_ERR GetPostProcessFrameSize(uint32_t &FrameSize);

private:
  int8_t                  m_MaxProcessingSteps;
  std::vector<IProcess*>  m_ProcessVector;
  IProcess                **m_PostProcess;
  CFFTWindowFunctions     *m_FFTWindow;
  IFFT                    *m_FFT;
  IProcess                *m_SpectrumRemapper;
  IProcess                *m_SpectrumCalc;

  uint32_t                      m_FrameSize;
  uint32_t                      m_FFTFrameSize;
  uint32_t                      m_RemapperFrameSize;
  FrameBuffer_NativeFloat       *m_InBuf;
  FrameBuffer_NativeFloat       *m_PostProcessBuf[2];
  FrameBuffer_NativeCpxFloat    *m_OutFFTBuf;
};
}
