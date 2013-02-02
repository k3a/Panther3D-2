/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, amigo
*/
#pragma once

namespace P3D
{

/// High-resolution platform-independent timer.
/** Class is designed to allow very precise time measurement.
	Time is measured between last two calls of Loop() method.
	You can retrieve elapsed time in various units (seconds, miliseconds and microseconds).*/
class CTimer
{
private:
#if defined(WIN32)
	unsigned long	m_dwLastValue;			// Last performance counter value - windows only
	float			m_fFreq;				// Performance counter frequency - windows only
#else
    timeval m_lastValue;
#endif
	float			m_fElapsedTimeSec;		// Elapsed seconds between last two Loop()
	unsigned long	m_dwElapsedTimeMiliSec;	// Elapsed miliseconds between last two Loop()
	unsigned long	m_dwElapsedTimeMicroSec;// Elapsed microseconds between last two Loop()
	unsigned long	m_dwTickCount;			// Number of miliseconds from system start at the time of Loop()
public:
	CTimer();
	~CTimer();
	/// Update timer - one "tick"
	void Loop();
	/// Get time in seconds elapsed between two Loop() calls
	float GetTime_s()const{return m_fElapsedTimeSec;};
	/// Get time in miliseconds elapsed between two Loop() calls
	unsigned long GetTime_ms()const{return m_dwElapsedTimeMiliSec;};
	/// Get time in microseconds elapsed between two Loop() calls
	unsigned long GetTime_micros()const{return m_dwElapsedTimeMicroSec;};
};


}; // namespace
