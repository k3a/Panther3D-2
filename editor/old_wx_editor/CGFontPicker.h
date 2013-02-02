/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_fontpicker.h
// Purpose:     XML resource handler for wxFontPickerCtrl
// Author:      Francesco Montorsi
// Created:     2006-04-17
// RCS-ID:      $Id: xh_fontpicker.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2006 Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGFontPicker : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGFontPicker)

public:
    CGFontPicker();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


