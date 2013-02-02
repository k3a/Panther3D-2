/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_radbt.h
// Purpose:     XML resource handler for wxRadioButton
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_radbt.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGRadioButton : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGRadioButton)

public:
    CGRadioButton();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


