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
#include "wx/stattext.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGStaticText, wxXmlResourceHandlerEx)

CGStaticText::CGStaticText()
: wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxST_NO_AUTORESIZE);
    XRC_ADD_STYLE(wxALIGN_LEFT);
    XRC_ADD_STYLE(wxALIGN_RIGHT);
    XRC_ADD_STYLE(wxALIGN_CENTRE);
    AddWindowStyles();
}

wxObject *CGStaticText::DoCreateResource()
{
    XRC_MAKE_INSTANCE(text, wxStaticText)

    text->Create(m_parentAsWindow,
                 GetID(),
                 GetText(wxT("label")),
                 GetPosition(), GetSize(),
                 GetStyle(),
                 GetName());

    SetupWindow(text);

    long wrap = GetLong(wxT("wrap"), -1);
    if (wrap != -1)
        text->Wrap(wrap);

    return text;
}

bool CGStaticText::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStaticText"));
}

}; // namespace
