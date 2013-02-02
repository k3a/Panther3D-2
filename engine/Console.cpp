/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "Console.h"
#include "Engine.h"

#include <stdio.h>
#include <time.h>

namespace P3D
{

extern wchar g_consoleHeadTemplate[];
extern wchar g_consoleTailTemplate[];

size_t WriteUTF8(const wchar* string, FILE *fp)
{
	if (!string || !*string) return 0;

	// if not unicode
#ifndef UNICODE
	return fwrite(string, 1, strlen(string), fp);
#else

	unsigned int inputLen = wstrlen(string);
	unsigned char *target;
	target = new unsigned char[inputLen*4+1]; // make buffer long enough
	UErrorCode status = U_ZERO_ERROR;
	int32_t     len;

	//// set up the converter
	UConverter *conv = ucnv_open("utf-8", &status);
	assertd(U_SUCCESS(status)!=0, "Failed to open utf-8 converter!");

	//// convert 
	len = ucnv_fromUChars(conv, (char*)target, inputLen*4, string, -1, &status);
	assertd(U_SUCCESS(status)!=0, "Failed to convert string to utf-8!");

	//// write to file
	size_t wrlen = fwrite(target, 1, len, fp);

	//// close and clean
	delete[] target;
	ucnv_close(conv);

	return wrlen;
#endif
}

//------------------------------------------
CConsole::CConsole()
{
	wchar szPhPath[MAX_PATH];

	// construct physical path
	wstrcpy(szPhPath, I_GetBinaryPath());
	wstrcat(szPhPath, _W("../console.htm"));

	m_fp = wfopen(szPhPath, _W("wb"));
	if (!m_fp)
	{
		MessageBox(0, _W("Cannot open console.htm for writing!"), _W("Cannot open log"), 0);
		exit(-1);
	}
	WriteUTF8(g_consoleHeadTemplate, m_fp);

	wchar systemInfo[1024];
	wsprintf(systemInfo, 1023, _W(" \
								  <div class=\"header\"> \
								   <div class=\"title\">%s</div> \
								   <div> \
								    Engine build: %d<br> \
									Game: n/a<br> \
								   </div> \
								  </div>"),
								  _A2W(P3DNAME), CEngine::instance()->GetBuildNum() );
	WriteUTF8(systemInfo, m_fp);
}

//------------------------------------------
CConsole::~CConsole()
{
	for (unsigned int i=0; i< m_lines.size(); i++) delete[]  m_lines[i];

	WriteUTF8(g_consoleTailTemplate, m_fp);
	fclose(m_fp);
}

//-------------------------------------------
void CConsole::AddMsg(eMsgType type, const wchar* text, ...){
	wchar buffer[2048];

	try
	{
		va_list argList;
		va_start(argList, text);
		wvsnprintf(buffer,2047, text,argList);
		va_end(argList);
	}
	catch(...)
	{
		assert(!"Can't create console message, maybe too long (>2047)! Please split it to more messages...");
		return;
	}

	// remove \n\r from end
	unsigned int slen = wstrlen(buffer);
	for (unsigned int i=slen-1; i>=0; i--)
	{
		if (buffer[i]==_W('\r') || buffer[i]==_W('\n'))
			buffer[i] = 0;
		else
			break;
	}

	// parse lines
	wchar* ptrLine = buffer;
	for(unsigned int i=0; i<=slen; i++)
	{
		if ( buffer[i]==_W('\r') )
		{
			buffer[i]=_W(' ');
			continue; // nothing
		}
		else if ( buffer[i]==_W('\n') )
		{
			buffer[i]=0;
			if (ptrLine!=buffer) // this is additional line 
			{
				*(ptrLine-1) = _W(' ');
				Print(type, ptrLine-1, false);
			}
			else
				Print(type, ptrLine, false);
			ptrLine = &buffer[i+1];
		}
		else if (buffer[i]==0)
		{
			if (ptrLine!=buffer)
			{
				*(ptrLine-1) = ' '; // additional line 
				Print(type, ptrLine-1, false);
			}
			else
				Print(type, ptrLine, false);
			break;
		}
	}
}

//----------------------------------------------
void CConsole::AddMsgEx(eMsgType type, const wchar* scopeName, const wchar* text, ...){
	wchar buffer[2048+128]; wchar* buffPtr=buffer; // 128 for scopeName :-P

	// copy scope name
	/*wstrncpy(buffPtr, scopeName, 127);
	buffPtr+=wstrlen(buffPtr);
	*buffPtr=':'; buffPtr++;
	*buffPtr=' '; buffPtr++;*/

	try
	{
		va_list argList;
		va_start(argList, text);
		wvsnprintf(buffPtr,2250, text,argList);
		va_end(argList);
	}
	catch(...)
	{
		assert(!"Can't create console message, maybe too long (>2048)! Please split it to more messages...");
		return;
	}

	// remove \n\r from end
	unsigned int slen = wstrlen(buffer);
	for (unsigned int i=slen-1; i>=0; i--)
	{
		if (buffer[i]==_W('\r') || buffer[i]==_W('\n'))
			buffer[i] = 0;
		else
			break;
	}

	// parse lines
	wchar* ptrLine = buffer;
	for(unsigned int i=0; i<=slen; i++)
	{
		if ( buffer[i]==_W('\r') )
		{
			buffer[i]=_W(' ');
			continue; // nothing
		}
		else if ( buffer[i]==_W('\n') )
		{
			buffer[i]=0;
			if (ptrLine!=buffer) // this is additional line 
			{
				*(ptrLine-1) = _W(' ');
				Print(type, ptrLine-1, true);
			}
			else
				Print(type, ptrLine, true);
			ptrLine = &buffer[i+1];
		}
		else if (buffer[i]==0)
		{
			if (ptrLine!=buffer) 
			{
				*(ptrLine-1) = ' '; // additional line 
				Print(type, ptrLine-1, true);
			}
			else
				Print(type, ptrLine, true);
			break;
		}
	}
}

//----------------------------------------------
void CConsole::Print(eMsgType type, const wchar* text, bool bExtended)
{
	wchar buffer[1024+128+8];
	wsprintf(buffer, 1024+128+7, _W("> %s\r\n"), text);

#if defined(WIN32)
	OutputDebugString(buffer);
#else
	printf(buffer);
#endif

	// get system time from standard library
	time_t rawtime;
	struct tm * ti;

	time ( &rawtime );
	ti = localtime ( &rawtime );

	// store to memory
	wchar* lnstr=new wchar[wstrlen(text)+3];
	wstrcpy(lnstr, text);
	m_lines.push_back(lnstr);

	wstrcat(lnstr, _W("\r\n"));
	m_string += lnstr;

	// print to file
	// POKUD BY TOHLE DELALO PROBLEM TAK HLASIT! Udelam thread lock. On je nejspis treba, ale zatim se nestala chyba, tak to nemam jak testovat.
	wsprintf(buffer, 1024+128+7, _W("%.2d.%.2d.%.4d %.2d:%.2d:%.2d   %s<br>\r\n"), ti->tm_mday, ti->tm_mon+1, ti->tm_year+1900, ti->tm_hour, ti->tm_min, ti->tm_sec, text);
	WriteUTF8(buffer, m_fp);
	fflush(m_fp);

	// show message box
	switch(type)
	{
	case MSG_BOX:
		MessageBox(0, text, _W("INFORMATION!"), MB_ICONINFORMATION | MB_SYSTEMMODAL);
		break;
	case MSG_ERR_BOX:
		MessageBox(0, text, _W("ERROR!"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
		break;
	case MSG_ERR_FATAL:
		MessageBox(0, text, _W("FATAL ERROR!"), MB_ICONSTOP | MB_SYSTEMMODAL);
		_exit(-1); // KEX: Maybe call something like Engine->RequestShutdown? But it is fatall error, doesn't matter
		break;
	}
}

//------------------------------------------
static SVar svConsoleFont(_W("ConsoleFont"), _W("Engine.Debugging"), _W("arial.ttf"), _W("Console text font."));
static SVar svConsoleFontSize(_W("ConsoleFontSize"), _W("Engine.Debugging"), _W("10"), _W("Console text font size."));
void CConsole::InitializeGUI(){

//	IGUIWindow *win = CEngine::instance()->mGUI()->CreateWindow(_W("Console"), Vec2i(100, 100), Vec2i(500, 300));
//	win->LeftRollup()->AddLabel(_W("Test label"));
/*	m_wndConsole = CEngine::instance()->mGUI()->CreateWindow(_W("Console"), Vec2i(200, 150), Vec2i(500, 300));
	m_wndConsole->SetVisible(false);
	
	m_wndConsole->BeginSizer(2, true);

		m_textBox = m_wndConsole->AddTextBox(_W("Console text"), true);

		m_wndConsole->BeginSizer(2, false);

			m_wndConsole->AddTextBox(_W("Command line"));
			m_wndConsole->AddButton(_W("Submit"));

		m_wndConsole->EndSizer();

	m_wndConsole->EndSizer();*/

	M_tr = CEngine::instance()->mRenderer()->iTextRenderer();
	m_font = M_tr->CreateFont( svConsoleFont.GetString(), (unsigned int)svConsoleFontSize.GetInt() );
	if (!m_font) CON( MSG_ERR, _W("Failed to create console font %s!"), svConsoleFont.GetString() );
}

void CConsole::Draw()
{
	//m_textBox->SetText(GetString());
/*	IWindow * wnd = CEngine::instance()->GetCurrectWindowTarget();
	Rect conRect(10, wnd->GetSize().y-50-svConsoleFontSize.GetInt(), wnd->GetSize().x-20, wnd->GetSize().y-10 );

	wchar conLine[256] = _W("> ");
	M_tr->Print(conLine, Vec2i( conRect.x1, conRect.y1 ), ColorRGBA(1,1,1,1), m_font, NULL, &conRect );*/
}


// -----------------------------------------------------------------------------

wchar g_consoleHeadTemplate[] = 
_W(" \
<html> \
<head> \
<title>Panther3D Console Log</title> \
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"> \
<style type=\"text/css\"> \
body \
{ \
	font-family: 'courier new';  \
	font-weight: normal; \
	font-size: small; \
	color: black; \
	background-color: rgb(230, 230, 250); \
} \
.header \
{ \
	background-color: rgb(176, 196, 222); \
	padding: 10px; \
	border-color: rgb(100, 149, 237); \
	border-width: 1px; \
	border-style: solid; \
	font-family: arial; \
	font-weight: normal; \
	font-size: small; \
	color: darkblue; \
	margin-bottom: 10px; \
} \
.title \
{ \
	font-size: large; \
	color: black; \
} \
</style> \
</head> \
<body> \
   ");

wchar g_consoleTailTemplate[] = 
_W(" \
</body> \
</html> \
   ");

};
