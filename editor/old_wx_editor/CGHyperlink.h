/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_hyperlink.h
// Purpose:     Hyperlink control (wxAdv)
// Author:      David Norris <danorris@gmail.com>
// Modified by: Ryan Norton, Francesco Montorsi
// Created:     04/02/2005
// RCS-ID:      $Id: xh_hyperlink.h 41590 2006-10-03 14:53:40Z VZ $
// Copyright:   (c) 2005 David Norris
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////




#include "wx/xrc/xmlres.h"

namespace P3D {

class WXDLLIMPEXP_XRC CGHyperlink : public wxXmlResourceHandlerEx
{
    // Register with wxWindows' dynamic class subsystem.
    DECLARE_DYNAMIC_CLASS(CGHyperlink)

public:
    // Constructor.
    CGHyperlink();

    // Creates the control and returns a pointer to it.
    virtual wxObject *DoCreateResource();

    // Returns true if we know how to create a control for the given node.
    virtual bool CanHandle(wxXmlNode *node);
};

}; // namespace


