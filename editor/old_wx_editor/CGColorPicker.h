/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_clrpicker.h
// Purpose:     XML resource handler for wxColourPickerCtrl
// Author:      Francesco Montorsi
// Created:     2006-04-17
// RCS-ID:      $Id: xh_clrpicker.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2006 Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGColorPicker : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGColorPicker)

public:
    CGColorPicker();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


