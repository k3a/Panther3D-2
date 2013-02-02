/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once
/************************************************************************/
/* This is core Editor GUI implementation (EdGUI). It allows loading    */
/* resources from wxWidgets XRC and use it for editor gui dialogs and   */
/* widgets. This can be used on both Editor and Plugin side.            */
/************************************************************************/

#include "wx/xrc/xmlres.h"

#include "EdGUI.h"


namespace P3D {

	class wxXmlResourceHandlerEx;

	/// Extension to wxXmlResource
	/** This class is used for manipulation with wxWidgets wxc. The purpose of this extension is interconnection with EdGUI.
	    It means, when you create new window from wxRC, you can get IGWindow from EdGUI instead of wxWindow. It also handles
		events and forward it to base IGWindow interface methods, so plugin window class can handle it.
		And there is no more wxWidgets dependency from plugin side. Theoretically it is possible to implement other GUI 
		system than wxWidgets in the future.<br><br>Class is based directly on wx class so more doc there.*/
	class wxXmlResourceEx : public wxXmlResource
	{
	public:
		void Free();
		void AddHandler(wxXmlResourceHandlerEx *handler);
		void InsertHandler(wxXmlResourceHandlerEx *handler);
		void ClearHandlers();
		static wxXmlResourceEx *Get();

		wxObject *CreateResFromNode(wxXmlNode *node, wxObject *parent,
			wxObject *instance/* = NULL*/,
			wxXmlResourceHandlerEx *handlerToUse/* = NULL*/, IGWindow* implclass);

		// Loads a frame.
		wxFrame *LoadFrame(wxWindow* parent, const wxString& name, IGWindow* implclass);
		bool LoadFrame(wxFrame* frame, wxWindow *parent, const wxString& name, IGWindow* implclass);

	private:
		wxList m_handlers;
		// singleton instance:
		static wxXmlResourceEx *ms_instanceEx;

		friend class wxXmlResourceHandlerEx;
	};

	/// Extension to wxXmlHandler
	/** Class is based on wxXmlHandler, more doc. there. */
	class wxXmlResourceHandlerEx : public wxXmlResourceHandler
	{
	public:
		wxXmlResourceHandlerEx() : wxXmlResourceHandler() { m_resourceEx=NULL; m_implclass=NULL; };
		void CreateChildren(wxObject *parent, bool this_hnd_only = false);
		//void CreateChildrenPrivately(wxObject *parent, wxXmlNode *rootnode = NULL);.
		wxObject *CreateResFromNode(wxXmlNode *node,
			wxObject *parent, wxObject *instance = NULL)
		{ return m_resourceEx->CreateResFromNode(node, parent, instance, NULL, GetImplClass()); }

		void SetParentResource(wxXmlResourceEx *res) { m_resourceEx = res; }

		/// Set implementation class (client class delivered from IGWindow)
		void SetImplClass(IGWindow * cls){ m_implclass=cls; };
		/// Get implementation class (client class delivered from IGWindow)
		IGWindow *GetImplClass(){ return m_implclass; };

