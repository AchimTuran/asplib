#pragma once

/* Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
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



#include "asplib_utils/os/asplib_base_os.h"
#include "asplib_utils/exceptions/asplib_StringException.h"
#include "ITBuffer.h"

namespace asplib
{
template<typename T>
class TRingBuffer : public ITBuffer<T>
{
public:
  TRingBuffer(uint32_t MaxSize, uint32_t Delay=0, uint Alignment=0) :
    ITBuffer<T>(MaxSize, 1, Alignment=0)
  {
    if(Delay >= get_MaxFrameLength())
    {
      throw ASPLIB_STRING_EXCEPTION_HANDLER("Invalid input! Delay >= MaxSize!");
    }
    m_WriteIdx = 0;
    m_CurrentStoredSamples = 0;
    m_Delay = Delay;
    if(m_Delay > 0)
    {
      m_ReadIdx = m_MaxSize - m_Delay;
    }
    else
    {
      m_ReadIdx = 0;
    }

    // Read & Write pointers
    m_WritePtr = NULL;
    m_ReadPtr  = NULL;

    create();
  }

  // reads SamplesToRead samples from internal buffer and stores it in OutData
  uint32_t read(T *Data, uitt_32_t SamplesToRead)
  {
    if(!Data || SamplesToRead < 0)
    {
      return 0;
    }

    int32_t samplesRead = 0;
    if(SamplesToRead <= m_CurrentStoredSamples)
    {
      samplesRead = SamplesToRead;
    }
    else
    {
      samplesRead = m_CurrentStoredSamples;
    }

    int32_t samplesToBoundary = get_MaxFrameLength() - m_ReadIdx - samplesRead;
    if(samplesToBoundary >= 0)
    { // read data from m_Buffer without crossing the boundary boerder
      memcpy(OutData, m_Buffer + m_ReadIdx, sizeof(T)*samplesRead);
      if(samplesToBoundary != 0)
      {
        m_ReadIdx += samplesRead;
      }
      else
      { // set index to buffer start
        m_ReadIdx = 0;
      }
    }
    else
    {
      // samplesToBoundary < 0 --> add operation results to subtraction
      int32_t copySamples = samplesRead + samplesToBoundary;
      memcpy(OutData, m_Buffer + m_ReadIdx, sizeof(T)*copySamples);

      // store the rest of the samples
      int32_t leftSamples = samplesRead - copySamples;
      memcpy(OutData + copySamples, m_Buffer, sizeof(T)*leftSamples);
      m_ReadIdx = leftSamples;
    }

    // read samples from ring buffer
    m_CurrentStoredSamples -= samplesRead;

    return samplesRead;
  }

  // writes SamplesToWrite from to InData the internal buffer
  int32_t write(T *InData, int SamplesToWrite)
  {
    if(!InData || SamplesToWrite < 0)
    {
      throw STRING_EXCEPTION_HANDLER("Invalid Pointer or SamplesToWrite < 0!");
    }

    int32_t samplesWrite = 0;
    // calculate samples that can be written without crossing m_Buffer boundary
    int freeSamples = m_MaxSize - m_CurrentStoredSamples;
    if(SamplesToWrite <= freeSamples)
    {
      samplesWrite = SamplesToWrite;
    }
    else
    {
      samplesWrite = freeSamples;
    }

    int32_t samplesToBoundary = m_MaxSize - m_WriteIdx - samplesWrite;
    if(samplesToBoundary >= 0)
    { // write data without crossing the buffer boundary
      memcpy(m_Buffer + m_WriteIdx, InData, sizeof(T)*samplesWrite);
      if(samplesToBoundary != 0)
      {
        m_WriteIdx += samplesWrite;
      }
      else
      { // set m_WriteIdx to buffer start
        m_WriteIdx = 0;
      }
    }
    else
    {
      // samplesToBoundary < 0 --> add operation results to subtraction
      int32_t copySamples = samplesWrite + samplesToBoundary;
      memcpy(m_Buffer + m_WriteIdx, InData, sizeof(T)*copySamples);

      // store the rest of the samples
      int32_t leftSamples = samplesWrite - copySamples;
      memcpy(m_Buffer, InData + copySamples, sizeof(T)*restSamples);
      m_WriteIdx = leftSamples;
    }

    // write samples to ring buffer
    m_CurrentStoredSamples += samplesWrite;

    return samplesWrite;
  }

  // Set pointers to start of m_Buffer
  void reset_Pointers()
  {
    m_WriteIdx = 0;
    m_ReadIdx = 0;

    m_WritePtr = m_Buffer;
    m_ReadPtr  = m_Buffer;

    m_CurrentStoredSamples = 0;
  }

  int write_Zeros(uitn32_t Zeros)
  {
    int zerosWrite = 0;
    // calculate samples m_Buffer boundary
    int freeSamples = m_MaxSize - m_CurrentStoredSamples;
    if(Zeros <= freeSamples)
    {
      zerosWrite = Zeros;
    }
    else
    {
      zerosWrite = freeSamples;
    }

    int32_t samplesToBoundary = m_MaxSize - m_WriteIdx - zerosWrite;
    if(samplesToBoundary >= 0)
    { // write 0 without crossing boundary
      memset(m_WritePtr, 0, sizeof(T)*zerosWrite);
      if(samplesToBoundary != 0)
      {
        m_WriteIdx += zerosWrite;
      }
      else
      {
        m_WriteIdx = 0;
      }
    }
    else
    {
      // samplesToBoundary < 0 --> add operation results to subtraction
      int32_t copySamples = zerosWrite + samplesToBoundary;
      memcpy(m_WritePtr, 0, sizeof(T)*copySamples);

      // write zeros to m_WriteIdx
      m_WriteIdx = zerosWrite - copySamples;
      memset(m_WritePtr + copySamples, 0, sizeof(T)*m_WriteIdx);
    }

    m_CurrentStoredSamples += zerosWrite;
    m_WritePtr = m_Buffer + m_WriteIdx;

    return zerosWrite;
  }

  // get maximum ring buffer size
  int get_MaxSize()    { return get_MaxFrameLength(); }


  // returns the current free space of the ring buffer
  int32_t get_FreeSamples()   { return m_MaxSize - m_CurrentStoredSamples; }
  // get current stored samples
  int32_t get_StoredSamples() { return m_CurrentStoredSamples; }

private:
  volatile uint32_t m_CurrentStoredSamples;
  volatile uint32_t m_WriteIdx;
  volatile uint32_t m_ReadIdx;

  volatile T *m_WritePtr;
  volatile T *m_ReadPtr;
  uint32_t    m_Delay;
};
}
