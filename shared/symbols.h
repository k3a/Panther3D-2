/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
MÌsto pro umisùov·nÌ sdÌlen˝ch maker, definic, pripadne funkci
*/
#pragma once

// memory mgmt for ASCII<->Unicode conversions
#include <malloc.h>

// Memory leak detection
#if defined(_DEBUG) && defined(_MSC_VER) && _MSC_VER >= 800
	#include <stdlib.h>
	#include <crtdbg.h>
#else
	// memory leak detection disabled
#endif

#ifndef WIN32
#define MAX_PATH 256 // for Linux
#define HMODULE void* // dlopen on Linux returns void* as handle

// message box defines
#define MB_SYSTEMMODAL 2
#define MB_ICONSTOP 4
#define MB_ICONEXCLAMATION 8
#define MB_ICONINFORMATION 16

// Win32 Sleep(miliseconds) x LINUX sleep(seconds)
#include <unistd.h>
inline void Sleep(unsigned long ms)
{
	sleep(div(ms,1000));
	usleep((ms%1000)*1000);
}
#endif

/*
* Fucking DirectX definition of color. Dunno why it must be defined here, but never mind :-P
*/
#ifndef D3DCOLOR_DEFINED
typedef unsigned long D3DCOLOR;
#define D3DCOLOR_DEFINED
#endif


// TODO: move to namespace later
// UNICODE SUPPORT SYMBOLS
#ifdef _UNICODE
		typedef wchar_t  wchar;
		#define __W(x) L##x
		#define _W(x) __W(x)

		// kexik helper to make some errors
		/*#undef _T
		#define _T 00
		#define _INC_TCHAR*/

		// undef other's
		#ifdef wsprintf
		 #undef wsprintf
		#endif
		#ifdef wvsprintf
		 #undef wvsprintf
		#endif

		#define witoa _itow
		#define watoi _wtoi
		#define watof _wtof
		#define watol _wtol
		#define wstrlen wcslen
		#define wstrcpy wcscpy
		#define wstrncpy wcsncpy
		#define wsprintf swprintf
		#define wvsnprintf _vsnwprintf
		#define wvsprintf _vswprint
		#define wstrcat wcscat
		#define wstricmp wcsicmp
		#define wstrcmpi wcsicmp
		#define wstrnicmp wcsnicmp
		#define wstrncmp wcsncmp
		#define wsscanf swscanf
		#define wstrcmp wcscmp
		#define wstrpbrk wcspbrk 
		#define wvprintf vwprintf
		#define wstrtok wcstok
		// stdio
		#define wfopen _wfopen 
		#define wfputs fputws
		#define wstat _wstat
		#define wfprintf fwprintf
#else
		typedef char  wchar;
		//typedef std::string  std::wstring;
		#define _W(x) x

		// undef other's
		#ifdef wsprintf
		 #undef wsprintf
		#endif

		#define witoa itoa 
		#define watoi atoi
		#define watof atof
		#define watol atol
		#define wstrlen strlen 
		#define wstrcpy strcpy 
		#define wstrncpy strncpy
		#define wsprintf sprintf 
		#define wvsnprintf vsnprintf
		#define wvsprintf vsprintf
		#define wstrcat strcat
		#define wstricmp stricmp 
		#define wstrcmpi strcmpi 
		#define wstrnicmp strnicmp
		#define wstrncmp strncmp
		#define wsscanf sscanf
		#define wstrcmp strcmp 
		#define wstrpbrk strpbrk 
		#define wvprintf vprintf 
		#define wstrtok strtok
		// stdio
		#define wfopen fopen 
		#define wfputs fputs
		#define wstat stat
		#define wfprintf fprintf
