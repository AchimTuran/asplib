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



#include "Core/Exceptions/asplib_StringException.h"

#include "Logger/LoggerFile.h"

#include <string>
#include <stdarg.h>
using namespace std;

namespace asplib
{
CLoggerFile::CLoggerFile(string FilePath, loggerTags_t &LoggerTags) :
  ILogger(LoggerTags)
{
  if(FilePath.empty() || FilePath == "")
  {
    throw ASPLIB_STRING_EXCEPTION_HANDLER("Invalid input! Invalid file path string!");
  }

  m_FilePath = FilePath;
  m_FilePtr = fopen(m_FilePath.c_str(), "w");
  if(!m_FilePtr)
  {
    throw ASPLIB_STRING_EXCEPTION_HANDLER("Invalid input! Invalid file path string!");
  }
  fprintf(m_FilePtr, "");
  fclose(m_FilePtr);
  m_FilePtr = NULL;
}

CLoggerFile::~CLoggerFile()
{
  if(m_FilePtr)
  {
    fclose(m_FilePtr);
    m_FilePtr = NULL;
  }
}

ASPLIB_ERR CLoggerFile::Open()
{
  m_FilePtr = fopen(m_FilePath.c_str(), "a");
  if(!m_FilePtr)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CLoggerFile::Close()
{
  if(m_FilePtr)
  {
    fclose(m_FilePtr);
    m_FilePtr = NULL;
  }

  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CLoggerFile::LogWrite(const uint32_t TagID, const std::string DateStr, std::string Message, va_list VarArgs)
{
  if(!m_FilePtr)
  {
    return ASPLIB_ERR_INVALID_INPUT;
  }

  // TODO check errors from fprinf(...) && vfprintf(...)
  fprintf(m_FilePtr, "%s %s", DateStr.c_str(), get_LoggerTagStr(TagID).c_str());
  vfprintf(m_FilePtr, Message.c_str(), VarArgs);
  fprintf(m_FilePtr, "\n");

  return ASPLIB_ERR_NO_ERROR;
}
}
