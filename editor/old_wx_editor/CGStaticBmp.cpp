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

#ifndef  WX_PRECOMP
#include "wx/statbmp.h"
#endif


namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGStaticBmp, wxXmlResourceHandlerEx)

CGStaticBmp::CGStaticBmp()
                         :wxXmlResourceHandlerEx()
{
    AddWindowStyles();
}

wxObject *CGStaticBmp::DoCreateResource()
{
    XRC_MAKE_INSTANCE(bmp, wxStaticBitmap)

    bmp->Create(m_parentAsWindow,
                GetID(),
                GetBitmap(wxT("bitmap"), wxART_OTHER, GetSize()),
                GetPosition(), GetSize(),
                GetStyle(),
                GetName());

    SetupWindow(bmp);

    return bmp;
}

bool CGStaticBmp::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStaticBitmap"));
}

}; // namespace
