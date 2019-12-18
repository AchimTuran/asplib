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



#include <iostream>
using namespace std;

#include "Timer/asplib_CPUTimer.h"
#include "Core/Constants_Typedefs/asplib_Constants.h"

namespace asplib
{
CCPUTimer::CCPUTimer()
{
  m_activeTimer = false;
  m_completeMeasure = false;

#if defined(TARGET_WINDOWS)
  if(!QueryPerformanceFrequency(&m_frequency))
	{
	  cout << "Warning can't initialize timer!" << endl;
	}
#endif
}

CCPUTimer::~CCPUTimer()
{
}

void CCPUTimer::start_Timer()
{
  if(m_activeTimer)
  {
    cout << "Warning in function " << __ASPLIB_FUNCTION__ << " : time measurement is running! Please stop this measurement before running a new one." << endl;
  }
  else
  {
    m_activeTimer = true;
    if(m_completeMeasure)
    {
      cout << "Warning in function " << __ASPLIB_FUNCTION__ << ": old time measure value lost!" << endl;
    }
    m_completeMeasure = false;

#if defined(TARGET_WINDOWS)
    QueryPerformanceCounter(&m_t1);
#else
    //gettimeofday(&m_startTime, 0);
    clock_gettime(CLOCK_REALTIME, &m_startTime);
#endif
  }
}

void CCPUTimer::stop_Timer()
{
  if(!m_activeTimer)
  {
    cout << "Warning in function " << __ASPLIB_FUNCTION__ << ": no time measurement was running! Please run a time time measuremnt first" << endl;
  }
  else
  {
    m_activeTimer = false;
    if(m_completeMeasure)
    {
      cout << "Warning: old time measure value lost!" << endl;
    }
    m_completeMeasure = true;

#if defined(TARGET_WINDOWS)
    QueryPerformanceCounter(&m_t2);
#else
    //gettimeofday( &m_endTime, 0 );
    clock_gettime(CLOCK_REALTIME, &m_endTime);
#endif
  }
}

bool CCPUTimer::is_TimerRunning()
{
  return m_activeTimer;
}

double CCPUTimer::get_ElapsedTime()
{
  if(m_activeTimer)
  {
    cout << "Warning in function " << __ASPLIB_FUNCTION__ << ": no complete time measurement available! Timer measurement is running" << endl;
    return -1.0f;
  }

  if(!m_completeMeasure)
  {
    cout << "Warning in function " << __ASPLIB_FUNCTION__ << ": no complete time measurement available!" << endl;
    return -1.0f;
  }
  else
  {
    m_completeMeasure = false;

#if defined(TARGET_WINDOWS)
    return ((double)(m_t2.QuadPart-m_t1.QuadPart))/((double)(m_frequency.QuadPart));
#else
  timespec timeDiff;
    if ( m_endTime.tv_nsec - m_startTime.tv_nsec < 0 )
    {
      timeDiff.tv_sec = m_endTime.tv_sec - m_startTime.tv_sec -1;
      timeDiff.tv_nsec = 1000000000 + m_endTime.tv_nsec - m_startTime.tv_nsec;
    }
    else
    {
      timeDiff.tv_sec = m_endTime.tv_sec - m_startTime.tv_sec;
      timeDiff.tv_nsec = m_endTime.tv_nsec - m_startTime.tv_nsec;
    }

    return (double)timeDiff.tv_sec + ((double)timeDiff.tv_nsec)*E_m9;
#endif
  }
}

void CCPUTimer::pause_Timer()
{
}

void CCPUTimer::print_ElapsedTime()
{
  if(!m_completeMeasure)
  {
    cout << "Warning in function " << __ASPLIB_FUNCTION__ << ": no complete time measurement available!" << endl;
  }
  else
  {
#if defined(TARGET_WINDOWS)
    cout << "elapsed time: " << ((double)(m_t2.QuadPart-m_t1.QuadPart))/((double)(m_frequency.QuadPart)) << "s" << endl;
#else
    timespec diff;
    if (m_endTime.tv_nsec - m_startTime.tv_nsec < 0 )
    {
      diff.tv_sec = m_endTime.tv_sec - m_startTime.tv_sec -1;
      diff.tv_nsec = 1000000000 + m_endTime.tv_nsec - m_startTime.tv_nsec;
    }
    else
    {
      diff.tv_sec = m_endTime.tv_sec - m_startTime.tv_sec;
      diff.tv_nsec = m_endTime.tv_nsec - m_startTime.tv_nsec;
    }

    const double elapsedTime = (double)diff.tv_sec + ((double)diff.tv_nsec)*E_m9;
    cout << "elapsed time: " << elapsedTime << "s" << endl;
#endif
  }
}
}
