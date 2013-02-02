/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "CGUI.h"
#include "editor.h" // for logging (CON makro) and all wxWidgets includes!

namespace P3D {


	void wxXmlResourceHandlerEx::CreateChildren(wxObject *parent, bool this_hnd_only)
	{
		wxXmlNode *n = m_node->GetChildren();

		while (n)
		{
			if (n->GetType() == wxXML_ELEMENT_NODE &&
				(n->GetName() == wxT("object") || n->GetName() == wxT("object_ref")))
			{
				m_resourceEx->CreateResFromNode(n, parent, NULL,
					this_hnd_only ? this : NULL, GetImplClass());
			}
			n = n->GetNext();
		}
	}


// --------------------------------------------------------------------------




static void MergeNodes(wxXmlNode& dest, wxXmlNode& with)
{
    // Merge properties:
    for (wxXmlProperty *prop = with.GetAttributes(); prop; prop = prop->GetNext())
    {
        wxXmlProperty *dprop;
        for (dprop = dest.GetAttributes(); dprop; dprop = dprop->GetNext())
        {

            if ( dprop->GetName() == prop->GetName() )
            {
                dprop->SetValue(prop->GetValue());
                break;
            }
        }

        if ( !dprop )
            dest.AddAttribute(prop->GetName(), prop->GetValue());
   }

    // Merge child nodes:
    for (wxXmlNode* node = with.GetChildren(); node; node = node->GetNext())
    {
        wxString name = node->GetAttribute(wxT("name"), wxEmptyString);
        wxXmlNode *dnode;

        for (dnode = dest.GetChildren(); dnode; dnode = dnode->GetNext() )
        {
            if ( dnode->GetName() == node->GetName() &&
                 dnode->GetAttribute(wxT("name"), wxEmptyString) == name &&
                 dnode->GetType() == node->GetType() )
            {
                MergeNodes(*dnode, *node);
                break;
            }
        }

        if ( !dnode )
        {
            static const wxChar *AT_END = wxT("end");
            wxString insert_pos = node->GetAttribute(wxT("insert_at"), AT_END);
            if ( insert_pos == AT_END )
            {
                dest.AddChild(new wxXmlNode(*node));
            }
            else if ( insert_pos == wxT("begin") )
            {
                dest.InsertChild(new wxXmlNode(*node), dest.GetChildren());
            }
        }
    }

    if ( dest.GetType() == wxXML_TEXT_NODE && with.GetContent().length() )
         dest.SetContent(with.GetContent());
}

wxXmlResourceEx *wxXmlResourceEx::ms_instanceEx = NULL;

/*static*/ wxXmlResourceEx *wxXmlResourceEx::Get()
{
	if ( !ms_instanceEx )
		ms_instanceEx = new wxXmlResourceEx;
	return ms_instanceEx;
}

void wxXmlResourceEx::Free()
{
	wxList::compatibility_iterator ND = m_handlers.GetFirst();
	while (ND)
	{
		wxXmlResourceHandlerEx* handler = (wxXmlResourceHandlerEx*)ND->GetData();
		delete handler;
		ND = ND->GetNext();
	}

	ClearHandlers();

	delete this;
}

void wxXmlResourceEx::AddHandler(wxXmlResourceHandlerEx *handler)
{
	m_handlers.Append(handler);
	handler->SetParentResource(this);
	((wxXmlResourceHandler*)handler)->SetParentResource(this);
}

void wxXmlResourceEx::InsertHandler(wxXmlResourceHandlerEx *handler)
{
	m_handlers.Insert(handler);
	handler->SetParentResource(this);
	((wxXmlResourceHandler*)handler)->SetParentResource(this);
}

void wxXmlResourceEx::ClearHandlers()
{
	//WX_CLEAR_LIST(wxList, m_handlers);
}

wxObject *wxXmlResourceEx::CreateResFromNode(wxXmlNode *node, wxObject *parent,
                                           wxObject *instance,
                                           wxXmlResourceHandlerEx *handlerToUse,
										   IGWindow* implclass)
{
    if (node == NULL) return NULL;

    // handling of referenced resource
    if ( node->GetName() == wxT("object_ref") )
    {
        wxString refName = node->GetAttribute(wxT("ref"), wxEmptyString);
        wxXmlNode* refNode = FindResource(refName, wxEmptyString, true);

        if ( !refNode )
        {
            wxLogError(_("Referenced object node with ref=\"%s\" not found!"),
                       refName.c_str());
            return NULL;
        }

        wxXmlNode copy(*refNode);
		MergeNodes(copy, *node);

        return CreateResFromNode(&copy, parent, instance, NULL, implclass);
    }

    wxXmlResourceHandlerEx *handler;

    if (handlerToUse)
    {
        if (handlerToUse->CanHandle(node))
        {
			handlerToUse->SetImplClass(implclass);
            return handlerToUse->CreateResource(node, parent, instance);
        }
    }
    else if (node->GetName() == wxT("object"))
    {
        wxList::compatibility_iterator ND = m_handlers.GetFirst();
        while (ND)
        {
            handler = (wxXmlResourceHandlerEx*)ND->GetData();
            if (handler->CanHandle(node))
            {
				handler->SetImplClass(implclass);
                return handler->CreateResource(node, parent, instance);
            }
            ND = ND->GetNext();
        }
    }

    wxLogError(_("No handler found for XML node '%s', class '%s'!"),
               node->GetName().c_str(),
               node->GetAttribute(wxT("class"), wxEmptyString).c_str());
    return NULL;
}




wxFrame *wxXmlResourceEx::LoadFrame(wxWindow* parent, const wxString& name, IGWindow* implclass)
{
	return (wxFrame*)CreateResFromNode(FindResource(name, wxT("wxFrame")), parent, NULL, NULL, implclass);
}

bool wxXmlResourceEx::LoadFrame(wxFrame* frame, wxWindow *parent, const wxString& name, IGWindow* implclass)
{
	return CreateResFromNode(FindResource(name, wxT("wxFrame")), parent, frame, NULL, implclass) != NULL;
}













/// Function for widgets registration.
/** These widgets will be created by wxRC extended system when requested by EdGUI.
    It is possible to define new handlers and register them here.
	For example when we need some new "graph" widget. */
void EdGUI_RegisterHandlers()
{
	wxXmlResourceEx::Get()->AddHandler(new CGAnimation);
	wxXmlResourceEx::Get()->AddHandler(new CGBmp);
	wxXmlResourceEx::Get()->AddHandler(new CGBmpButton);
	wxXmlResourceEx::Get()->AddHandler(new CGBmpComboBox);
	wxXmlResourceEx::Get()->AddHandler(new CGButton);
	wxXmlResourceEx::Get()->AddHandler(new CGCheckBox);
	wxXmlResourceEx::Get()->AddHandler(new CGCheckList);
	wxXmlResourceEx::Get()->AddHandler(new CGChoice);
	wxXmlResourceEx::Get()->AddHandler(new CGChoiceBook);
	wxXmlResourceEx::Get()->AddHandler(new CGCollapsiblePane);
	wxXmlResourceEx::Get()->AddHandler(new CGColorPicker);
	wxXmlResourceEx::Get()->AddHandler(new CGCombo);
	wxXmlResourceEx::Get()->AddHandler(new CGDialog);
	wxXmlResourceEx::Get()->AddHandler(new CGDirPicker);
	wxXmlResourceEx::Get()->AddHandler(new CGFilePicker);
	wxXmlResourceEx::Get()->AddHandler(new CGFontPicker);
	wxXmlResourceEx::Get()->AddHandler(new CGFrame);
	wxXmlResourceEx::Get()->AddHandler(new CGGauge);
	wxXmlResourceEx::Get()->AddHandler(new CGHtml);
	wxXmlResourceEx::Get()->AddHandler(new CGHyperlink);
	wxXmlResourceEx::Get()->AddHandler(new CGListBook);
	wxXmlResourceEx::Get()->AddHandler(new CGListBox);
	wxXmlResourceEx::Get()->AddHandler(new CGList);
	wxXmlResourceEx::Get()->AddHandler(new CGMenu);
	wxXmlResourceEx::Get()->AddHandler(new CGNoteBook);
	wxXmlResourceEx::Get()->AddHandler(new CGPanel);
	wxXmlResourceEx::Get()->AddHandler(new CGRadioBox);
	wxXmlResourceEx::Get()->AddHandler(new CGRadioButton);
	wxXmlResourceEx::Get()->AddHandler(new CGRichTextBox);
	wxXmlResourceEx::Get()->AddHandler(new CGScrollBar);
	wxXmlResourceEx::Get()->AddHandler(new CGScrolledWindow);
	wxXmlResourceEx::Get()->AddHandler(new CGSizer);
	wxXmlResourceEx::Get()->AddHandler(new CGStdDialogButtonSizer);
	wxXmlResourceEx::Get()->AddHandler(new CGSlider);
	//wxXmlResource::Get()->AddHandler(new CGSpin);
	wxXmlResourceEx::Get()->AddHandler(new CGSplitterWindow);
	wxXmlResourceEx::Get()->AddHandler(new CGStaticBmp);
	wxXmlResourceEx::Get()->AddHandler(new CGStaticBox);
	wxXmlResourceEx::Get()->AddHandler(new CGStaticLine);
	wxXmlResourceEx::Get()->AddHandler(new CGStaticText);
	wxXmlResourceEx::Get()->AddHandler(new CGStatusBar);
	wxXmlResourceEx::Get()->AddHandler(new CGTextBox);
	wxXmlResourceEx::Get()->AddHandler(new CGToggleButton);
	wxXmlResourceEx::Get()->AddHandler(new CGToolBar);
	wxXmlResourceEx::Get()->AddHandler(new CGTree);
	wxXmlResourceEx::Get()->AddHandler(new CGTreeBook);
	wxXmlResourceEx::Get()->AddHandler(new CGUnknown);
}

void EdGUI_UnregisterHandlers()
{
	wxXmlResourceEx::Get()->Free();
}


class EdGUILog : public wxLog
{
public:
	EdGUILog()
	{
		wxLog::EnableLogging();
		wxLog::SetActiveTarget(this);
	}
	/*void DoLog(wxLogLevel level, const wxChar *szString, time_t t)
	{
		CON(MSG_DBG, _W("Wx(%d): %s"), level, szString);
	}*/
	void DoLogString(const wxChar *szString, time_t t)
	{	
		CON(MSG_DBG, _W("Wx: %s"), szString);
	}
};

void EdGUI_EnableLogging()
{
	/*const wxEventTypeTag<wxKeyEvent> test;
	test.operator const wxEventType &*/
	EdGUILog *logger = new EdGUILog();
}

// wxWidgets 2.8.10 changed wxEVT_* to wxEventTag templates, so I use  .() and remove & at the beginning
// event mapping
sEventMap g_eventMap[GE__LAST] =
{
	GE_UNKNOWN, wxEVT_NULL,
	GE_CHAR, wxEVT_CHAR,
	GE_KEY_UP, wxEVT_KEY_UP, 
	GE_KEY_DN, wxEVT_KEY_DOWN, 
	//--- mouse events:
	GE_LEFT_UP, wxEVT_LEFT_UP,
	GE_LEFT_DN, wxEVT_LEFT_DOWN,
	GE_MIDDLE_UP, wxEVT_MIDDLE_UP,
	GE_MIDDLE_DN, wxEVT_MIDDLE_DOWN,
	GE_RIGHT_UP, wxEVT_RIGHT_UP,
	GE_RIGHT_DN, wxEVT_RIGHT_DOWN,
	GE_MOTION, wxEVT_MOTION,
	GE_ENTER, wxEVT_ENTER_WINDOW,
	GE_LEAVE, wxEVT_LEAVE_WINDOW,
	GE_WHEEL, wxEVT_MOUSEWHEEL,
	//---
	GE_GOT_FOCUS, wxEVT_SET_FOCUS,
	GE_LOST_FOCUS, wxEVT_KILL_FOCUS,
	//--- component-specific:
	GE_BUTTON_CLICK, wxEVT_COMMAND_BUTTON_CLICKED,
	GE_TEXT_CHANGE, wxEVT_COMMAND_TEXT_UPDATED, 
	GE_TEXT_ENTER, wxEVT_COMMAND_TEXT_ENTER, 
	GE_CHECKBOX_CLICK, wxEVT_COMMAND_CHECKBOX_CLICKED,
	GE_CHOICE_SELECT, wxEVT_COMMAND_CHOICE_SELECTED,
	GE_LISTBOX_SELECT, wxEVT_COMMAND_LISTBOX_SELECTED,
	GE_LISTBOX_DCLICK, wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,
	GE_CHECKLISTBOX_TOGGLE, wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,
	GE_MENU_SELECT, wxEVT_COMMAND_MENU_SELECTED,
	GE_SLIDER_CHANGE, wxEVT_COMMAND_SLIDER_UPDATED,
	GE_RADIOBOX_SELECT, wxEVT_COMMAND_RADIOBOX_SELECTED,
	GE_RADIOBUTTON_SELECT, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
	GE_SCROLLBAR_CHANGE, wxEVT_COMMAND_SCROLLBAR_UPDATED,
	GE_VLBOX_SELECT, wxEVT_COMMAND_VLBOX_SELECTED,
	GE_COMBOBOX_SELECT, wxEVT_COMMAND_COMBOBOX_SELECTED,
	GE_TOOL_RCLICK, wxEVT_COMMAND_TOOL_RCLICKED,
	GE_TOOL_ENTER, wxEVT_COMMAND_TOOL_ENTER,
	/*GE_SPIN_CHANGE, wxEVT_COMMAND_SPINCTRL_UPDATED,*/
	GE_TOGGLEBUTTON_CLICK, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
	//---system events:
	GE_SIZE, wxEVT_SIZE,
	GE_MOVE, wxEVT_MOVE,
	GE_CLOSE, wxEVT_CLOSE_WINDOW,
	/// window activated (got focus)
	GE_ACTIVATE, wxEVT_ACTIVATE,
	/// window shown
	GE_SHOW, wxEVT_SHOW,
	GE_MINIMIZE, wxEVT_ICONIZE,
	GE_MAXIMIZE, wxEVT_MAXIMIZE,
	/// idle event useful for rendering
	GE_IDLE, wxEVT_IDLE,
};

}; // namespace