/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "CGUI.h"

namespace P3D
{

IMPLEMENT_DYNAMIC_CLASS(CGButton, wxXmlResourceHandlerEx)

/// Button wrapper for EdGUI implementing interface (see. EdGUI.h for more info)
/** This wrapper classes are used to allow EdGUI client (plugin) to manipulate with GUI system - wxWidgets without 
dependency to original GUI API headers and libraries */
class CGButtonW : public IGButton, public wxEvtHandler {
public:

	CGButtonW(IGWindow* pwin, wxButton* pobj){obj=pobj; win=pwin; id=obj->GetId(); pwin->AddButton(this); };
	~CGButtonW(){};
	void SetCaption(const wchar* szCaption){
		obj->SetLabel(szCaption);
	};
	const wchar* GetCaption(){
		return obj->GetLabel();
	};
	void SetName(const wchar* szName)
	{
		obj->SetName(szName);
	};
	const wchar* GetName()
	{
		return obj->GetName();
	}

	// EVENTS:
	void EventClick(wxCommandEvent& event)
	{
		if (!win) return;

		eGEvent ev = MapEvent(event.GetEventType());
		if (ev!=GE_UNKNOWN) win->evButton(ev, id, this);

		event.Skip();
	}

/*	void CGButtonW::EventMouse(wxMouseEvent& ev)
	{
		//if (ev.Entering()) obj->ToggleWindowStyle(wxNO_BORDER);
		//else if (ev.Leaving()) obj->ToggleWindowStyle(wxNO_BORDER);
	}*/
private:
	wxButton* obj; 
};

// --------------------------


CGButton::CGButton()
: wxXmlResourceHandlerEx()
{
	XRC_ADD_STYLE(wxBU_LEFT);
	XRC_ADD_STYLE(wxBU_RIGHT);
	XRC_ADD_STYLE(wxBU_TOP);
	XRC_ADD_STYLE(wxBU_BOTTOM);
	XRC_ADD_STYLE(wxBU_EXACTFIT);
	AddWindowStyles();
}

wxObject *CGButton::DoCreateResource()
{
	XRC_MAKE_INSTANCE(button, wxButton)

		button->Create(m_parentAsWindow,
		GetID(),
		GetText(wxT("label")),
		GetPosition(), GetSize(),
		GetStyle(),
		wxDefaultValidator,
		GetName());

	if (GetBool(wxT("default"), 0))
		button->SetDefault();
	SetupWindow(button);

	CGButtonW *wrp = new CGButtonW(GetImplClass(), button);
	button->SetClientData(wrp);

	// connect event handlers
	button->Connect( button->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CGButtonW::EventClick) , NULL, (wxEvtHandler *)wrp );
	//button->Connect( button->GetId(), wxEVT_ENTER_WINDOW, wxMouseEventHandler(CGButtonW::EventMouse) , NULL, (wxEvtHandler *)wrp );
	//button->Connect( button->GetId(), wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CGButtonW::EventMouse) , NULL, (wxEvtHandler *)wrp );

	return button;
}

bool CGButton::CanHandle(wxXmlNode *node)
{
	return IsOfClass(node, wxT("wxButton"));
}


}; // namespace