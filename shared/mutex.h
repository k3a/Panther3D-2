/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 08/29/2009 File created - Petr
*/

#pragma once

#if defined(WIN32)
#include <windows.h>
#include <process.h>
#elif defined(LINUX)
#include <pthread.h>
#endif

#include "symbols.h"

namespace P3D
{
	/// \brief : Represents a mutex.
	/// Mutexes are being used to lock/unlock the shared variables among the threads.
	class CMutex
	{
	public:
		/// Constructor for the class.
		CMutex();
		/// Destructor for the class.
		~CMutex();
		/// Locks the mutex.
		void Lock();
		/// Unocks the mutex.
		void Unlock();
		/// Tries to lock the mutex and returns whether the lock was successful, so you can do some other stuff while waiting to lock the mutex.
		/// Example:
		/// @code
		/// RNL::CMutex mutex;
		///	while(!mutex.TryLock())
		///	{
		//		// do some stuff... 
		///	}
		/// ...
		/// @endcode
		bool TryLock();
	private:
		#if defined(WIN32)
		CRITICAL_SECTION m_handle;
		#elif defined(LINUX)
		pthread_mutex_t m_mutex;
		#endif

	};

}