#endif
// Unicode Support Functions
#ifdef _UNICODE
	// ansi wchar to text wchar
	inline wchar A2W ( char In ) { return (wchar)In; }
	// text wchar to ansi wchar
	inline char W2A ( wchar In ) { return In<(wchar)0x100 ? In : '?'; }
	// ansi to text
	inline wchar* A2W ( wchar* Dest, const char* Source ) 
	{ 
		int i = 0;

		while(Source[i] != '\0')
		{
			Dest[i] = A2W(Source[i]);
			++i;
		}
		Dest[i]=0;
		return Dest;
	}
	// ansi to text
	inline wchar* A2W ( wchar* Dest, int CharCnt, const char* Source ) 
	{ 
		int i = 0;

		while(Source[i] != '\0' && i<CharCnt)
		{
			Dest[i] = A2W(Source[i]);
			++i;
		}
		Dest[i]=0;
		return Dest;
	}
	// text to ansi
	inline char* W2A ( char* Dest, const wchar* Source ) { 
		int i = 0;

		while(Source[i] != '\0')
		{
			Dest[i] = W2A(Source[i]);
			++i;
		}
		Dest[i]=0;
		return Dest;
	}

	// text to ansi
	inline char* W2A ( char* Dest, int CharCnt, const wchar* Source ) { 
		int i = 0;

		while(Source[i] != _W('\0') && i<CharCnt)
		{
			Dest[i] = W2A(Source[i]);
			++i;
		}
		Dest[i]=0;
		return Dest;
	}
	#define _W2A(wide) W2A( (char*)alloca(wstrlen((const wchar *)wide)+1), (const wchar *)wide)
	#define _A2W(ascii) A2W( (wchar*)alloca( sizeof(wchar) * (strlen((const char *)ascii)+1) ), (const char *)ascii)
#else
	// ansi wchar to text wchar
	inline wchar A2W ( char In ) { return In; }
	// text wchar to ansi wchar
	inline char W2A ( wchar In ) { return In; }
	// ansi to text
	inline wchar* A2W ( wchar* Dest, const char* Source ) 
	{ 
		int i = 0;

		while(Source[i] != '\0')
		{
			Dest[i] = A2W(Source[i]);
			++i;
		}
		Dest[i]=0;
		return Dest;
	}
	// ansi to text
	inline wchar* A2W ( wchar* Dest, int CharCnt, const char* Source ) 
	{ 
		int i = 0;

		while(Source[i] != '\0' && i<CharCnt)
		{
			Dest[i] = A2W(Source[i]);
			++i;
		}
		Dest[i]=0;
		return Dest;
	}
	// text to ansi
	inline char* W2A ( char* Dest, const wchar* Source ) { 
		int i = 0;

		while(Source[i] != '\0')
		{
			Dest[i] = W2A(Source[i]);
			++i;
		}
		Dest[i]=0;
		return Dest;
	}

	// text to ansi
	inline char* W2A ( char* Dest, int CharCnt, const wchar* Source ) { 
		int i = 0;

		while(Source[i] != '\0' && i<CharCnt)
		{
			Dest[i] = W2A(Source[i]);
			++i;
		}
		Dest[i]=0;
		return Dest;
	}
	#define _W2A(wide) wide
	#define _A2W(ascii) ascii
#endif









namespace P3D
{
	#define P3DNAME "Panther3D[2] Engine"

	// SPECIAL DEFINITIONS / CONFIGURATIONS
#ifdef _DEBUG
	/// Specifies that this build is considered "Internal"
	/** Internal build are made for our team and other game developers. It should be almost as fast as Release but can contain 
	more debug tools and messages which are useful at development-time, but not in release version. 
	Internal executables often consumes more memory. Debug version (_DEBUG) is automatically also Internal,
	but these are far more slower then only Internal or Release. */
	#define _INTERNAL 1 // debug version is also internal
#else
 #ifndef _INTERNAL
	/// Specifies that this build is considered "Release"
	#define _RELEASE 1
 #endif
#endif

	// SAFE DELETE MACROS
	#define SAFE_NULL(p)			{p = NULL;}
	#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
	#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
	#define SAFE_SHUTDOWN(p)		{ if(p) { (p)->Shutdown();(p)=NULL; } }
	#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }

	#define ONE_METER 				100.0f  // one meter
	#define ONE_METER_DIV			0.01f	// (1 / one meter)
	#define UNIVERSAL_GRAVITY		9.8f

