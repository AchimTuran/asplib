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



#include "SpectrumVisProcessor/Interfaces/asplib_ProcessFactory.hpp"
#include "SpectrumVisProcessor/AxisScaler/asplib_AxisScalerFactory.hpp"
#include "SpectrumVisProcessor/FFTSmoother/asplib_FFTSmootherFactory.hpp"
#include "SpectrumVisProcessor/Remapper/asplib_RemapperFactory.hpp"
#include "SpectrumVisProcessor/SpectrumCalculation/asplib_SpectrumCalculationFactory.hpp"


namespace asplib
{
REGISTER_PROCESS_FACTORY(CAxisScalerFactory, AxisScalerID_t, IProcess::PROCESS_AXIS_SCALER)
REGISTER_PROCESS_FACTORY(CFFTSmootherFactory, FFTSmootherID_t, IProcess::PROCESS_FFT_SMOOTHER)
REGISTER_PROCESS_FACTORY(CRemapperFactory, RemapperID_t, IProcess::PROCESS_REMAPPER)
REGISTER_PROCESS_FACTORY(CSpectrumCalculationFactory, SpectrumCalculationID_t, IProcess::PROCESS_SPECTRUM_CALC)

const std::string CProcessFactoryCallbacks::Name = "ProcessFactoryCallbacks";

ASPLIB_ERR CProcessFactoryCallbacks::Create(IProcess::ProcessTypes_t ProcessCategory, int32_t ProductID,
  asplibFmt_t FmtIn, asplibFmt_t FmtOut, IProcess *&InterfacePtr)
{
  MapCallbacks_t::iterator iter = GetCallbackMap().find(ProcessCategory);
  if (iter == GetCallbackMap().end())
  {
    return ASPLIB_ERR_NOT_IMPLEMENTED;
  }

  return iter->second.Create(ProductID, FmtIn, FmtOut, InterfacePtr);
}

ASPLIB_ERR CProcessFactoryCallbacks::Destroy(IProcess *&InterfacePtr)
{
  if (!InterfacePtr)
  {
    return ASPLIB_ERR_NO_ERROR;
  }

  MapCallbacks_t::iterator iter = GetCallbackMap().find(InterfacePtr->GetProcessType());
  if (iter == GetCallbackMap().end())
  {
    return ASPLIB_ERR_NOT_IMPLEMENTED;
  }

  return iter->second.Destroy(InterfacePtr);
}

ASPLIB_ERR CProcessFactoryCallbacks::GetName(IProcess::ProcessTypes_t ProcessCategory, std::string &Name)
{
  MapCallbacks_t::iterator iter = GetCallbackMap().find(ProcessCategory);
  if (iter == GetCallbackMap().end())
  {
    return ASPLIB_ERR_NOT_IMPLEMENTED;
  }

  // TODO: how to add Name?
  //Name = iter->second.Name;
  return ASPLIB_ERR_NO_ERROR;
}


int64_t CProcessFactoryCallbacks::GetActiveProducts(IProcess::ProcessTypes_t ProcessCategory, int32_t ProductID,
  asplibFmt_t FmtIn, asplibFmt_t FmtOut)
{
  MapCallbacks_t::iterator iter = GetCallbackMap().find(ProcessCategory);
  if (iter == GetCallbackMap().end())
  {
    return ASPLIB_ERR_NOT_IMPLEMENTED;
  }

  return iter->second.GetActiveProducts(ProductID, FmtIn, FmtOut);
}

int64_t CProcessFactoryCallbacks::GetCreatedProducts(IProcess::ProcessTypes_t ProcessCategory, int32_t ProductID,
  asplibFmt_t FmtIn, asplibFmt_t FmtOut)
{
  MapCallbacks_t::iterator iter = GetCallbackMap().find(ProcessCategory);
  if (iter == GetCallbackMap().end())
  {
    return ASPLIB_ERR_NOT_IMPLEMENTED;
  }

  return iter->second.GetCreatedProducts(ProductID, FmtIn, FmtOut);
}

int64_t CProcessFactoryCallbacks::GetDestroyedProducts(IProcess::ProcessTypes_t ProcessCategory, int32_t ProductID,
  asplibFmt_t FmtIn, asplibFmt_t FmtOut)
{
  MapCallbacks_t::iterator iter = GetCallbackMap().find(ProcessCategory);
  if (iter == GetCallbackMap().end())
  {
    return ASPLIB_ERR_NOT_IMPLEMENTED;
  }

  return iter->second.GetDestroyedProducts(ProductID, FmtIn, FmtOut);
}


IProcess::ProcessTypes_t CProcessFactoryCallbacks::RegisterFactory(IProcess::ProcessTypes_t FactoryID, std::string Name, const FactoryCallbacks_t *Callbacks)
{
  MapCallbacks_t::iterator iter = GetCallbackMap().find(FactoryID);
  if (iter == GetCallbackMap().end())
  {// only register not yet known factories
    GetCallbackMap()[FactoryID] = *Callbacks;
  }

  return FactoryID;
}
}
