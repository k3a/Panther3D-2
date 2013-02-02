/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "profiler.h"

#include <fstream> //TEMPORARY!!

namespace P3D
{
	
	CProfiler::sSample CProfiler::m_samples[MAX_PROFILER_SAMPLES];

	CProfiler::CProfiler(const wchar *name)
	{
		m_name.SetEx(name);
		m_availableSample = 0;
		m_availableParentID = -1;
		m_numActiveSamples = 0;
	}

	unsigned int CProfiler::BeginSample(const wchar *name, const wchar *filename, const wchar *funcname, unsigned int startline)
	{
		for(unsigned int i=0; i<m_availableSample; i++)
		{
			if(wstrcmp(m_samples[i].m_name.Get(), name) == 0) 
			{
				m_samples[i].m_parentID = m_availableParentID;	
				m_availableParentID = i;
				return i;
			}
		}

		m_samples[m_availableSample].m_name.SetEx(name);
		m_samples[m_availableSample].m_file.SetEx(filename);
		m_samples[m_availableSample].m_function.SetEx(funcname);
		m_samples[m_availableSample].m_parentID = m_availableParentID;
		m_samples[m_availableSample].m_numParents = m_numActiveSamples;
		//TODO: check whether is still active
		m_availableParentID = m_availableSample;

		m_numActiveSamples++;

		m_availableSample++;
		return (m_availableSample-1);
	}

	void CProfiler::EndSample(float time, unsigned int id)
	{
		m_samples[id].m_totalTime += time;
		m_samples[id].m_totalCycles += 1;

		m_availableParentID = m_samples[id].m_parentID;
		m_numActiveSamples = m_samples[id].m_numParents;
	}

	void CProfiler::Process()
	{

		std::ofstream fout("../data/profiler/prof.html");

		fout << "<table>";
		fout << "<tr><th>NAME</th></tr>";
		fout << "<tr><th>Sample</th><th>Time AVG</th><th>Function</th><th>File</th></tr>";
		for(unsigned int i=0; i<m_availableSample; i++)
		{ 
			float time = (float)(m_samples[i].m_totalTime / m_samples[i].m_totalCycles);
			fout << "<tr>";
			fout << "<td>";
			for(unsigned int y=0; y<m_samples[i].m_numParents; y++) fout << "&nbsp ";
			fout << _W2A(m_samples[i].m_name.Get()) << "</td><td>" << time << "</td><td>" << _W2A(m_samples[i].m_function.Get()) << "</td><td>" << _W2A(m_samples[i].m_file.Get()) << "</td>";
			fout << "</tr>";
		}
		fout << "</table>";

	}


	/////////////////////////////////

	CProfileSample::CProfileSample(const wchar *name, CProfiler *profiler, const wchar *filename, const wchar *funcname, unsigned int startline)
	{
		m_profiler = profiler;
		m_id = m_profiler->BeginSample(name, filename, funcname, startline);
		if (!QueryPerformanceFrequency((LARGE_INTEGER *)&m_frequency))
		{
			//high res timer not supported (I really hope this cannot happen nowadays)
		}

		QueryPerformanceCounter((LARGE_INTEGER *)&m_performanceTimerStart);
		// Calculate The Timer Resolution Using The Timer Frequency
		m_resolution		= (float) (((double)1.0f)/((double)m_frequency));
		m_running = true;
	}

	CProfileSample::~CProfileSample()
	{
		Stop();
	}

	void CProfileSample::Stop()
	{
		if(!m_running) return;
		QueryPerformanceCounter((LARGE_INTEGER *) &m_performanceTimerEnd);
		m_time = (float)((m_performanceTimerEnd - m_performanceTimerStart) * m_resolution)*1000.0f;//to get ms
		//add sample
		m_profiler->EndSample(m_time, m_id);
		m_running = false;
	}



};
