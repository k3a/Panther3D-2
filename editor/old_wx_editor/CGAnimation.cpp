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
#include "wx/animate.h"

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGAnimation, wxXmlResourceHandlerEx)

CGAnimation::CGAnimation() : wxXmlResourceHandlerEx()
{
    XRC_ADD_STYLE(wxAC_NO_AUTORESIZE);
    XRC_ADD_STYLE(wxAC_DEFAULT_STYLE);
    AddWindowStyles();
}

wxObject *CGAnimation::DoCreateResource()
{
    XRC_MAKE_INSTANCE(ctrl, wxAnimationCtrl)

    ctrl->Create(m_parentAsWindow,
                  GetID(),
                  GetAnimation(wxT("animation")),
                  GetPosition(), GetSize(),
                  GetStyle(_T("style"), wxAC_DEFAULT_STYLE),
                  GetName());

    // if no inactive-bitmap has been provided, GetBitmap() will return wxNullBitmap
    // which just tells wxAnimationCtrl to use the default for inactive status
    ctrl->SetInactiveBitmap(GetBitmap(wxT("inactive-bitmap")));

    SetupWindow(ctrl);

    return ctrl;
}

bool CGAnimation::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxAnimationCtrl"));
}

}; // namespace
