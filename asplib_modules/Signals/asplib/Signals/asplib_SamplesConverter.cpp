/*
 * Copyright (C) 2014-2015 Achim Turan, mail@achim-turan.de
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



#include "Signals/asplib_SamplesConverter.h"
#include <memory>
#include <string.h>

bool fmtc_PCM_TO_float(unsigned char *InSamples, float *OutSamples, uint16_t BitsPerSample, uint32_t MaxSamples)
{
  if(!InSamples || !MaxSamples || !OutSamples)
  {
    return false;
  }

  // check bytes with >>3 (divide by 8)
  if(/*BitsPerSample % (1<<3) || */(uint32_t)(BitsPerSample>>3) > sizeof(uint32_t))
  {
    return false;
  }

  double halfValue = (double)((1<<(sizeof(uint32_t)*8 -1)) -1);
  const uint16_t bytes = (BitsPerSample>>3);
  const uint32_t byteOffset = sizeof(uint32_t) - bytes;
  uint32_t pSample = 0;

  for(uint32_t ii = 0; ii < MaxSamples; ii++)
  {
    memset(&pSample, 0, sizeof(uint32_t));
    for(uint32_t byte = 0; byte < bytes; byte++)
    {
      pSample |= InSamples[ii*bytes + byte] << (byte+byteOffset)*8;
    }

    OutSamples[ii] = (float)(((double)(pSample))/(halfValue));
  }

  return true;
}
