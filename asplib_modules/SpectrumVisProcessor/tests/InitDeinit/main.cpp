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



#include <iostream>

#include "SpectrumVisProcessor/asplib_SpectrumVisProcessor.hpp"
#include "SpectrumVisProcessor/asplib_SpectrumVisProcessorConfigurator.hpp"

using namespace std;
using namespace asplib;


int main()
{
  const uint32_t frameSize = 128;
  ASPLIB_ERR err = ASPLIB_ERR_NO_ERROR;

  float in[frameSize];
  float out[frameSize];
  memset(in, 0, sizeof(float)*frameSize);
  memset(out, 0, sizeof(float)*frameSize);

  CSpectrumVisProcessor processor;
  CSpectrumVisProcessorConfigurator config;
  err = processor.Create(config, frameSize);
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }
  
  for (int ii = 0; ii < 100; ii++)
  {
    err = processor.Process(in, out);
    if (err != ASPLIB_ERR_NO_ERROR)
    {
      return processor.Destroy();
    }
  }

  err = processor.Destroy();
  if (err != ASPLIB_ERR_NO_ERROR)
  {
    return err;
  }

  return 0;
}
