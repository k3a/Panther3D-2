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
#endif

#include "wx/hyperlink.h"
#include "wx/xrc/xmlres.h"

namespace P3D {

//===========================================================================
// Implementation
//===========================================================================

//---------------------------------------------------------------------------
// CGHyperlink
//---------------------------------------------------------------------------

// Register with wxWindows' dynamic class subsystem.
IMPLEMENT_DYNAMIC_CLASS(CGHyperlink, wxXmlResourceHandlerEx)

CGHyperlink::CGHyperlink()
{
    XRC_ADD_STYLE(wxHL_CONTEXTMENU);
    XRC_ADD_STYLE(wxHL_ALIGN_LEFT);
    XRC_ADD_STYLE(wxHL_ALIGN_RIGHT);
    XRC_ADD_STYLE(wxHL_ALIGN_CENTRE);
    XRC_ADD_STYLE(wxHL_DEFAULT_STYLE);

    AddWindowStyles();
}

wxObject *CGHyperlink::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxHyperlinkCtrl)

    control->Create(m_parentAsWindow, GetID(),
        GetParamValue(wxT("label")), GetParamValue(wxT("url")),
        GetPosition(), GetSize(),
        GetStyle(wxT("style"), wxHL_DEFAULT_STYLE),
        GetName());

    SetupWindow(control);

    return control;
}

bool CGHyperlink::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxHyperlinkCtrl"));
}

}; // namespace
