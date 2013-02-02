/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_tree.h
// Purpose:     XML resource handler for wxTreeCtrl
// Author:      Brian Gavin
// Created:     2000/09/09
// RCS-ID:      $Id: xh_tree.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGTree : public wxXmlResourceHandlerEx
{
    DECLARE_DYNAMIC_CLASS(CGTree)

public:
    CGTree();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


