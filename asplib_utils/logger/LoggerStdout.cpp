/*
 * Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
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



#include "LoggerStdout.h"
#include "asplib_utils/exceptions/asplib_StringException.h"

#include <string>
#include <stdio.h>
using namespace std;

namespace asplib
{
CLoggerStdout::CLoggerStdout(loggerTags_t &LoggerTags) :
  ILogger(LoggerTags)
{
}

ASPLIB_ERR CLoggerStdout::Open()
{
  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CLoggerStdout::Close()
{
  return ASPLIB_ERR_NO_ERROR;
}

ASPLIB_ERR CLoggerStdout::LogWrite(const uint32_t TagID, const std::string DateStr, std::string Message, va_list VarArgs)
{
  printf("%s %s", DateStr.c_str(), get_LoggerTagStr(TagID).c_str());
  vprintf(Message.c_str(), VarArgs);
  printf("\n");

  return ASPLIB_ERR_NO_ERROR;
}
}
