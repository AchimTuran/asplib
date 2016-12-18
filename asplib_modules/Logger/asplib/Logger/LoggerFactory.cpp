#pragma once

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



#include "Logger/LoggerFactory.h"
#include "Logger/LoggerFile.h"
#include "Logger/LoggerStdout.h"

#include <vector>
#include <string>

using namespace std;

namespace asplib
{
ILogger* CLoggerFactory::get_Logger(ASPLIB_LOGGER_TYPES LoggerType, void *Specific)
{
  loggerTags_t defaultTags;
  for(uint32_t tagID = ASPLIB_MIN_LOG_TAG; tagID < ASPLIB_MAX_LOG_TAG; tagID++)
  {
    defaultTags.push_back(loggerTagPair_t(tagID, defaultLoggerTags[tagID]));
  }

  return get_Logger(LoggerType, defaultTags, Specific);
}

ILogger* CLoggerFactory::get_Logger(ASPLIB_LOGGER_TYPES LoggerType, loggerTags_t &LoggerTags, void *Specific)
{
  switch(LoggerType)
  {
    case ASPLIB_FILE_LOGGER:
      if(Specific)
      {
        return new CLoggerFile(*static_cast<string*>(Specific), LoggerTags);
      }
      else
      {
        return new CLoggerFile(ASPLIB_DEFAULT_LOGGER_FILE, LoggerTags);
      }
    break;

    case ASPLIB_STDOUT_LOGGER:
      return new CLoggerStdout(LoggerTags);
    break;
  }

  return NULL;
}

void CLoggerFactory::destroy_Logger(ILogger **Logger)
{
  if(Logger && *Logger)
  {
    delete *Logger;
    *Logger = NULL;
  }
}
}
