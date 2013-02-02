/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"
/*
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

 

#include "CGUI.h"

namespace P3D {

#include "wx/spinbutt.h"

IMPLEMENT_DYNAMIC_CLASS(CGSpin, wxXmlResourceHandlerEx)

CGSpin::CGSpin()
: wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxSP_HORIZONTAL);
    XRC_ADD_STYLE(wxSP_VERTICAL);
    XRC_ADD_STYLE(wxSP_ARROW_KEYS);
    XRC_ADD_STYLE(wxSP_WRAP);
    AddWindowStyles();
}

wxObject *CGSpin::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxSpinButton)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetPosition(), GetSize(),
                    GetStyle(wxT("style"), wxSP_VERTICAL | wxSP_ARROW_KEYS),
                    GetName());

    control->SetValue(GetLong( wxT("value"), wxSP_DEFAULT_VALUE));
    control->SetRange(GetLong( wxT("min"), wxSP_DEFAULT_MIN),
                      GetLong(wxT("max"), wxSP_DEFAULT_MAX));
    SetupWindow(control);

    return control;
}

bool CGSpin::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxSpinButton"));
}





#include "wx/spinctrl.h"

IMPLEMENT_DYNAMIC_CLASS(wxSpinCtrlXmlHandler, wxXmlResourceHandlerEx)

wxSpinCtrlXmlHandler::wxSpinCtrlXmlHandler()
: wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxSP_HORIZONTAL);
    XRC_ADD_STYLE(wxSP_VERTICAL);
    XRC_ADD_STYLE(wxSP_ARROW_KEYS);
    XRC_ADD_STYLE(wxSP_WRAP);
}

wxObject *wxSpinCtrlXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxSpinCtrl)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetText(wxT("value")),
                    GetPosition(), GetSize(),
                    GetStyle(wxT("style"), wxSP_ARROW_KEYS),
                    GetLong(wxT("min"), wxSP_DEFAULT_MIN),
                    GetLong(wxT("max"), wxSP_DEFAULT_MAX),
                    GetLong(wxT("value"), wxSP_DEFAULT_VALUE),
                    GetName());

    SetupWindow(control);

    return control;
}

bool wxSpinCtrlXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxSpinCtrl"));
}




}; // namespace

*/