	#define _PI		((float)3.141592654f)

	#define		DEFAULT_FIELD_OF_VIEW	((_PI/180.0f) * 65.0f)
	#define		DEFAULT_NEAR_PLANE		10.0f
	#define		DEFAULT_FAR_PLANE			40000.0f

	#define P3DMAX_PATH 256
	#define P3DMAX_FILENAME 64

	#ifndef OUT
	#define OUT
	#define IN
	#endif

	//#### COLORS
	#define COLOR_ARGB(a,r,g,b) \
		((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
	#define COLOR_XRGB(r,g,b) COLOR_ARGB(0xff,r,g,b)
	// basic colors
	#define COLOR_RED COLOR_XRGB(255, 0, 0)
	#define COLOR_GREEN COLOR_XRGB(0, 255, 0)
	#define COLOR_BLUE COLOR_XRGB(0, 0, 255)
	#define COLOR_BLACK 0xFF000000 // cerna
	#define COLOR_WHITE 0xFFFFFFFF // bila
	#define COLOR_GRAY 0xFF808080 // seda
	#define COLOR_ORANGE 0xFFFFA500 // oranzova
	#define COLOR_CYAN 0xFF0000FF // modrozelena
	#define COLOR_BROWN 0xFFA52A2A // hneda
	#define COLOR_YELLOW 0xFFFFFF00 // zluta
	#define COLOR_SILVER 0xFFC0C0C0 // stribrna
	#define COLOR_GOLD 0xFFFFD700 // zlata
	#define COLOR_LIME 0xFF00FF00 // razici zelena
	#define COLOR_CYAN 0xFF0000FF // modrozelena
	#define COLOR_MAGENTA 0xFFFF00FF //fialova
	#define COLOR_WOMATO 0xFFFF6347 // rajcatova oranzova
	#define COLOR_ORANGE 0xFFFFA500 // oranzova
	#define COLOR_AQUAMARINE 0xFF7FFFD4 // morska modra
	#define COLOR_BISQUE 0xFFFFE4C4 //susenkova hneda
	#define COLOR_CHOCOLATE 0xFFD2691E //cokoladova hneda
	#define COLOR_DARKBLUE 0xFF00008B //tmave modra
	#define COLOR_DARKGRAY 0xFFA9A9A9 //tmave seda
	#define COLOR_DARKORCHID 0xFF9932CC //tmava orchidej
	#define COLOR_SKYBLUE 0xFF00BFFF //nebeska modra
	#define COLOR_PURPLE 0xFF800080 //purpurova
	#define COLOR_SLATEBLUE 0xFF6A5ACD //jemna modra
	#define COLOR_STEELBLUE 0xFF4682B4 //kovova modra
	#define COLOR_GRASSGREEN 0xFF2E8B57 //travnata zelena
	#define COLOR_OLIVEGREEN 0xFF6B8E23 // olivove zelena

	#define ToRadian( degree ) ((degree) * (_PI / 180.0f))
	#define ToDegree( radian ) ((radian) * (180.0f / _PI))

	//#### copy from standard min ana max.... necessary
	#define pmax(a,b)            (((a) > (b)) ? (a) : (b))
	#define pmin(a,b)            (((a) < (b)) ? (a) : (b))

	int wildcmp(const wchar *pszWildcard, const wchar *pszString);

	// mathematical functions
	#include <stdlib.h>
	inline int randi(int nMin, int nMax){ return rand()%(nMax-nMin)+nMin; };
	inline int randi(int nMax){ return rand()%nMax; };
	inline float randf(float fMin, float fMax){ return (1.0f/RAND_MAX*rand())*(fMax-fMin)+fMin; };
	inline float randf(float fMax){ return (1.0f/RAND_MAX*rand())*fMax; };

#ifdef LINUX
	#include <unistd.h>
    void MessageBox(int, const wchar* text, const wchar* title, int);
#endif

	// hash random arrays
	extern unsigned int g_randHash256[];
	extern unsigned int g_randHash512[];
	extern unsigned int g_randHash2048[];
}; // namespace

#include "assert.h" // must be included last