/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "InputManager.h"
#include "Engine.h"

namespace P3D{
/*
TODO: move to keyboard assigment game menu, don't put here
static const wchar* s_szTypeNames[] = {	"Key down", 
										"Key up", 
										"Mouse motion", 
										"Mouse button up", 
										"Mouse button down", 
										"Joystick motion",
										"Joystick button up",
										"Joystick button down",
										"PAD motion",
										"PAD button down",
										"PAD button up"};

static const wchar* s_szButtonNames[] = {""	*/

CInputManager::CInputManager()
{
	// mouse
	memset(m_mouseButtonsPrev, 0, 8);
	memset(m_mouseButtons, 0, 8);
	m_mousePos = Vec2i(0, 0);
	m_mouseChange = Vec2(0, 0);

	// keyboard
	memset(m_keyboardPrev, 0, KEY_LAST);
	memset(m_keyboard, 0, KEY_LAST);
}

CInputManager::~CInputManager()
{
	/*for (unsigned int i=0; i<m_assign.size(); i++)
		SAFE_DELETE_ARRAY(m_assign[i]);*/
//	m_assign.clear();
}

void CInputManager::ProcessNewEvents(tArray<sInputEvent> &events)
{
	memcpy(m_mouseButtonsPrev, m_mouseButtons, 8);
	memcpy(m_keyboardPrev, m_keyboard, KEY_LAST);
	for (UINT i=0; i<events.Size(); i++)
	{
		switch(events[i].event)
		{
		case P3D_INPUT_EVENT_KEY_DOWN:
			m_keyboard[events[i].id]=1;
			if(events[i].id == KEY_UP) CON(MSG_INFO, _W("up pressed!!"));
			break;
		case P3D_INPUT_EVENT_KEY_UP:
			m_keyboard[events[i].id]=0;
			break;
		case WE_MOUSE_DOWN:
			m_mouseButtons[events[i].id]=1;
			break;
		case WE_MOUSE_UP:
			m_mouseButtons[events[i].id]=0;
			break;
		}
	}

}

void CInputManager::Assign(const wchar* actionName, eWEventType type, int button){
	
	sKeyAssignment ka;
	ka.button = button;
	ka.type = type;
	wstrncpy(ka.szActionName, actionName, 15);

	m_assign.push_back(ka);
}

float CInputManager::GetState(const wchar* actionName){

	sKeyAssignment ka; bool found=false;

	for (unsigned int i=0; i<m_assign.size(); i++)
	{
		if (!wstrncmp(m_assign[i].szActionName, actionName, 15))
		{
			ka = m_assign[i];
			found=true;
			break;
		}

	}

	if (!found) return 0.0f;

	return -1.0f; // temporary
}

bool CInputManager::Load(const wchar* filename)
{
	BYTE* buff; ULONG len;

	FILE* fp = wfopen(filename, _W("rb"));
	if (!fp)
	{
		MessageBox(0, _W("Failed to load keys.ini!"), _W(""), 0);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	buff = new BYTE[len+1];
	fseek(fp, 0, SEEK_SET);
	fread(buff, 1, len, fp);
	buff[len]=0;

	m_assign.clear();
	
	// go through all lines
	wchar * ln;
	ln = wstrtok ((wchar*)buff,_W("\r\n"));
	while (ln != NULL)
	{
		size_t lnlen = wstrlen(ln);
		
		wchar actionName[32]; int type; int button;
		wsscanf(ln, _W("%s:%d:%d"), actionName, &type, &button);

		sKeyAssignment ka;
		wstrcpy(ka.szActionName, actionName);
		ka.type = (eWEventType)type;
		ka.button = button;

		m_assign.push_back(ka);

		ln = wstrtok (NULL, _W("\r\n")); // parse next line
	}

	delete[] buff;
	fclose(fp);
	return true;
}

void CInputManager::ProcessNewEvents(sWEvent *events, UINT num)
{
	memcpy(m_mouseButtonsPrev, m_mouseButtons, 8);
	memcpy(m_keyboardPrev, m_keyboard, KEY_LAST);
	for (UINT i=0; i<num; i++)
	{
		switch(events[i].type)
		{
		case WE_KEY_DOWN:
			//CON(MSG_INFO, "Key down: %d", events[i].button);
			m_keyboard[events[i].button]=1;
			break;
		case WE_KEY_UP:
			//CON(MSG_INFO, "Key up: %d", events[i].button);
			m_keyboard[events[i].button]=0;
			break;
		case WE_MOUSE_DOWN:
			//CON(MSG_INFO, "Mouse down: %d", events[i].button);
			m_mouseButtons[events[i].button]=1;
			break;
		case WE_MOUSE_UP:
			//m_mouseButtonsPrev[events[i].button]=m_mouseButtons[events[i].button];
			m_mouseButtons[events[i].button]=0;
			break;
		case WE_MOUSE_MOTION:
			//m_mouseChange.x = (int)events[i].x - (int)m_mousePos.x;
			//m_mouseChange.y = (int)events[i].y - (int)m_mousePos.y;
			m_mouseChange.x += events[i].xrel;
			m_mouseChange.y += events[i].yrel;
			m_mousePos.x = (int)events[i].x;
			m_mousePos.y = (int)events[i].y;
			break;
		}
	}

	// smooth mouse change
	m_mouseChange.x *= 0.5f;
	m_mouseChange.y *= 0.5f;
}

bool CInputManager::Save(const wchar* filename)
{
	wchar line[512];
	FILE* fp = wfopen(filename, _W("wb"));

	if (!fp) return false;

	for (unsigned int i=0; i<m_assign.size(); i++)
	{
		wsprintf(line, 511, _W("%s:%d:%d\r\n"), m_assign[i].szActionName, (int)m_assign[i].type, m_assign[i].button);
		fwrite(line, 1, wstrlen(line), fp);
	}

	fclose(fp);
	return true;
}


}; // namespace
