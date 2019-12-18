#pragma once

/* Copyright (C) 2014-2016 Achim Turan, Achim.Turan@o2online.de
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

#include "Core/Constants_Typedefs/asplib_DataFmtDefines.hpp"
#include "Core/AutoFactory/asplib_TAutoFactory.hpp"

#include "Dynamics/Interfaces/asplib_IDynamics.hpp"


#define CREATE_ASPLIB_DYNAMICS_CLASS(Class, ClassID, InFmt, OutFmt, DynamicsObject)       \
class Class :                                                                             \
  public TDynamicsFactory::TRegisterProduct<Class, ClassID, InFmt, OutFmt>,               \
  public DynamicsObject                                                                   \
{                                                                                         \
public:                                                                                   \
  Class() {}                                                                              \
};

#define ASPLIB_FACTORY_DYNAMICS asplib::CDynamicsFactory()


namespace asplib
{
class CDynamicsFactoryMetaData
{
public:
  const static std::string Name;
};


typedef enum DynamicsID_t
{
  ASPLIB_DYNAMICS_INVALID_ID = -1,

  ASPLIB_DYNAMICS_Compressor,

  ASPLIB_DYNAMICS_MAX_ID
}DynamicsID_t;


typedef TAutoFactory<IDynamics, DynamicsID_t, asplibFmt_t, CDynamicsFactoryMetaData> TDynamicsFactory;

class CDynamicsFactory : public TDynamicsFactory
{
public:
  CDynamicsFactory();
  ~CDynamicsFactory();
};
}


