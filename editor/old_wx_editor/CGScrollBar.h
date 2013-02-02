/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_scrol.h
// Purpose:     XML resource handler for wxScrollBar
// Author:      Brian Gavin
// Created:     2000/09/09
// RCS-ID:      $Id: xh_scrol.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGScrollBar : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGScrollBar)
    enum
    {
        wxSL_DEFAULT_VALUE = 0,
        wxSL_DEFAULT_MIN = 0,
        wxSL_DEFAULT_MAX = 100
    };

public:
    CGScrollBar();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


