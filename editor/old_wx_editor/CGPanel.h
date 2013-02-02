/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_panel.h
// Purpose:     XML resource handler for wxPanel
// Author:      Vaclav Slavik
// Created:     2000/03/05
// RCS-ID:      $Id: xh_panel.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGPanel : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGPanel)

public:
    CGPanel();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace