/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

#pragma once

#include "types.h"
#include "IRenderCanvas.h"

namespace P3D
{

enum eWEventType
{
	WE_KEY_DOWN=0,
	WE_KEY_UP,
	WE_MOUSE_MOTION,
	WE_MOUSE_UP,
	WE_MOUSE_DOWN,
	WE_JOY_MOTION,
	WE_JOY_DOWN,
	WE_JOY_UP,
	WE_PAD_MOTION,
	WE_PAD_DOWN,
	WE_PAD_UP,
	WE_RESIZE,
	WE_QUIT
};

// General - purpose button ID's.
// These can be mouse buttons, joystick buttons or PAD buttons
enum eButton{
	BUTTON_1 = 1,
	BUTTON_2 = 2,
	BUTTON_3 = 3,
	BUTTON_4 = 4,
	BUTTON_5 = 5,
	BUTTON_6 = 6,
	BUTTON_7 = 7,
	BUTTON_8 = 8,
	BUTTON_9 = 9,
	BUTTON_10 = 10,
	BUTTON_11 = 11,
	BUTTON_12 = 12,
	BUTTON_13 = 13,
	BUTTON_14 = 14,
	BUTTON_15 = 15,
	BUTTON_16 = 16
};
 
#define MOUSE_LEFT BUTTON_1
#define MOUSE_MIDDLE BUTTON_2
#define MOUSE_RIGHT BUTTON_3

/// Keyboard KEY ID's based on OIS's keyboard map defines
enum eKey{
	KEY_UNASSIGNED  = 0,
	KEY_ESCAPE      = 1,
	KEY_1           = 2,
	KEY_2           = 3,
	KEY_3           = 4,
	KEY_4           = 5,
	KEY_5           = 6,
	KEY_6           = 7,
	KEY_7           = 8,
	KEY_8           = 9,
	KEY_9           = 10,
	KEY_0           = 11,
	KEY_MINUS       = 12,    // - on main keyboard
	KEY_EQUALS      = 13,
	KEY_BACK        = 14,    // backspace
	KEY_TAB         = 15,
	KEY_Q           = 16,
	KEY_W           = 17,
	KEY_E           = 18,
	KEY_R           = 19,
	KEY_T           = 20,
	KEY_Y           = 21,
	KEY_U           = 22,
	KEY_I           = 23,
	KEY_O           = 24,
	KEY_P           = 25,
	KEY_LBRACKET    = 26,
	KEY_RBRACKET    = 27,
	KEY_RETURN      = 28,    // Enter on main keyboard
	KEY_LCONTROL    = 29,
	KEY_A           = 30,
	KEY_S           = 31,
	KEY_D           = 32,
	KEY_F           = 33,
	KEY_G           = 34,
	KEY_H           = 35,
	KEY_J           = 36,
	KEY_K           = 37,
	KEY_L           = 38,
	KEY_SEMICOLON   = 39,
	KEY_APOSTROPHE  = 40,
	KEY_GRAVE       = 41,    // accent
	KEY_LSHIFT      = 42,
	KEY_BACKSLASH   = 43,
	KEY_Z           = 44,
	KEY_X           = 45,
	KEY_C           = 46,
	KEY_V           = 47,
	KEY_B           = 48,
	KEY_N           = 49,
	KEY_M           = 50,
	KEY_COMMA       = 51,
	KEY_PERIOD      = 52,    // . on main keyboard
	KEY_SLASH       = 53,    // / on main keyboard
	KEY_RSHIFT      = 54,
	KEY_MULTIPLY    = 55,    // * on numeric keypad
	KEY_LMENU       = 56,    // left Alt
	KEY_SPACE       = 57,
	KEY_CAPITAL     = 58,
	KEY_F1          = 59,
	KEY_F2          = 60,
	KEY_F3          = 61,
	KEY_F4          = 62,
	KEY_F5          = 63,
	KEY_F6          = 64,
	KEY_F7          = 65,
	KEY_F8          = 66,
	KEY_F9          = 67,
	KEY_F10         = 68,
	KEY_NUMLOCK     = 69,
	KEY_SCROLL      = 70,    // Scroll Lock
	KEY_NUMPAD7     = 71,
	KEY_NUMPAD8     = 72,
	KEY_NUMPAD9     = 73,
	KEY_SUBTRACT    = 74,    // - on numeric keypad
	KEY_NUMPAD4     = 75,
	KEY_NUMPAD5     = 76,
	KEY_NUMPAD6     = 77,
	KEY_ADD         = 78,    // + on numeric keypad
	KEY_NUMPAD1     = 79,
	KEY_NUMPAD2     = 80,
	KEY_NUMPAD3     = 81,
	KEY_NUMPAD0     = 82,
	KEY_DECIMAL     = 83,    // . on numeric keypad
	//KEY_OEM_102     = 84,    // < > | on UK/Germany keyboards
	KEY_F11         = 87,
	KEY_F12         = 88,
	KEY_F13         = 89,    //                     (NEC PC98)
	KEY_F14         = 90,    //                     (NEC PC98)
	KEY_F15         = 91,    //                     (NEC PC98)
	//KEY_KANA        = 90,    // (Japanese keyboard)
	//KEY_ABNT_C1     = 91,    // / ? on Portugese (Brazilian) keyboards
	//KEY_CONVERT     = 92,    // (Japanese keyboard)
	//KEY_NOCONVERT   = 93,    // (Japanese keyboard)
	//KEY_YEN         = 94,    // (Japanese keyboard)
	//KEY_ABNT_C2     = 95,    // Numpad . on Portugese (Brazilian) keyboards
	//KEY_NUMPADEQUALS= 96,    // = on numeric keypad (NEC PC98)
	//KEY_PREVTRACK   = 97,    // Previous Track (KEY_CIRCUMFLEX on Japanese keyboard)
	//KEY_AT          = 98,    //                     (NEC PC98)
	//KEY_COLON       = 99,    //                     (NEC PC98)
	//KEY_UNDERLINE   = 100,    //                     (NEC PC98)
	//KEY_KANJI       = 101,    // (Japanese keyboard)
	//KEY_STOP        = 102,    //                     (NEC PC98)
	//KEY_AX          = 103,    //                     (Japan AX)
	//KEY_UNLABELED   = 104,    //                        (J3100)
	//KEY_NEXTTRACK   = 105,    // Next Track
	KEY_NUMPADENTER = 156,    // Enter on numeric keypad
	KEY_RCONTROL    = 157,
	//KEY_MUTE        = 108,    // Mute
	//KEY_CALCULATOR  = 109,    // Calculator
	//KEY_PLAYPAUSE   = 110,    // Play / Pause
	//KEY_MEDIASTOP   = 111,    // Media Stop
	//KEY_VOLUMEDOWN  = 112,    // Volume -
	//KEY_VOLUMEUP    = 113,    // Volume +
	//KEY_WEBHOME     = 114,    // Web home
	//KEY_NUMPADCOMMA = 115,    // , on numeric keypad (NEC PC98)
	KEY_DIVIDE      = 181,    // / on numeric keypad
	//KEY_SYSRQ       = 117,
	KEY_RMENU       = 184,    // right Alt
	//KEY_PAUSE       = 119,    // Pause
	KEY_HOME        = 199,    // Home on arrow keypad
	KEY_UP          = 200,    // UpArrow on arrow keypad
	KEY_PGUP        = 201,    // PgUp on arrow keypad
	KEY_LEFT        = 203,    // LeftArrow on arrow keypad
	KEY_RIGHT       = 205,    // RightArrow on arrow keypad
	KEY_END         = 207,    // End on arrow keypad
	KEY_DOWN        = 208,    // DownArrow on arrow keypad
	KEY_PGDOWN      = 209,    // PgDn on arrow keypad
	KEY_INSERT      = 210,    // Insert on arrow keypad
	KEY_DELETE      = 211,    // Delete on arrow keypad
	KEY_LWIN        = 219,    // Left Windows key
	KEY_RWIN        = 220,    // Right Windows key
	//KEY_APPS        = 132,    // AppMenu key
	//KEY_POWER       = 133,    // System Power
	//KEY_SLEEP       = 134,    // System Sleep
	//KEY_WAKE        = 135,    // System Wake
	//KEY_WEBSEARCH   = 136,    // Web Search
	//KEY_WEBFAVORITES= 137,    // Web Favorites
	//KEY_WEBREFRESH  = 138,    // Web Refresh
	//KEY_WEBSTOP     = 139,    // Web Stop
	//KEY_WEBFORWARD  = 140,    // Web Forward
	//KEY_WEBBACK     = 150,    // Web Back
	//KEY_MYCOMPUTER  = 160,    // My Computer
	//KEY_MAIL        = 161,    // Mail
	//KEY_MEDIASELECT = 162,    // Media Select
	KEY_LAST        = 221 //added by am!go
};

/// Struct describing single event (ie. key down)
struct sWEvent
{
	eWEventType type;
	///Byte character translated from key
	wchar character;
	/// key, button or hat or ball or thumb depending on input type
	/** In case of keyboard key it will be keyboard hw scan code */
	int button;
	/// x coordinate or coordinate change depending on input type
	float x;
	/// y coordinate or coordinate change depending on input type
	float y;

