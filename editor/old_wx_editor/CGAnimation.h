/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once
#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGAnimation : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGAnimation)

public:
    CGAnimation();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


