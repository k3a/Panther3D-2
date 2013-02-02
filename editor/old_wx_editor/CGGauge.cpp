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
#include "wx/gauge.h"
#endif
namespace P3D {

/// Gauge wrapper for EdGUI implementing interface (see. EdGUI.h for more info)
/** This wrapper classes are used to allow EdGUI client (plugin) to manipulate with GUI system - wxWidgets without 
dependency to original GUI API headers and libraries */
class CGGaugeW : public IGGauge, public wxEvtHandler {
public:

	CGGaugeW(IGWindow* pwin, wxGauge* pobj){obj=pobj; win=pwin; id=obj->GetId(); pwin->AddGauge(this); };
	~CGGaugeW(){};
	void SetValue(int val){
		obj->SetValue(val);
	};
	int GetValue()const{
		return obj->GetValue();
	};
	void SetRange(int val){
		obj->SetRange(val);
	};
	int GetRange()const{
		return obj->GetRange();
	};
	void SetName(const wchar* szName)
	{
		obj->SetName(wxString(szName));
	};
	const wchar* GetName()
	{
		static wxString tmpstr = obj->GetName();
		return tmpstr;
	}

	// EVENTS:
	void EventKeyboard(wxKeyEvent& event)
	{
		if (!win) return;

		eGEvent ev = MapEvent(event.GetEventType());
		if (ev!=GE_UNKNOWN) win->evGauge(ev, id, this, event.m_rawCode, event.m_keyCode);
		
		event.Skip();
	}
	void EventMouse(wxMouseEvent& event)
	{
		if (!win) return;

		eGEvent ev = MapEvent(event.GetEventType());
		if (ev!=GE_UNKNOWN) 
		{
			if (ev==GE_WHEEL) win->evGauge(ev, id, this, event.m_wheelDelta, event.m_wheelRotation);
			else win->evGauge(ev, id, this, event.m_x, event.m_y);
		}

		event.Skip();
	}

private:
	wxGauge* obj; 
};

IMPLEMENT_DYNAMIC_CLASS(CGGauge, wxXmlResourceHandlerEx)

CGGauge::CGGauge()
                  :wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxGA_HORIZONTAL);
    XRC_ADD_STYLE(wxGA_VERTICAL);
#if WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxGA_PROGRESSBAR);
#endif // WXWIN_COMPATIBILITY_2_6
    XRC_ADD_STYLE(wxGA_SMOOTH);   // windows only
    AddWindowStyles();
}

wxObject *CGGauge::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxGauge)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetLong(wxT("range"), wxGAUGE_DEFAULT_RANGE),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    wxDefaultValidator,
                    GetName());

	int id = GetID();
    if( HasParam(wxT("value")))
    {
        control->SetValue(GetLong(wxT("value")));
    }
    if( HasParam(wxT("shadow")))
    {
        control->SetShadowWidth(GetDimension(wxT("shadow")));
    }
    if( HasParam(wxT("bezel")))
    {
        control->SetBezelFace(GetDimension(wxT("bezel")));
    }

    SetupWindow(control);

	// connect events
	GCOMPONENT_COMMON_INIT(Gauge, control);
	
    return control;
}

bool CGGauge::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxGauge"));
}

}; // namespace
