/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_stbmp.h
// Purpose:     XML resource handler for wxStaticBitmap
// Author:      Vaclav Slavik
// Created:     2000/04/22
// RCS-ID:      $Id: xh_stbmp.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGStaticBmp : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGStaticBmp)

public:
    CGStaticBmp();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