	float xrel;
	float yrel;
};

///Application window - place to render to.
/** Should be native window in GUI OS or whole screen on consoles. */
class IWindow
{
public:
	virtual ~IWindow() {};

	///Get window handle. Will be HWND on Windows, X Handle on X11, 0 where handle doesn't exist
	virtual void* GetHandle()const=0;

	///Determines whether is this window native (not using SDL or something platform-independent)
	virtual bool IsNativeWindow()const=0;

	///Can we go from this window to fullscreen?
	virtual bool CanGoFullscreen()const=0;

	//Get new events which hapened since last call
	virtual sWEvent* GetEvents(OUT unsigned int &numEvents)=0;

	///Mouse position within this window
	virtual Vec2i GetMousePos()const=0;

	///Returns the actual size in pixels
	virtual Vec2i GetSize()const=0;

	///Returns the number of bytes per pixels
	virtual unsigned int GetBPP()const=0;

	///Changes the size of a widnow
	virtual void SetSize(Vec2i wsize, bool fullscreen=false)=0;

	///Hide window.
	/** It isn't needed to implement this in window class. It is useful only for default renderer window. */
	virtual void HideWindow(bool hide=true){};

	///Sets mouse capture mode
	/** In case of menu mouse mode is enabled, mouse is shown and can be moved outside window.
		Otherwise mouse is kept in the center of the window which is useful while in game. */
	virtual void SetMenuMode(bool bMenu)=0;
};

}; // namespace
