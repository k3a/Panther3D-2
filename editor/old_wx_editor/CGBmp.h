/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_bmp.h
// Purpose:     XML resource handler for wxBitmap and wxIcon
// Author:      Vaclav Slavik
// Created:     2000/09/00
// RCS-ID:      $Id: xh_bmp.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGBmp : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGBmp)

public:
    CGBmp();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

class WXDLLIMPEXP_XRC wxIconXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(wxIconXmlHandler)

public:
    wxIconXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};


}; // namespace


