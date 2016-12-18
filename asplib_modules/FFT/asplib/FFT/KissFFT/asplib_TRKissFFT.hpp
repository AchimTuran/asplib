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

#include "Core/ExtendedStructs/asplib_ExtendedStructs.hpp"

#include "FFT/Interfaces/asplib_IFFT.h"
#include "FFT/KissFFT/asplib_KissFFTRealOptions.hpp"
#include <KissFFT/tools/kiss_fftr.h>

#define KISS_FFT_INVERSE  1
#define KISS_FFT_FORWARD  0


namespace asplib
{
//|--------------------------|
//| requiered converters     |
//|--------------------------|
//| fft:                     |
//| - extern to fft          |
//| - fft to intern          |
//|                          |
//| ifft:                    |
//| - intern to fft          |
//| - fft to extern          |
//|--------------------------|
template<class Extern_TO_FFTConverter, class FFT_TO_ExternConverter, class Intern_TO_FFTConverter, class FFT_TO_InternConverter>
class TRKissFFT : public IFFT
{
public:
  TRKissFFT()
  {
    m_FFTConfig  = NULL;
    m_iFFTConfig = NULL;

    m_SymmetricSpectrum    = false;
    SymmetricSpectrumStart = 0;
  }
  
  ~TRKissFFT()
  {
    Destroy();
  }

  virtual ASPLIB_ERR FFT(void *In, void *Out)
  {
    if (!In || !Out)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fftr(m_FFTConfig, m_Extern_TO_FFTConverter(In), m_FFT_TO_InternConverter(Out));
    if (m_SymmetricSpectrum)
    {
      kiss_fft_cpx *spectrum = m_FFT_TO_InternConverter.GetFFTPtr();
      uint32_t jj = 1;
      uint32_t ii = m_InternalFrameSize -1;
      while (ii > SymmetricSpectrumStart)
      {
        spectrum[ii].r = spectrum[jj].r;
        spectrum[ii].i = spectrum[jj].i;
        jj++;
        ii--;
      }
    }
    m_FFT_TO_InternConverter.fmtc_FFT_TO_Fmt(Out);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR FFT(void *RIn, void *CIn, void *Out)
  {// a real fft transformation can't handle complex input values
    return ASPLIB_ERR_NOT_IMPLEMENTED;
  }

  virtual ASPLIB_ERR iFFT(void *In, void *Out)
  {
    if (!In || !Out)
    {
      return ASPLIB_ERR_INVALID_INPUT;
    }

    kiss_fftri(m_iFFTConfig, m_Intern_TO_FFTConverter(In), m_FFT_TO_ExternConverter(Out));
    m_FFT_TO_ExternConverter.fmtc_FFT_TO_Fmt(Out);

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR iFFT(void *In, void *ROut, void *COut)
  {// a real ifft transformation can't handle complex outpt values
    return ASPLIB_ERR_NOT_IMPLEMENTED;
  }

  virtual ASPLIB_ERR Destroy()
  {
    if (m_FFTConfig)
    {
      kiss_fftr_free(m_FFTConfig);
    }
    m_FFTConfig = NULL;

    if (m_iFFTConfig)
    {
      kiss_fftr_free(m_iFFTConfig);
    }
    m_iFFTConfig = NULL;

    m_Extern_TO_FFTConverter.Destroy();
    m_FFT_TO_ExternConverter.Destroy();
    m_Intern_TO_FFTConverter.Destroy();
    m_FFT_TO_InternConverter.Destroy();

    return ASPLIB_ERR_NO_ERROR;
  }

  virtual ASPLIB_ERR CreateFFTLib(void *Options = NULL)
  {
    bool enableZeroPadding = false;
    if (Options)
    {
      CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Options);
      if (extendedStruct->ID != ASPLIB_EXTENDED_STRUCT_KissFFTRealOptions)
      {
        return ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT;
      }

      KissFFT_RealOptions *KissFFTOptions = static_cast<KissFFT_RealOptions*>(Options);
      enableZeroPadding = KissFFTOptions->enableZeroPadding;
      m_SymmetricSpectrum = KissFFTOptions->symmetricSpectrum;
    }

    // assign KissFFT options
    if (enableZeroPadding)
    {
      m_InternalFrameSize = kiss_fftr_next_fast_size_real(m_InternalFrameSize);
    }

    if (!m_SymmetricSpectrum)
    {
      m_OutFrameSize = m_InternalFrameSize/2;
    }
    else
    {
      m_OutFrameSize = m_InternalFrameSize;
      SymmetricSpectrumStart = m_InternalFrameSize/2 +1;
    }

    if (m_InternalFrameSize < m_InFrameSize)
    {// InternalFrameSize is never smaller than InFrameSize! FATAL ERROR!
      return ASPLIB_ERR_FFT_INVALID_INTERNAL_FRAME_SIZE;
    }

    // configure KissFTT
    m_FFTConfig  = kiss_fftr_alloc(m_InternalFrameSize, KISS_FFT_FORWARD, 0, 0);
    m_iFFTConfig = kiss_fftr_alloc(m_InternalFrameSize, KISS_FFT_INVERSE, 0, 0);
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
  kiss_fftr_cfg m_FFTConfig;
  kiss_fftr_cfg m_iFFTConfig;

  // extern to intern converter objects
  Extern_TO_FFTConverter m_Extern_TO_FFTConverter;
  FFT_TO_ExternConverter m_FFT_TO_ExternConverter;

  // intern to extern converter objects
  Intern_TO_FFTConverter m_Intern_TO_FFTConverter;
  FFT_TO_InternConverter m_FFT_TO_InternConverter;

  bool m_SymmetricSpectrum;
  uint32_t SymmetricSpectrumStart;
};
}
