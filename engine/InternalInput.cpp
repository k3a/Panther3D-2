/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 10/22/2009 File created - Petr
*/


#include "precompiled.h"

#include "Engine.h"
#include "InternalInput.h"

#include <iostream>
#include <vector>
#include <sstream>

namespace P3D
{

//---------------------------------------------------------------------------
	CInternalInput::CInternalInput()
	{
		m_pInputManager = NULL;
	}

//---------------------------------------------------------------------------
	CInternalInput::~CInternalInput()
	{
		if(m_pInputManager) OIS::InputManager::destroyInputSystem(m_pInputManager);
	}

//---------------------------------------------------------------------------
	bool CInternalInput::Init(void *winHandle)
	{
		OIS::ParamList pl;

		std::ostringstream wnd;
		wnd << (size_t)winHandle; //HWND
		//wnd << (size_t)GetActiveWindow(); //HWND
		pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

		//Default mode is foreground exclusive..but, we want to show mouse - so nonexclusive
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));

		//This never returns null.. it will raise an exception on errors
		m_pInputManager = OIS::InputManager::createInputSystem(pl);
		//m_pInputManager = OIS::InputManager::createInputSystem((size_t)winHandle);

		//Lets enable all addons that were compiled in:
		m_pInputManager->enableAddOnFactory(OIS::InputManager::AddOn_All);

		//Print debugging information
		unsigned int v = m_pInputManager->getVersionNumber();
		CON(MSG_INFO, _W("Using OIS Version: %d.%d.%d"), (v>>16), ((v>>8) & 0x000000FF), (v & 0x000000FF));
		CON(MSG_INFO, _W("...Release name: %s"), _A2W(m_pInputManager->getVersionName().data()) );
		CON(MSG_INFO, _W("...Manager: %s"), _A2W(m_pInputManager->inputSystemName().data()) );
		CON(MSG_INFO, _W("...Total JoySticks: %d"), m_pInputManager->getNumberOfDevices(OIS::OISJoyStick) );

		m_pKeyboard = (OIS::Keyboard*)m_pInputManager->createInputObject( OIS::OISKeyboard, true );
		m_pKeyboard->setEventCallback( this );

		m_pMouse = (OIS::Mouse*)m_pInputManager->createInputObject( OIS::OISMouse, true );
		m_pMouse->setEventCallback( this );

		return true;
	}

//---------------------------------------------------------------------------
	void CInternalInput::Update()
	{
		m_inputEvents.Clear();
		//all OIS callbacks are called in here
		m_pKeyboard->capture();
		m_pMouse->capture();
		
		m_eventMutex.Lock();
		m_finalInputEvents.Swap(m_inputEvents);
		m_eventMutex.Unlock();
	}

	void CInternalInput::SwapEvents(OUT tArray<sInputEvent> &inputEvents)
	{
		m_eventMutex.Lock();
		inputEvents.Swap(m_finalInputEvents);
		m_eventMutex.Unlock();
	}

//---------------------------------------------------------------------------
	bool CInternalInput::keyPressed(const OIS::KeyEvent &arg)
	{
		CON(MSG_INFO, _W("Key pressed: %d"), arg.key);

		sInputEvent ievent(P3D_INPUT_DEVICE_KEYBOARD, P3D_INPUT_EVENT_KEY_DOWN, arg.key, 0);
		m_inputEvents.AddEx(ievent);

		return true;
	}

//---------------------------------------------------------------------------
	bool CInternalInput::keyReleased(const OIS::KeyEvent &arg)
	{
		CON(MSG_INFO, _W("Key released: %d"), arg.key);

		sInputEvent ievent(P3D_INPUT_DEVICE_KEYBOARD, P3D_INPUT_EVENT_KEY_UP, arg.key, 0);
		m_inputEvents.AddEx(ievent);

		return true;
	}

//---------------------------------------------------------------------------
	bool CInternalInput::mouseMoved(const OIS::MouseEvent &arg)
	{
		//CON(MSG_INFO, _W("mouse motion: %d, %d, %d"), arg.state.X.rel, arg.state.Y.rel, arg.state.Z.rel);
		//horizontal motion
		if(arg.state.X.rel)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_MOUSE, P3D_INPUT_EVENT_AXIS_MOTION, 0, arg.state.X.rel);
			sInputEvent ievent2(P3D_INPUT_DEVICE_MOUSE, P3D_INPUT_EVENT_MOUSE_POSITION, 0, arg.state.X.abs);
			m_inputEvents.AddEx(ievent);
			m_inputEvents.AddEx(ievent2);
		}
		//vertical motion
		if(arg.state.Y.rel)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_MOUSE, P3D_INPUT_EVENT_AXIS_MOTION, 1, arg.state.Y.rel);
			sInputEvent ievent2(P3D_INPUT_DEVICE_MOUSE, P3D_INPUT_EVENT_MOUSE_POSITION, 1, arg.state.Y.abs);
			m_inputEvents.AddEx(ievent);
			m_inputEvents.AddEx(ievent2);
		}
		//scroll wheel
		if(arg.state.Z.rel)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_MOUSE, P3D_INPUT_EVENT_AXIS_MOTION, 2, arg.state.Z.rel);
			m_inputEvents.AddEx(ievent);
		}

		return true;
	}

