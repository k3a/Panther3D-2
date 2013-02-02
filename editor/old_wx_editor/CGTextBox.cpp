/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "CGUI.h"

#ifndef WX_PRECOMP
#include "wx/textctrl.h"
#endif

namespace P3D {

/// TextBox wrapper for EdGUI implementing interface (see. EdGUI.h for more info)
/** This wrapper classes are used to allow EdGUI client (plugin) to manipulate with GUI system - wxWidgets without 
dependency to original GUI API headers and libraries */
class CGTextBoxW : public IGTextBox, public wxEvtHandler {
public:

	CGTextBoxW(IGWindow* pwin, wxTextCtrl* pobj){obj=pobj; win=pwin; id=obj->GetId(); pwin->AddTextBox(this); };
	~CGTextBoxW(){};
	void SetText(const wchar* szText){
		obj->SetValue(wxString(szText));
	};
	const wchar* GetText(){
		static wxString tmpstr = obj->GetValue();
		return tmpstr;
	};

	// EVENTS:
	void EventCommand(wxCommandEvent& event)
	{
		if (!win) return;

		wxEventType et = event.GetEventType();
		if (et==wxEVT_COMMAND_TEXT_UPDATED) win->evTextBox(GE_TEXT_CHANGE, id, this);
		else if (et==wxEVT_COMMAND_ENTER) win->evTextBox(GE_TEXT_ENTER, id, this);

		event.Skip();
	}
	
	GCOMPONENT_COMMON_BODY(TextBox);

private:
	wxTextCtrl* obj; 
};


IMPLEMENT_DYNAMIC_CLASS(CGTextBox, wxXmlResourceHandlerEx)

CGTextBox::CGTextBox() : wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxTE_NO_VSCROLL);
    XRC_ADD_STYLE(wxTE_AUTO_SCROLL);
    XRC_ADD_STYLE(wxTE_PROCESS_ENTER);
    XRC_ADD_STYLE(wxTE_PROCESS_TAB);
    XRC_ADD_STYLE(wxTE_MULTILINE);
    XRC_ADD_STYLE(wxTE_PASSWORD);
    XRC_ADD_STYLE(wxTE_READONLY);
    XRC_ADD_STYLE(wxHSCROLL);
    XRC_ADD_STYLE(wxTE_RICH);
    XRC_ADD_STYLE(wxTE_RICH2);
    XRC_ADD_STYLE(wxTE_AUTO_URL);
    XRC_ADD_STYLE(wxTE_NOHIDESEL);
    XRC_ADD_STYLE(wxTE_LEFT);
    XRC_ADD_STYLE(wxTE_CENTRE);
    XRC_ADD_STYLE(wxTE_RIGHT);
    XRC_ADD_STYLE(wxTE_DONTWRAP);
#if WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxTE_LINEWRAP);
#endif // WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxTE_CHARWRAP);
    XRC_ADD_STYLE(wxTE_WORDWRAP);
    AddWindowStyles();
}

wxObject *CGTextBox::DoCreateResource()
{
    XRC_MAKE_INSTANCE(text, wxTextCtrl)

    text->Create(m_parentAsWindow,
                 GetID(),
                 GetText(wxT("value")),
                 GetPosition(), GetSize(),
                 GetStyle(),
                 wxDefaultValidator,
                 GetName());

    SetupWindow(text);

    if (HasParam(wxT("maxlength")))
        text->SetMaxLength(GetLong(wxT("maxlength")));

	GCOMPONENT_COMMON_INIT(TextBox, text);

	// connect event handlers
	text->Connect( text->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CGTextBoxW::EventCommand) , NULL, (wxEvtHandler *)wrp );
	text->Connect( text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(CGTextBoxW::EventCommand) , NULL, (wxEvtHandler *)wrp );

    return text;
}

bool CGTextBox::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxTextCtrl"));
}

}; // namespace
