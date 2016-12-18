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

#include "FFT/Interfaces/asplib_IFFT.h"
#include "FFT/KissFFT/asplib_KissFFTCpxOptions.hpp"
#include <KissFFT/kiss_fft.h>

#define KISS_FFT_INVERSE  1
#define KISS_FFT_FORWARD  0


namespace asplib
{
//|--------------------------|
//| requiered converters     |
//|--------------------------|
//| FFT:                     |
//| - extern to FFT          |
//| - FFT to intern          |
//|                          |
//| iFFT:                    |
//| - intern to FFT          |
//| - FFT to extern          |
//|--------------------------|
template<class Extern_TO_FFTConverter, class FFT_TO_ExternConverter, class Intern_TO_FFTConverter, class FFT_TO_InternConverter>
class TKissFFT : public IFFT
{
public:
  TKissFFT()
  {
    m_FFTConfig  = NULL;
    m_iFFTConfig = NULL;
  }

  ~TKissFFT()
  {
    Destroy();
  }

  virtual ASPLIB_ERR FFT(void *In, void *Out)
  {
    if (!In || !Out)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fft(m_FFTConfig, m_Extern_TO_FFTConverter(In), m_FFT_TO_InternConverter(Out));
    m_FFT_TO_InternConverter.fmtc_FFT_TO_Fmt(Out);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR FFT(void *RIn, void *CIn, void *Out)
  {
    if (!RIn || !CIn || !Out)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fft(m_FFTConfig, m_Extern_TO_FFTConverter(RIn, CIn), m_FFT_TO_InternConverter(Out));
    m_FFT_TO_InternConverter.fmtc_FFT_TO_Fmt(Out);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR iFFT(void *In, void *Out)
  {
    if (!In || !Out)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fft(m_iFFTConfig, m_Intern_TO_FFTConverter(In), m_FFT_TO_ExternConverter(Out));
    m_FFT_TO_ExternConverter.fmtc_FFT_TO_Fmt(Out);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR iFFT(void *In, void *ROut, void *COut)
  {
    if (!In || !ROut || !COut)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fft(m_iFFTConfig, m_Intern_TO_FFTConverter(In), m_FFT_TO_ExternConverter(ROut, COut));
    m_FFT_TO_ExternConverter.fmtc_FFT_TO_SplitFmt(ROut, COut);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR Destroy()
  {
    if (m_FFTConfig)
    {
      kiss_fft_free(m_FFTConfig);
    }
    m_FFTConfig = NULL;

    if (m_iFFTConfig)
    {
      kiss_fft_free(m_iFFTConfig);
    }
    m_iFFTConfig = NULL;

    m_Extern_TO_FFTConverter.Destroy();
    m_FFT_TO_ExternConverter.Destroy();
    m_Intern_TO_FFTConverter.Destroy();
    m_FFT_TO_InternConverter.Destroy();

    return ASPLIB_ERR_NO_ERROR;
  }

  // todo implement configuration part for overlapping frames
  virtual ASPLIB_ERR CreateFFTLib(void *Options = NULL)
  {
    bool enableZeroPadding = false;
    if (Options)
    {
      CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Options);
      if (extendedStruct->ID != ASPLIB_EXTENDED_STRUCT_KissFFTCpxOptions)
      {
        return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT;
      }

      KissFFT_CpxOptions *KissFFTOptions = static_cast<KissFFT_CpxOptions*>(Options);
      enableZeroPadding = KissFFTOptions->enableZeroPadding;
    }

    // assign KissFFT options
    if (enableZeroPadding)
    {
      m_InternalFrameSize = kiss_fft_next_fast_size(m_InternalFrameSize);
      m_OutFrameSize = m_InternalFrameSize;
    }

    if (m_InternalFrameSize < m_InFrameSize)
    {// InternalFrameSize is never smaller than InFrameSize! FATAL ERROR!
      return ASPLIB_ERR_FFT_INVALID_INTERNAL_FRAME_SIZE;
    }

    m_FFTConfig  = kiss_fft_alloc(m_InternalFrameSize, KISS_FFT_FORWARD, 0, 0);
    m_iFFTConfig = kiss_fft_alloc(m_InternalFrameSize, KISS_FFT_INVERSE, 0, 0);
    if (!m_FFTConfig || !m_iFFTConfig)
    {
      return ASPLIB_ERR_FFT_CONFIGURE_FAILED;
    }

    if (m_Extern_TO_FFTConverter.Create(m_InFrameSize, m_InternalFrameSize, Options) != ASPLIB_ERR_NO_ERROR)
    {
      return ASPLIB_ERR_FFT_FMT_CONVERTER_CONFIGURE_FAILED;
    }

    if (m_FFT_TO_ExternConverter.Create(m_InternalFrameSize, m_InFrameSize, Options) != ASPLIB_ERR_NO_ERROR)
    {
      return ASPLIB_ERR_FFT_FMT_CONVERTER_CONFIGURE_FAILED;
    }

    if (m_Intern_TO_FFTConverter.Create(m_OutFrameSize, m_InternalFrameSize, Options) != ASPLIB_ERR_NO_ERROR)
    {
      return ASPLIB_ERR_FFT_FMT_CONVERTER_CONFIGURE_FAILED;
    }

    if (m_FFT_TO_InternConverter.Create(m_InternalFrameSize, m_OutFrameSize, Options) != ASPLIB_ERR_NO_ERROR)
    {
      return ASPLIB_ERR_FFT_FMT_CONVERTER_CONFIGURE_FAILED;
    }

    return ASPLIB_ERR_NO_ERROR;
  }

private:
  kiss_fft_cfg m_FFTConfig;
  kiss_fft_cfg m_iFFTConfig;
  
  // extern to intern converter objects
  Extern_TO_FFTConverter m_Extern_TO_FFTConverter;
  FFT_TO_InternConverter m_FFT_TO_ExternConverter;

  // intern to extern converter objects
  Intern_TO_FFTConverter m_Intern_TO_FFTConverter;
  FFT_TO_InternConverter m_FFT_TO_InternConverter;
};
}
