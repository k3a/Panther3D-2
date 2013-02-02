/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 08/29/2009 File created - Petr
*/

#include "precompiled.h"
#include "thread.h"


namespace P3D
{

	#if defined(WIN32)
	unsigned int __stdcall CThread::ThreadMainFunction(void* arglist)
	#elif defined(LINUX)
	void* CThread::ThreadMainFunction(void* arglist)
	#endif
	{	
		CRunnable *runnable = (CRunnable*)arglist;

		#if defined(LINUX)
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
		#endif

		if(!runnable->Init()) 
		{
			_endthreadex(1);
			return 1;
		}
		runnable->Run();
		runnable->Exit();

		_endthreadex(0);
		return 0;
	}
	
//---------------------------------------------------------------------------
/// Short description
/** Detailed description
 * 
 * \param runnable 
 * \param desc 
 * \param deleteRunnable 
 * \return  
 */
/// Short description
/** Detailed description
 * 
 * \param runnable 
 * \param desc 
 * \param deleteRunnable 
 * \return  
 */
/// Short description
/** Detailed description
 * 
 * \param runnable 
 * \param desc 
 * \param deleteRunnable 
 * \return  
 */
/// Short description
/** Detailed description
 * 
 * \param runnable 
 * \param desc 
 * \param deleteRunnable 
 * \return  
 */
	CThread::CThread(CRunnable *runnable, const sThreadDesc &desc, bool deleteRunnable)
	{
		m_pRunnable = runnable;
		m_desc = desc;
		m_deleteRunnable = deleteRunnable;
		m_bActive = false;
	}
	
//---------------------------------------------------------------------------
	CThread::~CThread()
	{
		if(m_bActive) Wait();
		if(m_deleteRunnable) SAFE_DELETE(m_pRunnable);
	}

//---------------------------------------------------------------------------
	bool CThread::Launch()
	{
		#if defined(WIN32)
		m_handle = (HANDLE)_beginthreadex(NULL, m_desc.stackSize, &CThread::ThreadMainFunction, m_pRunnable, 0, (UINT*)&m_id); //the last parameter is an identifier, might be useful
		if(!m_handle) return false;

		THREADNAME_INFO info;
		{
			info.dwType = 0x1000;
			info.szName = m_desc.name;
			info.dwThreadID =  m_id; //GetCurrentThreadId();;
			info.dwFlags = 0;
		}
		__try
		{
			RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info );
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{
		}

		#elif defined(LINUX)
		pthread_attr_t *attr = 0;
		if(pthread_attr_init(attr)) return false;
		if(m_sTArg.stackSize)
		{
			if(pthread_attr_setstacksize(attr, m_sTArg.stackSize)) return false;
		}
		if(pthread_create(&m_thread, NULL, &CThread::ThreadMainFunction, &m_sTArg)) return false;
		if(pthread_attr_destroy(attr)) return false;
		#endif

		SetPriority(m_desc.priority);

		m_bActive = true;
		return true;
	}

//---------------------------------------------------------------------------
	bool CThread::Terminate()
	{
		if (m_bActive)
		{
			#if defined(WIN32)
				if(!TerminateThread(m_handle, 1)) return false;
				CloseHandle(m_handle);
			m_handle = NULL;
			#elif defined(LINUX)
				if(pthread_cancel(m_thread)) return false;
			#endif

			m_pRunnable->Terminate(); //it is safe to access the runnable now
			m_bActive = false;
			return true;
		}

		return false;
	}

//---------------------------------------------------------------------------
	bool CThread::Wait(ULONG timems)
	{
		if (m_bActive)
		{
			#if defined(WIN32)
			DWORD response = WaitForSingleObject(m_handle, timems);	
			if(response == WAIT_FAILED || response == WAIT_TIMEOUT) return false;
			//CloseHandle(m_handle); //_endthreadex() should do this
			m_handle = NULL;
			#elif defined(LINUX)
			if(pthread_join(m_thread, timems)) return false;
			#endif

			m_bActive = false;
			return true;
		}

		return false;
	}

//---------------------------------------------------------------------------
	bool CThread::SetPriority(eThreadPriority priority)
	{
		int intPriority;

		switch(priority)
		{
		case P3D_THREAD_PRIORITY_IDLE:
			intPriority = THREAD_PRIORITY_IDLE;
			break;
		case P3D_THREAD_PRIORITY_LOWEST:
			intPriority = THREAD_PRIORITY_LOWEST;
			break;
		case P3D_THREAD_PRIORITY_LOW:
			intPriority = THREAD_PRIORITY_BELOW_NORMAL;
			break;
		case P3D_THREAD_PRIORITY_NORMAL:
			intPriority = THREAD_PRIORITY_NORMAL;
			break;
		case P3D_THREAD_PRIORITY_HIGH:
			intPriority = THREAD_PRIORITY_ABOVE_NORMAL;
			break;
		case P3D_THREAD_PRIORITY_HIGHEST:
			intPriority = THREAD_PRIORITY_HIGHEST;
			break;
		default:
			return false;
		}

		return TRUE==SetThreadPriority(m_handle, intPriority);

	}

//---------------------------------------------------------------------------
	ULONG CThread::GetId()
	{
		if(!m_bActive) return 0;
		#if defined(WIN32)
			return GetThreadId(m_handle);
		#elif defined(LINUX)
			return m_thread;
		#endif
	}

//---------------------------------------------------------------------------
	ULONG CThread::GetCurrentThreadID()
	{
		#if defined(WIN32)
			return GetCurrentThreadId();
		#elif defined(LINUX)
			return pthread_self();
		#endif
	}

}
