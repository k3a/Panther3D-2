/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "IConsole.h"
#include "IGUI.h"
#include "ITextRenderer.h"

#include <vector>

namespace P3D
{

class CConsole : public IConsole
{
public:
	CConsole();
	~CConsole();

	void AddMsg(eMsgType type, const wchar* text, ...);
	void AddMsgEx(eMsgType type, const wchar* scopeName, const wchar* text, ...);

	unsigned int GetLineCount()const{ 
		return m_lines.size(); 
	};

	const wchar* GetLine(unsigned int lineNum) { 
		if (lineNum<m_lines.size()) 
			return m_lines[lineNum]; 
		else 
			return _W("");
	};

	const wchar* GetString()const{ return m_string.c_str();  };

	/// Show/Hide console. New state will be returned
	bool ToggleVisible(){ 
		/*if (m_wndConsole->IsVisible()) 
		{
			m_wndConsole->SetVisible(false);
			return false;
		} else {
			m_wndConsole->SetVisible(true); 
			return true;
		}*/
		return false;
	};
	void SetVisible(bool visible) { /*m_wndConsole->SetVisible(visible);*/ };

	void InitializeGUI();
	void Draw();
private:
	void Print(eMsgType type, const wchar* text, bool bExtended=false);
	/*IGUITextBox *m_textBox;
	IGUIWindow *m_wndConsole;*/
	ITextRenderer *M_tr;
	//
	FontHandle m_font;
	//
	vector<wchar*> m_lines;
	wstring m_string;
	FILE* m_fp;
};

}
