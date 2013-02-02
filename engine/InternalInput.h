/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 10/20/2009 File created - Petr
*/


#pragma once
#include <ois/OIS.h>
#include "types.h"
#include "mutex.h"


namespace P3D
{

	enum eInputDevice
	{
		P3D_INPUT_DEVICE_KEYBOARD,
		P3D_INPUT_DEVICE_MOUSE,
		P3D_INPUT_DEVICE_JOYSTICK,
	};

	enum eInputEvent
	{
		P3D_INPUT_EVENT_KEY_DOWN,
		P3D_INPUT_EVENT_KEY_UP,
		P3D_INPUT_EVENT_MOUSE_POSITION,
		P3D_INPUT_EVENT_BUTTON_DOWN,
		P3D_INPUT_EVENT_BUTTON_UP,
		P3D_INPUT_EVENT_POV_MOTION,
		P3D_INPUT_EVENT_AXIS_MOTION,
		P3D_INPUT_EVENT_SLIDER_MOTION
	};

	enum eInputPOVValue
	{
		P3D_INPUT_POV_CENTERED,
		P3D_INPUT_POV_NORTH,
		P3D_INPUT_POV_SOUTH,
		P3D_INPUT_POV_EAST,
		P3D_INPUT_POV_WEST,
		P3D_INPUT_POV_NORTHEAST,
		P3D_INPUT_POV_NORTHWEST,
		P3D_INPUT_POV_SOUTHEAST,
		P3D_INPUT_POV_SOUTHWEST,
	};

	struct sInputEvent
	{
		sInputEvent(eInputDevice device, eInputEvent event, UINT id, int value)
		{
			this->device = device;
			this->event = event;
			this->id = id;
			this->value = value;
		}
		sInputEvent() {}

		///Identifies the device (keyboard, joystick, ...)
		eInputDevice device;
		///Identifies the event (key up, mouse motion, ...)
		eInputEvent event;
		///Can be key Id, mouse axis Id, ..
		UINT id;
		///Specifies a value the mouse has moved, etc..
		int value;
	};
	//device type, Id, key/button/axis, value

	//key pressed/released
	//mouse button pressed/released
	//mouse moved x/y/z, z = scroll wheel
	//joy button pressed/released
	//joy axis moved a..n
	//joy slider moved
	
	class CInternalInput : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
	{
	public:
		CInternalInput();
		~CInternalInput();
		bool Init(void *winHandle);
		///thread-safe
		void Update();
		///thread-safe
		void SwapEvents(OUT tArray<sInputEvent> &inputEvents);
	private:
		CMutex m_eventMutex;
		tArray<sInputEvent> m_inputEvents;
		tArray<sInputEvent> m_finalInputEvents;
		//OIS
		OIS::InputManager *m_pInputManager;
		OIS::Keyboard *m_pKeyboard;
		OIS::Mouse *m_pMouse;

		//OIS callbacks
		//keyboard
		bool keyPressed(const OIS::KeyEvent &arg);
		bool keyReleased(const OIS::KeyEvent &arg);
		//mouse
		bool mouseMoved(const OIS::MouseEvent &arg);
		bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		//joystick
		bool buttonPressed(const OIS::JoyStickEvent &arg, int button);
		bool buttonReleased(const OIS::JoyStickEvent &arg, int button);
		bool axisMoved(const OIS::JoyStickEvent &arg, int axis);
		bool povMoved(const OIS::JoyStickEvent &arg, int pov);
		bool sliderMoved(const OIS::JoyStickEvent &arg, int index);
	};

}//NS