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
#include "wx/statbox.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGStaticBox, wxXmlResourceHandlerEx)

CGStaticBox::CGStaticBox()
                      :wxXmlResourceHandlerEx()
{
    AddWindowStyles();
}

wxObject *CGStaticBox::DoCreateResource()
{
    XRC_MAKE_INSTANCE(box, wxStaticBox)

    box->Create(m_parentAsWindow,
                GetID(),
                GetText(wxT("label")),
                GetPosition(), GetSize(),
                GetStyle(),
                GetName());

    SetupWindow(box);

    return box;
}

bool CGStaticBox::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStaticBox"));
}

}; // namespace
