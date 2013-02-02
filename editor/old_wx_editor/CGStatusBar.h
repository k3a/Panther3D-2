/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_statbar.h
// Purpose:     XML resource handler for wxStatusBar
// Author:      Brian Ravnsgaard Riis
// Created:     2004/01/21
// RCS-ID:
// Copyright:   (c) 2004 Brian Ravnsgaard Riis
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGStatusBar : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGStatusBar)

public:
    CGStatusBar();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


