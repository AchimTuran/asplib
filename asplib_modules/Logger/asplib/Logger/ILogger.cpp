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

#include "Logger/ILogger.h"

#include <stdarg.h>
#include <string>
#include <time.h>

using namespace std;

namespace asplib
{

// helper function prototypes
string get_DateTimeStr();

ILogger::ILogger(loggerTags_t &LoggerTags)
{
  if(LoggerTags.size() <= 0)
  {
    throw ASPLIB_STRING_EXCEPTION_HANDLER("Invalid input! No Logger Tags found!");
  }

  // get logger tag IDs
  int32_t greatesID = -1;
  for(uint32_t ii=0; ii < LoggerTags.size(); ii++)
  {
    if((int32_t)LoggerTags[ii].first > greatesID)
    {
      greatesID = LoggerTags[ii].first;
    }
  }

  // initialize logger tag table
  for(uint32_t ii = 0; ii < (uint32_t)(greatesID +1); ii++)
  {
    m_LoggerTagIDTable.push_back(-1);
  }

  // assigne logger tag IDs
  for(uint32_t ii = 0; ii < LoggerTags.size(); ii++)
  {
    m_LoggerTagIDTable[LoggerTags[ii].first] = ii;
    m_LoggerTags.push_back(LoggerTags[ii]);
  }
}

ILogger::~ILogger()
{
}


ASPLIB_ERR ILogger::Log(const uint32_t TagID, const std::string Message, ...)
{
  // first generate the date string
  string dateStr = get_DateTimeStr();

  ASPLIB_ERR asplibErr = Open();
  if(asplibErr != ASPLIB_ERR_NO_ERROR)
  {
    return asplibErr;
  }

  va_list variadicArgs;
  va_start(variadicArgs, Message);
  asplibErr = LogWrite(TagID, dateStr, Message, variadicArgs);
  va_end(variadicArgs);
  if(asplibErr != ASPLIB_ERR_NO_ERROR)
  {
    return asplibErr;
  }
  
  return Close();
}

string ILogger::get_LoggerTagStr(const uint32_t TagID)
{
  if(TagID >= (uint32_t)m_LoggerTagIDTable.size())
  {
    return string("[Unknown]");
  }

  int32_t translatedID = m_LoggerTagIDTable[TagID];
  if(translatedID < 0)
  {
    return string("[Unknown]");
    
  }
  
  return m_LoggerTags[translatedID].second;
}

// ---------------------------
// Helper Functions
// ---------------------------
string get_DateTimeStr()
{
  time_t timeNow;
  time(&timeNow);
  struct tm calendarTime;
  char timeBuf[256];

#if defined(TARGET_WINDOWS)
  localtime_s(&calendarTime, &timeNow);
#elif defined(TARGET_LINUX)
  localtime_r(&timeNow, &calendarTime);
#else
  #warning "Unknown Target! Time function is not implemeted."
  return "UnknownDate";
#endif

  // Time format: Dayname Day. Month. Year, HH:MM:SS
  // for details see http://www2.hs-fulda.de/~klingebiel/c-stdlib/time.htm
  strftime(timeBuf, 256, "%a %d. %b. %Y, %H:%M:%S", &calendarTime);

  return string(timeBuf);
}
}
