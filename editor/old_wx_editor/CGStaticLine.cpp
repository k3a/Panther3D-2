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
#include "wx/statline.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGStaticLine, wxXmlResourceHandlerEx)

CGStaticLine::CGStaticLine()
: wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxLI_HORIZONTAL);
    XRC_ADD_STYLE(wxLI_VERTICAL);
    AddWindowStyles();
}

wxObject *CGStaticLine::DoCreateResource()
{
    XRC_MAKE_INSTANCE(line, wxStaticLine)

    line->Create(m_parentAsWindow,
                GetID(),
                GetPosition(), GetSize(),
                GetStyle(wxT("style"), wxLI_HORIZONTAL),
                GetName());

    SetupWindow(line);

    return line;
}

bool CGStaticLine::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStaticLine"));
}

}; // namespace
