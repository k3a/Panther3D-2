/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_unkwn.h
// Purpose:     XML resource handler for unkown widget
// Author:      Vaclav Slavik
// Created:     2000/03/05
// RCS-ID:      $Id: xh_unkwn.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGUnknown : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGUnknown)

public:
    CGUnknown();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};




}; // namespace