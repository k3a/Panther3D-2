/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_text.h
// Purpose:     XML resource handler for wxTextCtrl
// Author:      Aleksandras Gluchovas
// Created:     2000/03/21
// RCS-ID:      $Id: xh_text.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Aleksandras Gluchovas
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGTextBox : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGTextBox)

public:
    CGTextBox();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


