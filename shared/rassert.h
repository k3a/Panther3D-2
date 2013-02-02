#pragma once

#ifdef _WIN64
#define BreakFn DebugBreak()
#else
#define BreakFn __asm{int 3}
#endif

#include <stdarg.h>

#if defined(WIN32)
#ifdef P3DSTATIC_LIBRARY
#define LIBAPI //__declspec(dllexport)
#else
#define LIBAPI //__declspec(dllimport)
#endif
#elif defined(LINUX)
#define LIBAPI 
#endif

#ifdef _DEBUG

LIBAPI bool ImplementBreak(bool test, const wchar *expression, const wchar *funcn, const wchar *filename, int linenumber, const wchar *description);
LIBAPI bool ImplementAssert(bool test, const wchar *expression, const wchar *funcn, const wchar *filename, int linenumber, const wchar *description);
LIBAPI void ImplementDebugPrint(const wchar *pattern, va_list args);

#define Assert(x) if (!(x) && ImplementAssert(x,_W(#x),_W(__FUNCTION__),_W(__FILE__),__LINE__,0)) BreakFn
#define AssertDesc(x,desc) if (!(x) && ImplementAssert(x,_W(#x),_W(__FUNCTION__),_W(__FILE__),__LINE__,_W(desc))) BreakFn
#define Verify(x)  if (!(x) && ImplementAssert(x,_W(#x),_W(__FUNCTION__),_W(__FILE__),__LINE__,0)) BreakFn
#define VerifyDesc(x,desc)  if (!(x) && ImplementAssert(x,_W(#x),_W(__FUNCTION__),_W(__FILE__),__LINE__,_W(desc))) BreakFn
//#define Error(desc)  if (!(x) && ImplementAssert(false,_W("General Fail"),_W(__FUNCTION__),_W(__FILE__),__LINE__,0)) BreakFn
static inline void DebugPrint(const wchar *pattern, ...) 
{
  va_list va;
  va_start(va,pattern);
  ImplementDebugPrint(pattern, va);
}
#define DebugPrint0(str) DebugPrint(str);
#define DebugPrint1(str,a1) DebugPrint(str,a1);
#define DebugPrint2(str,a1,a2) DebugPrint(str,a1,a2);
#define DebugPrint3(str,a1,a2,a3) DebugPrint(str,a1,a2,a3);
#define DebugPrint4(str,a1,a2,a3,a4) DebugPrint(str,a1,a2,a3,a4);
#define DebugPrint5(str,a1,a2,a3,a4,a5) DebugPrint(str,a1,a2,a3,a4,a5);
#define DebugPrint6(str,a1,a2,a3,a4,a5,a6) DebugPrint(str,a1,a2,a3,a4,a5,a6);
#define DebugPrint7(str,a1,a2,a3,a4,a5,a6,a7) DebugPrint(str,a1,a2,a3,a4,a5,a6,a7);
#define DebugPrint8(str,a1,a2,a3,a4,a5,a6,a7,a8) DebugPrint(str,a1,a2,a3,a4,a5,a6,a7,a8);
#define CompileTimeAssert(x,id) const int __ctassert_##id=1/(int)(x)

#else

#define Assert(x)
#define AssertDesc(x,desc)
#define Verify(x) x
#define VerifyDesc(x,desc) x
static inline void DebugPrint(const char *pattern, ...) {}
#define DebugPrint0(str)
#define DebugPrint1(str,a1)
#define DebugPrint2(str,a1,a2)
#define DebugPrint3(str,a1,a2,a3)
#define DebugPrint4(str,a1,a2,a3,a4)
#define DebugPrint5(str,a1,a2,a3,a4,a5)
#define DebugPrint6(str,a1,a2,a3,a4,a5,a6)
#define DebugPrint7(str,a1,a2,a3,a4,a5,a6,a7)
#define DebugPrint8(str,a1,a2,a3,a4,a5,a6,a7,a8)
#define PrintResult(x,format)
#define Error(desc)
#define CompileTimeAssert(x,id)
#endif

#ifdef assert
#undef assert
#endif

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef _DEBUG

	/// standard assertion
	#define assert(x)  if (!(x) && ImplementAssert(x,_W(#x),_W(__FUNCTION__),_W(__FILE__),__LINE__,0)) BreakFn
	/// assertion with description of failed state 
	#define assertd(x,desc)  if (!(x) && ImplementAssert(x,_W(#x),_W(__FUNCTION__),_W(__FILE__),__LINE__,_W(desc))) BreakFn

	/// standard assertion
	#define ASSERT(x)  if (!(x) && ImplementAssert(x,_W(#x),_W(__FUNCTION__),_W(__FILE__),__LINE__,0)) BreakFn
	/// assertion with description of failed state 
	#define ASSERTD(x,desc)  if (!(x) && ImplementAssert(x,_W(#x),_W(__FUNCTION__),_W(__FILE__),__LINE__,_W(desc)) BreakFn

#else

	/// standard assertion
	#define assert(x)
	/// assertion with description of failed state 
	#define assertd(x,desc)

	/// standard assertion
	#define ASSERT(x)
	/// assertion with description of failed state 
	#define ASSERTD(x,desc)

#endif