/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 08/29/2009 File created - Petr Pavlik
*/

#pragma once

#if defined(WIN32)
#include <windows.h>
#include <process.h>
#elif defined(LINUX)
#include <pthread.h>
#include <cstring>
#endif

#include "symbols.h"

namespace P3D
{

	///Represents priority of a thread
	enum eThreadPriority
	{
		P3D_THREAD_PRIORITY_IDLE,
		P3D_THREAD_PRIORITY_LOWEST,
		P3D_THREAD_PRIORITY_LOW,
		P3D_THREAD_PRIORITY_NORMAL,
		P3D_THREAD_PRIORITY_HIGH,
		P3D_THREAD_PRIORITY_HIGHEST
	};

	typedef void (*ThreadFunction)(void*);

	///Runnable object are meant to run on an arbitrary thread
	class CRunnable
	{
	public:
		///The thread loop is meant to be implemented in this method.
		virtual void Run()=0;
		///Is called right before the Run() method which will not start if this method returns false.
		virtual bool Init() { return true; };
		///Is called right after the Run() method.
		virtual void Exit() {};
		///Is called when the thread is forced to terminate.
		virtual void Terminate() {};
		///Virtual destructor
		virtual ~CRunnable() {};
	};

	///Description for a thread
	struct sThreadDesc
	{
		sThreadDesc()
		{
			priority = P3D_THREAD_PRIORITY_NORMAL;
			stackSize = 0;
			strcpy(name, "P3D Thread");
		}
		
		///Priority of a thread
		eThreadPriority priority;
		///Stack size of a thread. Default value is 0.
		int stackSize;
		///Name of a thread.
		char name[64];
	};

	///Represents a thread.
	class CThread
	{
	public:
		/**
		\brief
		Constructor
		\param runnable
		Pointer to an instance of a class derived from CRunnable.
		\param desc
		\see sThreadDesc
		\param deleteRunnable
		Whether to auto delete the given runnable.
		*/
		CThread(CRunnable *runnable, const sThreadDesc &desc, bool deleteRunnable = false);
		/// Destructor
		~CThread();
		/// Launches the thread.
		bool Launch();
		/**
		Terminates the thread.
		\warning
		This method does not let the thread to finish and therefore is not safe.
		*/
		bool Terminate();
		/** 
		\brief
		Wait for the thread to finish.
		\param timems
		Maximal amount of time in milliseconds to wait for the thread to exit.
		*/
		bool Wait(ULONG timems = ULONG_MAX);

		bool SetPriority(eThreadPriority priority);
		eThreadPriority GetPriority();
		DWORD GetId();
		static DWORD GetCurrentThreadID();

	private:
		
		bool m_bActive;
		CRunnable *m_pRunnable; //Do NOT touch this while the thread is running
		sThreadDesc m_desc;
		bool m_deleteRunnable;

		//the main thread function
		#if defined(WIN32)
		static unsigned int __stdcall ThreadMainFunction(void* arglist);
		#elif defined(LINUX)
		static void* ThreadMainFunction(void* arglist);
		#endif
		

		#if defined(WIN32)
		HANDLE   m_handle;
		DWORD m_id;

		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType; // must be 0x1000

			LPCSTR szName; // pointer to name (in user addr space)

			DWORD dwThreadID; // thread ID (-1=caller thread)

			DWORD dwFlags; // reserved for future use, must be zero

		} THREADNAME_INFO;

		#elif defined(LINUX)
		pthread_t m_thread;
		#endif

	};

}