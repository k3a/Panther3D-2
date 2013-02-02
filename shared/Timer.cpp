/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, amigo
*/
#include "precompiled.h"

#include "Timer.h"
#if defined(WIN32)
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace P3D
{

CTimer::CTimer()
{
#if defined(WIN32)
	LARGE_INTEGER lint;

	m_fFreq = 1.0f;
	if (QueryPerformanceFrequency (&lint))
		m_fFreq = (float)(lint.LowPart);

	if (QueryPerformanceCounter (&lint))
		m_dwLastValue = lint.LowPart;
	else
		m_dwLastValue = GetTickCount();
#else
    if(gettimeofday(&m_lastValue, NULL) != 0) //something is wrong
    {
        //TODO: do something
    }

#endif

	// Some initial non-zero values
	m_fElapsedTimeSec = 0.001f;
	m_dwElapsedTimeMiliSec = 1;
	m_dwElapsedTimeMicroSec = 1000;
}

CTimer::~CTimer()
{

}

void CTimer::Loop()
{
#if defined(WIN32)
	LARGE_INTEGER lint;

	if (QueryPerformanceCounter (&lint))
	{
		unsigned long dwTimeDif = lint.LowPart - m_dwLastValue;
		m_fElapsedTimeSec = float(dwTimeDif) / m_fFreq;
		m_dwElapsedTimeMiliSec = (unsigned long)(m_fElapsedTimeSec * 1000.0f);
		m_dwElapsedTimeMicroSec = (unsigned long)(m_fElapsedTimeSec * 1000000.0f);
		m_dwLastValue = lint.LowPart;
	}
	else
	{
		// if high-resolution timer failed, use standard tick counter
		unsigned long dwTickCount = GetTickCount ();
		m_dwElapsedTimeMiliSec = dwTickCount - m_dwLastValue;
		m_dwElapsedTimeMicroSec = m_dwElapsedTimeMiliSec * 1000;
		m_fElapsedTimeSec =  m_dwElapsedTimeMiliSec * 0.001f;
		m_dwLastValue = dwTickCount;
	}
#else
    timeval tv;
    timeval diff;
    if(gettimeofday(&tv, NULL) != 0) //something is wrong
    {
        //TODO: do something
    }

    diff.tv_sec = tv.tv_sec - m_lastValue.tv_sec;
    diff.tv_usec = tv.tv_usec - m_lastValue.tv_usec;

    m_dwElapsedTimeMiliSec = diff.tv_sec * 1000 + diff.tv_usec / 0.001f;
    m_dwElapsedTimeMicroSec = diff.tv_sec * 1000000 + diff.tv_usec;
	m_fElapsedTimeSec =  diff.tv_sec + diff.tv_usec / 0.000001f;
	m_lastValue = tv;

#endif
}


}; // namespace
