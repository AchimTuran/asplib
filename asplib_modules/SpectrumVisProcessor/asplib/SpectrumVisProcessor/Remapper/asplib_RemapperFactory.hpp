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
#include "Core/Constants_Typedefs/asplib_DataFmtDefines.hpp"
#include "Core/AutoFactory/asplib_TAutoFactory.hpp"


#define CREATE_ASPLIB_REMAPPER_CLASS(Class, ClassID, InFmt, OutFmt, RemapperObject)   \
class Class :                                                                         \
  public TRemapperFactory::TRegisterProduct<Class, ClassID, InFmt, OutFmt>,           \
  public RemapperObject                                                               \
{                                                                                     \
public:                                                                               \
  Class() {}                                                                          \
};

#define ASPLIB_FACTORY_REMAPPER asplib::CRemapperFactory()


namespace asplib
{
class CRemapperFactoryMetaData
{
public:
  const static std::string Name;
};


typedef enum RemapperID_t
{
  ASPLIB_REMAPPER_INVALID_ID = -1,

  ASPLIB_REMAPPER_GammaCorrector,

  ASPLIB_REMAPPER_MAX_ID
}RemapperID_t;


typedef TAutoFactory<IProcess, RemapperID_t, asplibFmt_t, CRemapperFactoryMetaData> TRemapperFactory;

class CRemapperFactory : public TRemapperFactory
{
public:
  CRemapperFactory();
  ~CRemapperFactory();
};
}
