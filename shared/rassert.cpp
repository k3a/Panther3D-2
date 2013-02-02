#include "precompiled.h"

#include "rassert.h"
#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif
#include <stdio.h>

#ifdef _DEBUG
#define ASSERT_MEMORY_SIZE 256
#else
#define ASSERT_MEMORY_SIZE 16
#endif



struct AssertMemoryInfo
{
  const wchar *file;
  int line;

  AssertMemoryInfo(const wchar *file=0, int line=0):file(file),line(line) {}  
};

static AssertMemoryInfo GAssertMemory[ASSERT_MEMORY_SIZE];
static int GAssertMemorySz=0;

static void SaveToMemory(const wchar *file, int line)
{
  if (GAssertMemorySz>=ASSERT_MEMORY_SIZE) return;
  GAssertMemory[GAssertMemorySz++]=AssertMemoryInfo(file,line);
}

static bool SearchInMemory(const wchar *file, int line)
{
  for (int i=0;i<GAssertMemorySz;i++)
	if (GAssertMemory[i].file==file && GAssertMemory[i].line==line) return true;
  return false;
}

#ifdef _WIN32
VOID CALLBACK MessageBoxHook(HWND hwnd,UINT uMsg,UINT_PTR idEvent, DWORD dwTime)
{
  HWND hWnd=GetActiveWindow();
  if (hWnd==0 || GetDlgItem(hWnd,IDABORT)==0) return;
  SetDlgItemText(hWnd,IDABORT,_W("&Stop"));
  SetDlgItemText(hWnd,IDRETRY,_W("&Debug"));
  SetDlgItemText(hWnd,IDIGNORE,_W("&Ignore"));
  SetDlgItemText(hWnd,IDHELP,_W("Ignore &All"));
  EnableWindow(GetDlgItem(hWnd,IDHELP),GAssertMemorySz+1<ASSERT_MEMORY_SIZE);
  SetWindowLong(GetDlgItem(hWnd,IDHELP),GWL_ID,IDYES);
  KillTimer(hwnd,idEvent);
  DestroyWindow(hwnd);
}
static DWORD WINAPI SafeMessageBox(LPVOID data)
{
  const wchar *text=(const wchar *)data;
/*  HWND hWnd=CreateWindow(_W("STATIC"),_W("MessageBoxHook"),0,0,0,0,0,0,0,GetModuleHandle(0),0);*/
  SetTimer(0,0,1,MessageBoxHook);
  UINT res=MessageBox(0,text, _W("Assertion Failed"),MB_ABORTRETRYIGNORE|MB_ICONSTOP|MB_TASKMODAL|MB_HELP|MB_DEFBUTTON2);  
  return res;
}
#endif


LIBAPI bool ImplementBreak(bool test, const wchar *expression, const wchar *funcn, const wchar *file, int line, const wchar *desc)
{
if (test) return false;
#ifndef _WIN32
	printf("%s\r\n\r\nFunction: %s\r\nFile: %s\r\nLine: %d\r\nExpression: %s\n", desc,funcn,file,line,expression);
#else
  wchar buff[1024];
  if (desc==0) desc=_W("Assertion failed in following expression:");
  wsprintf(buff,1023,_W("%s(%d): %s: %s\r\n\t%s\r\n"),file,line,funcn,desc,expression);
  OutputDebugString(buff);

  if (SearchInMemory(file, line)==false)
  {
	DWORD threadId;
	wsprintf(buff,1023,_W("%s\r\n\r\nFunction: %s\r\nFile: %s\r\nLine: %d\r\nExpression: %s"),desc,funcn,file,line,expression);	
	HANDLE h=CreateThread(0,0,SafeMessageBox,(LPVOID)buff,0,&threadId);
	WaitForSingleObject(h,INFINITE);
	DWORD res;
	GetExitCodeThread(h,&res);
	CloseHandle(h);
	if (res==IDABORT) ExitProcess(-1);
	if (res==IDRETRY) {DebugBreak();}
	if (res==IDYES) SaveToMemory(file,line);
  }
#endif
  return false;
}

LIBAPI bool ImplementAssert(bool test, const wchar *expression, const wchar *funcn, const wchar *file, int line, const wchar *desc)
{
	if (test) return false;
#ifndef _WIN32
	printf("%s\r\n\r\nFunction: %s\r\nFile: %s\r\nLine: %d\r\nExpression: %s\n",desc,funcn,file,line,expression);	
#else
	wchar buff[1024];
	if (desc==0) desc=_W("Assertion failed in following expression:");
	wsprintf(buff,1023,_W("%s(%d): %s: %s\r\n\t%s\r\n"),file,line,funcn,desc,expression);
	OutputDebugString(buff);
	if (SearchInMemory(file, line)==false)
	{
		DWORD threadId;
		wsprintf(buff,1023,_W("%s\r\n\r\nFunction: %s\r\nFile: %s\r\nLine: %d\r\nExpression: %s"),desc,funcn,file,line,expression);	
		HANDLE h=CreateThread(0,0,SafeMessageBox,(LPVOID)buff,0,&threadId);
		WaitForSingleObject(h,INFINITE);
		DWORD res;
		GetExitCodeThread(h,&res);
		CloseHandle(h);
		if (res==IDABORT) ExitProcess(-1);
		if (res==IDRETRY) {return true;}
		if (res==IDYES) SaveToMemory(file,line);
	}
#endif
	return false;
}

LIBAPI void ImplementDebugPrint(const wchar *pattern, va_list args)
{
  wchar buffer[1024];
  wvsnprintf(buffer,1024,pattern,args);
#ifdef _WIN32
  OutputDebugString(buffer);
#else
  printf("%s\n", buffer);
#endif
}