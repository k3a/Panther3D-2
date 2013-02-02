/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "symbols.h"

namespace P3D
{
	
	#define MAX_PROFILER_SAMPLES 100

	class CProfiler
	{
	private:
		Str m_name;

		static struct sSample
		{
			sSample()
			{
				////m_timeAVG = -1.0f;
				m_parentID = -1;
				//m_active = false;
				m_numParents = 0;
				m_totalTime = 0;
				m_totalCycles = 0;
			}

			Str m_name;
			Str m_file;
			Str m_function;
			//bool m_active;
			////float m_timeAVG;
			int m_parentID;
			unsigned int m_numParents;
			double m_totalTime;
			unsigned int m_totalCycles;

		} m_samples[MAX_PROFILER_SAMPLES];

		unsigned int m_availableSample;
		int m_availableParentID;
		unsigned int m_numActiveSamples;

	public:
		CProfiler(const wchar *name);
		unsigned int BeginSample(const wchar *name, const wchar *filename, const wchar *funcname, unsigned int startline);
		void EndSample(float time, unsigned int id);
		void Process();
	};
	
	class CProfileSample
	{
	public:
		CProfileSample(const wchar *name, CProfiler *profiler, const wchar *filename = _W(""), const wchar *funcname = _W(""), unsigned int startline = 0);
		~CProfileSample();
		void Stop();
	private:
		unsigned int m_id;
		bool m_running;
		float	m_time;
		CProfiler *m_profiler;

		//windows timer
		#ifdef WIN32
		__int64       m_frequency;
		__int64       m_performanceTimerStart;
		__int64       m_performanceTimerEnd;
		float         m_resolution;
		#endif
	};

	////////////////////////////////////////
	//#define PROFILEENGINE

	#ifdef PROFILEENGINE
	#define PROFILESAMPLE(name) CProfileSample name(_W(#name), PROFILERLOCATION, _W(__FILE__), _W(__FUNCTION__), __LINE__)
	#define ENDPROFILESAMPLE(name) name.Stop()
	#else
	#define PROFILESAMPLE(name)
	#define ENDPROFILESAMPLE(name)
	#endif
	////////////////////////////////////////

};
