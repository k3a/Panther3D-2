/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

#include <vector>

namespace P3D{

// forward decl.
class IGWindow;

/// EdGUI event types
enum eGEvent
{
	GE_UNKNOWN=0,
	//--- keyboard events:
	/// character typed (value1=raw code, value2=scan code)
	GE_CHAR,
	/// keyboard button up (value1=raw code, value2=scan code)
	GE_KEY_UP,
	/// keyboard button down (value1=raw code, value2=scan code)
	GE_KEY_DN,
	//--- mouse events:
	/// left mouse button up
	GE_LEFT_UP,
	/// left mouse button down
	GE_LEFT_DN,
	/// middle mouse button up
	GE_MIDDLE_UP,
	/// middle mouse button down
	GE_MIDDLE_DN,
	/// right mouse button up
	GE_RIGHT_UP,
	/// right mouse button down
	GE_RIGHT_DN,
	/// mouse moving over component (value1=x, value2=y)
	GE_MOTION,
	/// mouse entered componnet
	GE_ENTER,
	/// mouse leaved component
	GE_LEAVE,
	/// mouse whell used (value1=wheel delta, value2=wheel rotation)
	GE_WHEEL,
	/// component got focus
	GE_GOT_FOCUS,
	/// component lost focus
	GE_LOST_FOCUS,
	//--- component-specific:
	/// button click
	GE_BUTTON_CLICK,
	/// text changed in textbox, richtextbox and similar components
	GE_TEXT_CHANGE,
	/// enter pressed in textbox (only for textboxes with enter allowed)
	GE_TEXT_ENTER,
	GE_CHECKBOX_CLICK,
	GE_CHOICE_SELECT,
	GE_LISTBOX_SELECT,
	GE_LISTBOX_DCLICK,
	GE_CHECKLISTBOX_TOGGLE,
	GE_MENU_SELECT,
	GE_SLIDER_CHANGE,
	GE_RADIOBOX_SELECT,
	GE_RADIOBUTTON_SELECT,
	GE_SCROLLBAR_CHANGE,
	GE_VLBOX_SELECT,
	GE_COMBOBOX_SELECT,
	GE_TOOL_RCLICK,
	GE_TOOL_ENTER,
	/*GE_SPIN_CHANGE,*/
	GE_TOGGLEBUTTON_CLICK,
	//---system events:
	/// window changed size
	GE_SIZE,
	/// window moved
	GE_MOVE,
	/// window closed
	GE_CLOSE,
	/// window activated (got focus)
	GE_ACTIVATE,
	/// window shown
	GE_SHOW,
	/// window minimized
	GE_MINIMIZE,
	/// window maximized
	GE_MAXIMIZE,
	/// idle event useful for rendering
	GE_IDLE,

	/// last event - internal usage. Don't remove!
	GE__LAST,
};

/// Base EdGUI Component class with common methods
class IGBase 
{
public:
	virtual ~IGBase(){};
	virtual const wchar* GetName(){return NULL;};
	virtual void SetName(const wchar* szName){};
	bool IsNameOf(const wchar* szName){ return !wstrcmp(GetName(),szName); };
	//
	unsigned int id;
	IGWindow* win; // parent (window) - where events will be sended
};

// -------- Widget wrappers - interfaces delivered by EdGUI itself offering ability to manipulate with widgets on run-time

/// Slider manipulation class
/** Using this class you can manipulate with widget (i.e. change its properties on events) */
class IGSlider : public IGBase {
public:
	/// Set new gauge value
	virtual void SetValue(int val)=0;
	/// Get current gauge value
	virtual int GetValue()const=0;
	/// Set minimum slider value
	virtual void SetMinValue(int val)=0;
	/// Get minimum slider value
	virtual int GetMinValue()const=0;
	/// Set minimum slider value
	virtual void SetMaxValue(int val)=0;
	/// Get minimum slider value
	virtual int GetMaxValue()const=0;
};

/// Gauge (ProgressBar) manipulation class
/** Using this class you can manipulate with widget (i.e. change its properties on events) */
class IGGauge : public IGBase {
public:
	/// Set new gauge value
	virtual void SetValue(int val)=0;
	/// Get current gauge value
	virtual int GetValue()const=0;
	/// Set range (value which equals maximum)
	virtual void SetRange(int val)=0;
	/// Get range (value which equals maximum)
	virtual int GetRange()const=0;
};

/// Button manipulation class
/** Using this class you can manipulate with widget (i.e. change its properties on events) */
class IGButton : public IGBase {
public:
	virtual void SetCaption(const wchar* szCaption)=0;
	virtual const wchar* GetCaption()=0;
};

/// TextBox manipulation class
/** Using this class you can manipulate with widget (i.e. change its properties on events) */
class IGTextBox : public IGBase {
public:
	virtual void SetText(const wchar* szText)=0;
	virtual const wchar* GetText()=0;
};

// -------- IGWindow is used in client side (client will derive it)
#define GCOMPONENT(vector, clsname, name) \
	void Add##name##(##clsname##* p){##vector##.push_back(p);}; \
	clsname##* Get##name##ById(unsigned int id) { for(unsigned int i=0; i<##vector##.size(); i++) { if (##vector##[i]->id==id) return vector##[i]; } return NULL; }; \
	clsname##* Get##name##ByName(const wchar* name) { for(unsigned int i=0; i<##vector##.size(); i++) { if (!wstrcmp(##vector##[i]->GetName(),name)) return vector##[i]; } return NULL; };

/// Base EdGUI Window interface.
/** This interface must be derived by window handling class to get window widget's events and messages and be able to react to them. */
class IGWindow {
public:
	/// Called on some button event
	virtual void evButton(eGEvent ev, unsigned int id, IGButton* obj, int value1=0, int value2=0){};
	/// Called on some TextBox event
	virtual void evTextBox(eGEvent ev, unsigned int id, IGTextBox* obj, int value1=0, int value2=0){};
	/// Called on some Gauge (ProgressBar) event
	virtual void evGauge(eGEvent ev, unsigned int id, IGGauge* obj, int value1=0, int value2=0){};
	/// Called on some Slider event
	virtual void evSlider(eGEvent ev, unsigned int id, IGSlider* obj, int value1=0, int value2=0){};
	
	// declare helper manipulation methods AddComponent, GetComponentByID and GetComponentByName
	GCOMPONENT(m_buttons, IGButton, Button);
	GCOMPONENT(m_textboxes, IGTextBox, TextBox);
	GCOMPONENT(m_gauges, IGGauge, Gauge);
	GCOMPONENT(m_sliders, IGSlider, Slider);
private:
	std::vector<IGButton*> m_buttons;
	std::vector<IGTextBox*> m_textboxes;
	std::vector<IGGauge*> m_gauges;
	std::vector<IGSlider*> m_sliders;
};

}; // namespace