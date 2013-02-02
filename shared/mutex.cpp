/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 08/29/2009 File created - Petr
*/

#include "precompiled.h"

#include "mutex.h"


namespace P3D
{

	CMutex::CMutex()
	{
		#if defined(WIN32)
		InitializeCriticalSection(&m_handle);
		#elif defined(LINUX)
		pthread_mutex_init(&m_mutex, NULL);
		#endif
	}

	CMutex::~CMutex()
	{
		#if defined(WIN32)
		DeleteCriticalSection(&m_handle);
		#elif defined(LINUX)
		pthread_mutex_destroy(&m_mutex);
		#endif
	}

	void CMutex::Lock()
	{
		#if defined(WIN32)
		EnterCriticalSection(&m_handle);
		#elif defined(LINUX)
		pthread_mutex_lock(&m_mutex);
		#endif
	}

	void CMutex::Unlock()
	{
		#if defined(WIN32)
		LeaveCriticalSection(&m_handle);
		#elif defined(LINUX)
		pthread_mutex_unlock(&m_mutex);
		#endif
	}

	bool CMutex::TryLock()
	{
		#if defined(WIN32)
		return (bool)TryEnterCriticalSection(&m_handle);
		#elif defined(LINUX)
		return !(bool)pthread_mutex_trylock(&m_mutex);
		#endif
	}

}
