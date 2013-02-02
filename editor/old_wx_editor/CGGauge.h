/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_gauge.h
// Purpose:     XML resource handler for wxGauge
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_gauge.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGGauge : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGGauge)
    enum
    {
        wxGAUGE_DEFAULT_RANGE = 100
    };

public:
    CGGauge();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


