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
#include "Core/Constants_Typedefs/asplib_DataFmtDefines.hpp"
#include "Core/Exceptions/asplib_StringException.h"

#include "SpectrumVisProcessor/Interfaces/asplib_IProcess.hpp"
#include <map>

#define REGISTER_PROCESS_FACTORY(ProcessFactory, FactoryTypeID, FactoryID) \
template class CProcessFactoryCallbacks::TRegisterFactory<TProcessFactoryCallbacks<ProcessFactory, FactoryTypeID>, FactoryID>;


namespace asplib
{
template<class TFactory, typename TypeID>
class TProcessFactoryCallbacks
{
public:
  static ASPLIB_ERR Create(int32_t ProductID, asplibFmt_t FmtIn, asplibFmt_t FmtOut, IProcess *&InterfacePtr)
  {
    return TFactory::Create((TypeID)ProductID, FmtIn, FmtOut, InterfacePtr);
  }

  static ASPLIB_ERR Destroy(IProcess *&InterfacePtr)
  {
    return TFactory::Destroy(InterfacePtr);
  }


  static int64_t GetActiveProducts(int32_t ProductID, asplibFmt_t FmtIn, asplibFmt_t FmtOut)
  {
    return TFactory::GetActiveProducts((TypeID)ProductID, FmtIn, FmtOut);
  }

  static int64_t GetCreatedProducts(int32_t ProductID, asplibFmt_t FmtIn, asplibFmt_t FmtOut)
  {
    return TFactory::GetCreatedProducts((TypeID)ProductID, FmtIn, FmtOut);
  }

  static int64_t GetDestroyedProducts(int32_t ProductID, asplibFmt_t FmtIn, asplibFmt_t FmtOut)
  {
    return TFactory::GetDestroyedProducts((TypeID)ProductID, FmtIn, FmtOut);
  }


  const static std::string Name;
};

template<class TFactory, typename TypeID>
const std::string TProcessFactoryCallbacks<TFactory, TypeID>::Name = TFactory::Name;


class CProcessFactoryCallbacks
{
public:
  typedef ASPLIB_ERR  (*ProcessCreateCallback)(int32_t ProductID, asplibFmt_t FmtIn, asplibFmt_t FmtOut, IProcess *&InterfacePtr);
  typedef ASPLIB_ERR  (*ProcessDestroyCallback)(IProcess *&InterfacePtr);
  typedef int64_t     (*ProcessGetActiveProductsCallback)(int32_t ProductID, asplibFmt_t FmtIn, asplibFmt_t FmtOut);
  typedef int64_t     (*ProcessGetCreatedProductsCallback)(int32_t ProductID, asplibFmt_t FmtIn, asplibFmt_t FmtOut);
  typedef int64_t     (*ProcessGetDestroyedProductsCallback)(int32_t ProductID, asplibFmt_t FmtIn, asplibFmt_t FmtOut);

  typedef struct FactoryCallbacks_t
  {
    ProcessCreateCallback               Create;
    ProcessDestroyCallback              Destroy;
    ProcessGetActiveProductsCallback    GetActiveProducts;
    ProcessGetCreatedProductsCallback   GetCreatedProducts;
    ProcessGetDestroyedProductsCallback GetDestroyedProducts;
    // TODO: how to add Name?
    //std::string                         Name;
  }FactoryCallbacks_t;

private:
  typedef std::map<IProcess::ProcessTypes_t, FactoryCallbacks_t> MapCallbacks_t;

public:
  template<class TFactory, IProcess::ProcessTypes_t ID>
  class TRegisterFactory : public TFactory
  {
    friend class CProcessFactoryCallbacks;

  public:
    TRegisterFactory() : m_HiddenProductID(FactoryID) {} // force registration by assinging FactoryID to m_HiddenProductID

    static const IProcess::ProcessTypes_t FactoryID;

  private:
    const IProcess::ProcessTypes_t m_HiddenProductID;   // force initialization for m_HiddenProductID
    static FactoryCallbacks_t m_Callbacks;
  };

  
  static ASPLIB_ERR Create(IProcess::ProcessTypes_t ProcessCategory, int32_t ProductID, 
                           asplibFmt_t FmtIn, asplibFmt_t FmtOut, IProcess *&InterfacePtr);
  static ASPLIB_ERR Destroy(IProcess *&InterfacePtr);
  static ASPLIB_ERR GetName(IProcess::ProcessTypes_t ProcessCategory, std::string &Name);

  // TODO: return value should be the type ASPLIB_ERR!
  static int64_t GetActiveProducts(IProcess::ProcessTypes_t ProcessCategory, int32_t ProductID,
                                   asplibFmt_t FmtIn, asplibFmt_t FmtOut);
  // TODO: return value should be the type ASPLIB_ERR!
  static int64_t GetCreatedProducts(IProcess::ProcessTypes_t ProcessCategory, int32_t ProductID,
                                    asplibFmt_t FmtIn, asplibFmt_t FmtOut);
  // TODO: return value should be the type ASPLIB_ERR!
  static int64_t GetDestroyedProducts(IProcess::ProcessTypes_t ProcessCategory, int32_t ProductID,
                                      asplibFmt_t FmtIn, asplibFmt_t FmtOut);

  
  static IProcess::ProcessTypes_t RegisterFactory(IProcess::ProcessTypes_t FactoryID, std::string Name, const FactoryCallbacks_t *Callbacks);

private:
  inline static MapCallbacks_t& GetCallbackMap()
  {
    static MapCallbacks_t s_Callbacks;

    return s_Callbacks;
  }

  const static std::string  Name;
};


template<class TFactory, IProcess::ProcessTypes_t ID>
const IProcess::ProcessTypes_t CProcessFactoryCallbacks::TRegisterFactory<TFactory, ID>::FactoryID = CProcessFactoryCallbacks::RegisterFactory(ID, TFactory::Name, &CProcessFactoryCallbacks::TRegisterFactory<TFactory, ID>::m_Callbacks);

template<class TFactory, IProcess::ProcessTypes_t ID>
typename CProcessFactoryCallbacks::FactoryCallbacks_t CProcessFactoryCallbacks::TRegisterFactory<TFactory, ID>::m_Callbacks =
{
  TFactory::Create,
  TFactory::Destroy,
  TFactory::GetActiveProducts,
  TFactory::GetCreatedProducts,
  TFactory::GetDestroyedProducts
  // TODO: how to add Name?
  /*,
  TFactory::Name*/
};
}
