/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#include "IInputManager.h"
#include <vector>
#include "InternalInput.h"

namespace P3D{

struct sKeyAssignment
{
	wchar szActionName[16];
	eWEventType type;
	int button;
};

class CInputManager : public IInputManager
{
public:
	CInputManager();
	~CInputManager();

	///New way of processing input events
	void ProcessNewEvents(tArray<sInputEvent> &events);

	void Assign(const wchar* actionName, eWEventType type, int button);
	float GetState(const wchar* actionName);

	void ProcessNewEvents(sWEvent *events, UINT num);
	void GetMousePos(Vec2i &pos)const{pos = m_mousePos;};
	void GetMousePosChange(Vec2 &change)const{change = m_mouseChange;};
	bool IsMouseButtonDown(eButton button)const{ return m_mouseButtons[button]==1 ? true : false; };
	bool IsMouseButtonDownFirstTime(eButton button)const{ return (m_mouseButtons[button]==1 && m_mouseButtonsPrev[button]==0) ? true : false; };
	bool IsMouseButtonUp(eButton button)const{ return m_mouseButtons[button]==0 ? true : false; };
	bool IsMouseButtonUpFirstTime(eButton button)const{ return (m_mouseButtons[button]==0 && m_mouseButtonsPrev[button]==1) ? true : false; };
	bool IsKeyDown(eKey key)const{ return m_keyboard[key]==1 ? true : false; };
	bool IsKeyDownFirstTime(eKey key)const{ return (m_keyboard[key]==1 && m_keyboardPrev[key]==0) ? true : false; };
	bool IsKeyUp(eKey key)const{ return m_keyboard[key]==0 ? true : false; };
	bool IsKeyUpFirstTime(eKey key)const{ return (m_keyboard[key]==0 && m_keyboardPrev[key]==1) ? true : false; };

	bool Load(const wchar* filename);
	bool Save(const wchar* filename);
private:
	vector<sKeyAssignment> m_assign;
	// mouse
	Vec2i m_mousePos;
	Vec2 m_mouseChange;
	BYTE m_mouseButtons[8];
	BYTE m_mouseButtonsPrev[8];
	// keyboard
	BYTE m_keyboard[KEY_LAST];
	BYTE m_keyboardPrev[KEY_LAST];
};

}; // namespace
