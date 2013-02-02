/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_scwin.h
// Purpose:     XML resource handler for wxScrolledWindow
// Author:      Vaclav Slavik
// Created:     2002/10/18
// RCS-ID:      $Id: xh_scwin.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2002 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGScrolledWindow : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGScrolledWindow)

public:
    CGScrolledWindow();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};




}; // namespace