	protected:
		wxXmlResourceEx *m_resourceEx;
		IGWindow *m_implclass;
	};

void EdGUI_RegisterHandlers();
void EdGUI_UnregisterHandlers();
void EdGUI_EnableLogging();

/// Struct for mapping shitty wxEventType constants to eGEvent
struct sEventMap
{
	const eGEvent g;
	const wxEventType &wx;
};
extern sEventMap g_eventMap[];
inline const eGEvent MapEvent(wxEventType wet){
	for (unsigned int i=0; i<GE__LAST; i++) 
		if (g_eventMap[i].wx==wet) return g_eventMap[i].g; 
	return GE_UNKNOWN;
}

// *** begin common component body *** these are common input handlers and GetName and SetName
#define GCOMPONENT_COMMON_BODY(compName) \
	void SetName(const wchar* szName) \
{ \
	obj->SetName(wxString(szName)); \
}; \
const wchar* GetName() \
{ \
	static wxString tmpstr = obj->GetName(); \
	return tmpstr; \
} \
void EventKeyboard(wxKeyEvent& event) \
{ \
	if (!win) return; \
\
	eGEvent ev = MapEvent(event.GetEventType()); \
	if (ev!=GE_UNKNOWN) win->ev##compName##(ev, id, this, event.m_rawCode, event.m_keyCode); \
\
	event.Skip(); \
} \
void EventMouse(wxMouseEvent& event) \
{ \
	if (!win) return; \
\
	eGEvent ev = MapEvent(event.GetEventType()); \
	if (ev!=GE_UNKNOWN) \
	{ \
		if (ev==GE_WHEEL) win->ev##compName##(ev, id, this, event.m_wheelDelta, event.m_wheelRotation); \
		else win->ev##compName##(ev, id, this, event.m_x, event.m_y); \
	} \
\
	event.Skip(); \
} 
// *** end common component body ***
/// base component event connections (compName=component name i.e. Slider, compVarName=variable name i.e. control)
#define GCOMPONENT_COMMON_INIT(compName, compVarName) \
/* create wrapper and assign it to control */ \
CG##compName##W *wrp = new CG##compName##W(GetImplClass(), compVarName##); \
compVarName##->SetClientData(wrp); \
/* common keyboard evets: */ \
compVarName##->Connect( compVarName##->GetId(), wxEVT_CHAR, wxKeyEventHandler(CG##compName##W::EventKeyboard) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_KEY_UP, wxKeyEventHandler(CG##compName##W::EventKeyboard) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_KEY_DOWN, wxKeyEventHandler(CG##compName##W::EventKeyboard) , NULL, (wxEvtHandler *)wrp ); \
\
/* common mouse events: */ \
compVarName##->Connect( compVarName##->GetId(), wxEVT_LEFT_DOWN, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_RIGHT_DOWN, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_MIDDLE_DOWN, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_LEFT_UP, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_RIGHT_UP, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_MIDDLE_UP, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_LEFT_DCLICK, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_RIGHT_DCLICK, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_MOTION, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_ENTER_WINDOW, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_SET_FOCUS, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_KILL_FOCUS, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp ); \
compVarName##->Connect( compVarName##->GetId(), wxEVT_MOUSEWHEEL, wxMouseEventHandler(CG##compName##W::EventMouse) , NULL, (wxEvtHandler *)wrp );

}; // ns

#include "CGAnimation.h"
#include "CGBmp.h"
#include "CGBmpButton.h"
#include "CGBmpComboBox.h"
#include "CGButton.h"
#include "CGCheckBox.h"
#include "CGCheckList.h"
#include "CGChoice.h"
#include "CGChoiceBook.h"
#include "CGCollapsiblePane.h"
#include "CGColorPicker.h"
#include "CGCombo.h"
#include "CGDialog.h"
#include "CGDirPicker.h"
#include "CGFilePicker.h"
#include "CGFontPicker.h"
#include "CGFrame.h"
#include "CGGauge.h"
#include "CGHtml.h"
#include "CGHyperlink.h"
#include "CGListBook.h"
#include "CGListBox.h"
#include "CGList.h"
#include "CGMenu.h"
#include "CGNoteBook.h"
#include "CGPanel.h"
#include "CGRadioBox.h"
#include "CGRadioButton.h"
#include "CGRichTextBox.h"
#include "CGScrollBar.h"
#include "CGScrolledWindow.h"
#include "CGSizer.h"
#include "CGSlider.h"
#include "CGSpin.h"
#include "CGSplitterWindow.h"
#include "CGStaticBmp.h"
#include "CGStaticBox.h"
#include "CGStaticLine.h"
#include "CGStaticText.h"
#include "CGStatusBar.h"
#include "CGTextBox.h"
#include "CGToggleButton.h"
#include "CGToolBar.h"
#include "CGTree.h"
#include "CGTreeBook.h"
#include "CGUnknown.h"