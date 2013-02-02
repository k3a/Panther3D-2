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
#include "wx/slider.h"
#endif

namespace P3D {

/// Slider wrapper for EdGUI implementing interface (see. EdGUI.h for more info)
/** This wrapper classes are used to allow EdGUI client (plugin) to manipulate with GUI system - wxWidgets without 
dependency to original GUI API headers and libraries */
class CGSliderW : public IGSlider, public wxEvtHandler {
public:

	CGSliderW(IGWindow* pwin, wxSlider* pobj){obj=pobj; win=pwin; id=obj->GetId(); pwin->AddSlider(this); };
	~CGSliderW(){};
	void SetValue(int val){
		obj->SetValue(val);
	};
	int GetValue()const{
		return obj->GetValue();
	};
	void SetMinValue(int val){
		obj->SetMin(val);
	};
	int GetMinValue()const{
		return obj->GetMin();
	};
	void SetMaxValue(int val){
		obj->SetMax(val);
	};
	int GetMaxValue()const{
		return obj->GetMax();
	};

	GCOMPONENT_COMMON_BODY(Slider);

	void EventCommand(wxCommandEvent& event)
	{
		if (!win) return;
		win->evSlider(GE_SLIDER_CHANGE, id, this);
		event.Skip();
	}

private:
	wxSlider* obj; 
};

IMPLEMENT_DYNAMIC_CLASS(CGSlider, wxXmlResourceHandlerEx)

CGSlider::CGSlider()
                   :wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxSL_HORIZONTAL);
    XRC_ADD_STYLE(wxSL_VERTICAL);
    XRC_ADD_STYLE(wxSL_AUTOTICKS);
    XRC_ADD_STYLE(wxSL_LABELS);
    XRC_ADD_STYLE(wxSL_LEFT);
    XRC_ADD_STYLE(wxSL_TOP);
    XRC_ADD_STYLE(wxSL_RIGHT);
    XRC_ADD_STYLE(wxSL_BOTTOM);
    XRC_ADD_STYLE(wxSL_BOTH);
    XRC_ADD_STYLE(wxSL_SELRANGE);
    XRC_ADD_STYLE(wxSL_INVERSE);
    AddWindowStyles();
}

wxObject *CGSlider::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxSlider)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetLong(wxT("value"), wxSL_DEFAULT_VALUE),
                    GetLong(wxT("min"), wxSL_DEFAULT_MIN),
                    GetLong(wxT("max"), wxSL_DEFAULT_MAX),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    wxDefaultValidator,
                    GetName());

    if( HasParam(wxT("tickfreq")))
    {
        control->SetTickFreq(GetLong(wxT("tickfreq")), 0);
    }
    if( HasParam(wxT("pagesize")))
    {
        control->SetPageSize(GetLong(wxT("pagesize")));
    }
    if( HasParam(wxT("linesize")))
    {
        control->SetLineSize(GetLong(wxT("linesize")));
    }
    if( HasParam(wxT("thumb")))
    {
        control->SetThumbLength(GetLong(wxT("thumb")));
    }
    if( HasParam(wxT("tick")))
    {
        control->SetTick(GetLong(wxT("tick")));
    }
    if( HasParam(wxT("selmin")) && HasParam(wxT("selmax")))
    {
        control->SetSelection(GetLong(wxT("selmin")), GetLong(wxT("selmax")));
    }

    SetupWindow(control);

	GCOMPONENT_COMMON_INIT(Slider, control);

	// slider events
	control->Connect( control->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(CGSliderW::EventCommand) , NULL, (wxEvtHandler *)wrp );

    return control;
}

bool CGSlider::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxSlider"));
}

}; // namespace
