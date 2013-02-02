/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "interface.h"
#include "IWindow.h" // key definitions ;) KEY_*

namespace P3D{

class IInputManager /*: public IBaseInterface*/
{
public:
	/// Assign event type and button to desired action
	/** For example you can assign MOVE_FORWARD to WE_KEY_DOWN with button KEY_W. */
	virtual void Assign(const wchar* actionName, eWEventType type, int button)=0;
	/// Retrieve state of previously assigned action for current frame.
	/** It can be 1.0f or 0.0f for boolean actions (key down) or floating point value for axis or mouse change. */
	virtual float GetState(const wchar* actionName)=0;

	virtual void ProcessNewEvents(sWEvent *events, UINT num)=0;
	virtual void GetMousePos(Vec2i &pos)const=0;
	virtual void GetMousePosChange(Vec2 &change)const=0;
	virtual bool IsMouseButtonDown(eButton button)const=0;
	virtual bool IsMouseButtonDownFirstTime(eButton button)const=0;
	virtual bool IsMouseButtonUp(eButton button)const=0;
	virtual bool IsMouseButtonUpFirstTime(eButton button)const=0;
	virtual bool IsKeyDown(eKey key)const=0;
	virtual bool IsKeyDownFirstTime(eKey key)const=0;
	virtual bool IsKeyUp(eKey key)const=0;
	virtual bool IsKeyUpFirstTime(eKey key)const=0;


	/// Load assigment from file
	virtual bool Load(const wchar* filename)=0;
	/// Save assigment to file
	virtual bool Save(const wchar* filename)=0;
};

}; // namespace
