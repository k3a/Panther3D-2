/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_stlin.h
// Purpose:     XML resource handler for wxStaticLine
// Author:      Vaclav Slavik
// Created:     2000/09/00
// RCS-ID:      $Id: xh_stlin.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGStaticLine : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGStaticLine)

public:
    CGStaticLine();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


