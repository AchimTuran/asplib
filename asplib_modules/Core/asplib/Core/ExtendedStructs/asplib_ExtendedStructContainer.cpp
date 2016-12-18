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
#include "Core/ExtendedStructs/asplib_ExtendedStructContainer.hpp"
#include "Core/ExtendedStructs/asplib_ExtendedStructs.hpp"


namespace asplib
{
CExtendedStructContainer::CExtendedStructContainer()
{
  m_Data          = NULL;
  m_StructSize    = 0;
  m_ReservedSize  = 0;

  m_StructID      = 0;
}


CExtendedStructContainer::CExtendedStructContainer(const CExtendedStructContainer &Container)
{
  this->m_StructSize   = Container.m_StructSize;
  this->m_ReservedSize = Container.m_ReservedSize;
  this->m_StructID     = Container.m_StructID;
  this->m_Data         = new uint8_t[Container.m_ReservedSize];
  
  memcpy(this->m_Data, Container.m_Data, this->m_ReservedSize);
}


CExtendedStructContainer::~CExtendedStructContainer()
{
  if (m_Data)
  {
    delete [] m_Data;
    m_Data = NULL;
  }

  m_StructSize    = 0;
  m_ReservedSize  = 0;

  m_StructID      = 0;
}


CExtendedStructContainer& CExtendedStructContainer::operator = (const CExtendedStructContainer &Container)
{
  if (this->m_Data && this->m_ReservedSize < Container.m_ReservedSize)
  {
    delete [] this->m_Data;
    this->m_Data = NULL;
  }

  if (!this->m_Data && Container.m_ReservedSize > 0)
  {
    this->m_Data = new uint8_t[Container.m_ReservedSize];
  }

  this->m_StructSize = Container.m_StructSize;
  this->m_ReservedSize = Container.m_ReservedSize;
  this->m_StructID = Container.m_StructID;

  if (this->m_Data)
  {
    memcpy(this->m_Data, Container.m_Data, this->m_ReservedSize);
  }

  return *this;
}


ASPLIB_ERR CExtendedStructContainer::SaveStruct(void *Struct)
{
  if (!Struct)
  {
    if (m_ReservedSize > 0 || m_StructSize > 0)
    {
      m_ReservedSize  = 0;
      m_StructSize    = 0;
      m_StructID      = 0;

      if (m_Data)
      {
        delete [] m_Data;
        m_Data = NULL;
      }
    }

    return ASPLIB_ERR_NO_ERROR;
  }

  CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Struct);
  if (extendedStruct->ID < 0)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  m_StructID = extendedStruct->ID;
  if (m_ReservedSize <= 0 || m_ReservedSize < extendedStruct->size + extendedStruct->byteOffset)
  {
    if (m_Data)
    {
      delete [] m_Data;
      m_Data = NULL;
    }

    m_ReservedSize = extendedStruct->size + extendedStruct->byteOffset;
    m_StructSize = extendedStruct->size;

    // allocating new memory
    m_Data = new uint8_t[m_ReservedSize];
    memset(m_Data, 0, m_ReservedSize);
  }
  else
  {
    m_StructSize = extendedStruct->size;
  }

  // save the whole data, which contains extended information and the real struct data
  memcpy((uint8_t*)m_Data, (uint8_t*)Struct, m_ReservedSize);

  return ASPLIB_ERR_NO_ERROR;
}


ASPLIB_ERR CExtendedStructContainer::RestoreStruct(void *Struct)
{
  if (!m_Data || !Struct)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  CExtendedStructs *extendedStruct = static_cast<CExtendedStructs*>(Struct);
  if (m_StructID != extendedStruct->ID)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  if (m_StructSize != extendedStruct->size)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  // only copy the struct data and skip the extended info
  memcpy((uint8_t*)Struct + extendedStruct->byteOffset, m_Data + extendedStruct->byteOffset, m_StructSize);

  return ASPLIB_ERR_NO_ERROR;
}


CExtendedStructContainer::operator void*()
{
  return m_Data;
}
}
