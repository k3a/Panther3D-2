/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_frame.h
// Purpose:     XML resource handler for wxFrame
// Author:      Vaclav Slavik & Aleks.
// Created:     2000/03/05
// RCS-ID:      $Id: xh_frame.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




//#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGFrame : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGFrame)

public:
    CGFrame();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};



}; // namespace
