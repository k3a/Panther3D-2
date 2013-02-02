/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
MultiThreading tests
*/
#include "precompiled.h"

#include "tests.h"
#include "platform.h"
#include "types.h"

namespace P3D
{

class Runnable1 : public IRunnable
{
public:
	unsigned long Run()
	{
		for (int i=0; i<5; i++)
		{
			//DebugPrint(_W("THREAD TEST: Runnable1 is working...\n")); // ===> You can set here a breakpoint and see running threads in debugger
			Sleep(10);
		}

		return 0;
	}
};

class AutomaticalyDestroyableRunnable : public IRunnable
{
public:
	unsigned long Run()
	{
		for (int i=0; i<5; i++)
		{
			//DebugPrint(_W("THREAD TEST: AutomaticalyDestroyableRunnable is working...\n")); // ===> You can set here a breakpoint and see running threads in debugger
			Sleep(10);
		}
		//DebugPrint(_W("THREAD TEST: AutomaticalyDestroyableRunnable will be destroyed...\n"));
		delete this;
		return 0;
	}
};

// define test runnable and thread
Runnable1 run1;
Thread th1(&run1, _W("My Test Thread:)"), ThreadBase::PriorityLow );

void TestThreads()
{
	// start global thread
	th1.Start();

	// start thread which will delete runnable and thread when finished working...
	Thread *thDyn = new ThreadDyn(new AutomaticalyDestroyableRunnable(), _W("AutomaticalyDestroyableRunnable"));
	thDyn->Start();
}

};
