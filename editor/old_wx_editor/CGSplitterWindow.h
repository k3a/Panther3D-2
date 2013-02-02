/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_split.h
// Purpose:     XRC resource for wxSplitterWindow
// Author:      panga@freemail.hu, Vaclav Slavik
// Created:     2003/01/26
// RCS-ID:      $Id: xh_split.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2003 panga@freemail.hu, Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGSplitterWindow : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGSplitterWindow)

public:
    CGSplitterWindow();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


