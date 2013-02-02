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
    #include "wx/bitmap.h"
#endif

namespace P3D {

IMPLEMENT_DYNAMIC_CLASS(CGBmp, wxXmlResourceHandlerEx)

CGBmp::CGBmp()
                   :wxXmlResourceHandlerEx()
{
}

wxObject *CGBmp::DoCreateResource()
{
    // NB: empty parameter name means "take directly from this node's next
    //     instead of from subnode with given name"
    return new wxBitmap(GetBitmap(wxEmptyString));
}

bool CGBmp::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxBitmap"));
}

IMPLEMENT_DYNAMIC_CLASS(wxIconXmlHandler, wxXmlResourceHandlerEx)

wxIconXmlHandler::wxIconXmlHandler()
: wxXmlResourceHandler()
{
}

wxObject *wxIconXmlHandler::DoCreateResource()
{
    // NB: empty parameter name means "take directly from this node's next
    //     instead of from subnode with given name"
    return new wxIcon(GetIcon(wxEmptyString));
}

bool wxIconXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxIcon"));
}

}; // namespace

