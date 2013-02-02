/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_stbox.h
// Purpose:     XML resource handler for wxStaticBox
// Author:      Brian Gavin
// Created:     2000/09/00
// RCS-ID:      $Id: xh_stbox.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGStaticBox : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGStaticBox)

public:
    CGStaticBox();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


