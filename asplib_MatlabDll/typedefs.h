#pragma once

/*
 * Copyright (C) 2014 Achim Turan, Achim.Turan@o2online.de
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
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#if defined(_WIN32)
    #if !defined(TARGET_WINDOWS)
        #define TARGET_WINDOWS
    #endif
#endif

// Matlab type definitions
// taken from here: http://de.mathworks.com/help/matlab/matlab_external/passing-arguments-to-shared-library-functions.html
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef float single;

//#if defined(TARGET_WINDOWS)
//    typedef long int32;
//    typedef unsigned long uint32;
//#elif defined(TARGET_LINUX)
#if defined(TARGET_LINUX)
    typedef long int64;
    typedef unsigned long uint64;
#endif

typedef enum DLL_ERROR_CODE
{
  ERR_INVALID_INPUT = -3, 
  ERR_FATAL_ERROR   = -2,
  ERR_NOT_INIT      = -1,
  ERR_NO_ERROR=0  
}RET_ERR;