//---------------------------------------------------------------------------
	bool CInternalInput::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		CON(MSG_INFO, _W("Mouse button pressed: %d"), id);

		sInputEvent ievent(P3D_INPUT_DEVICE_MOUSE, P3D_INPUT_EVENT_BUTTON_DOWN, id, 0);
		m_inputEvents.AddEx(ievent);

		return true;
	}

//---------------------------------------------------------------------------
	bool CInternalInput::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		CON(MSG_INFO, _W("Mouse button released: %d"), id);

		sInputEvent ievent(P3D_INPUT_DEVICE_MOUSE, P3D_INPUT_EVENT_BUTTON_UP, id, 0);
		m_inputEvents.AddEx(ievent);

		return true;
	}

//---------------------------------------------------------------------------
	bool CInternalInput::buttonPressed(const OIS::JoyStickEvent &arg, int button)
	{
		sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_BUTTON_DOWN, button, 0);
		m_inputEvents.AddEx(ievent);

		return true;
	}

//---------------------------------------------------------------------------
	bool CInternalInput::buttonReleased(const OIS::JoyStickEvent &arg, int button)
	{
		sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_BUTTON_UP, button, 0);
		m_inputEvents.AddEx(ievent);

		return true;
	}

//---------------------------------------------------------------------------
	bool CInternalInput::axisMoved(const OIS::JoyStickEvent &arg, int axis)
	{
		//Provide a little dead zone
		if( arg.state.mAxes[axis].abs > 2500 || arg.state.mAxes[axis].abs < -2500 )
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_AXIS_MOTION, axis, arg.state.mAxes[axis].abs);
			m_inputEvents.AddEx(ievent);
		}
		
		return true;
	}

//---------------------------------------------------------------------------
	bool CInternalInput::povMoved(const OIS::JoyStickEvent &arg, int pov)
	{
		if(arg.state.mPOV[pov].direction == OIS::Pov::Centered)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_POV_MOTION, pov, P3D_INPUT_POV_CENTERED);
			m_inputEvents.AddEx(ievent);
			return true;
		}

		if(arg.state.mPOV[pov].direction & OIS::Pov::NorthEast)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_POV_MOTION, pov, P3D_INPUT_POV_NORTHEAST);
			m_inputEvents.AddEx(ievent);
		}
		else if(arg.state.mPOV[pov].direction & OIS::Pov::NorthWest)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_POV_MOTION, pov, P3D_INPUT_POV_NORTHWEST);
			m_inputEvents.AddEx(ievent);
		}
		else if(arg.state.mPOV[pov].direction & OIS::Pov::SouthEast)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_POV_MOTION, pov, P3D_INPUT_POV_SOUTHEAST);
			m_inputEvents.AddEx(ievent);
		}
		else if(arg.state.mPOV[pov].direction & OIS::Pov::SouthWest)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_POV_MOTION, pov, P3D_INPUT_POV_SOUTHWEST);
			m_inputEvents.AddEx(ievent);
		}
		else if(arg.state.mPOV[pov].direction & OIS::Pov::North)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_POV_MOTION, pov, P3D_INPUT_POV_NORTH);
			m_inputEvents.AddEx(ievent);
		}
		else if(arg.state.mPOV[pov].direction & OIS::Pov::South)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_POV_MOTION, pov, P3D_INPUT_POV_SOUTH);
			m_inputEvents.AddEx(ievent);
		}
		else if(arg.state.mPOV[pov].direction & OIS::Pov::East)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_POV_MOTION, pov, P3D_INPUT_POV_EAST);
			m_inputEvents.AddEx(ievent);
		}
		else if(arg.state.mPOV[pov].direction & OIS::Pov::West)
		{
			sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_POV_MOTION, pov, P3D_INPUT_POV_WEST);
			m_inputEvents.AddEx(ievent);
		}
		
		return true;

	}

//---------------------------------------------------------------------------
	bool CInternalInput::sliderMoved(const OIS::JoyStickEvent &arg, int index)
	{
		sInputEvent ievent(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_SLIDER_MOTION, index*2, arg.state.mSliders[index].abX);
		sInputEvent ievent2(P3D_INPUT_DEVICE_JOYSTICK, P3D_INPUT_EVENT_SLIDER_MOTION, index*2+1, arg.state.mSliders[index].abY);
		m_inputEvents.AddEx(ievent);
		m_inputEvents.AddEx(ievent2);

		return true;
	}

}