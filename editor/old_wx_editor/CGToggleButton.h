/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_tglbtn.h
// Purpose:     XML resource handler for wxToggleButton
// Author:      Julian Smart
// Created:     2004-08-30
// RCS-ID:      $Id: xh_tglbtn.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2004 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGToggleButton : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGToggleButton)

public:
    CGToggleButton();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


