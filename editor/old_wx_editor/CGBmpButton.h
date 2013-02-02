/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_bmpbt.h
// Purpose:     XML resource handler for bitmap buttons
// Author:      Brian Gavin
// Created:     2000/03/05
// RCS-ID:      $Id: xh_bmpbt.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGBmpButton : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGBmpButton)

public:
    CGBmpButton();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


