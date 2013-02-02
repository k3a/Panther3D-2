/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#pragma once

#include "IPhysEngine.h"
#include "PhysManager.h"
#include "IEngine.h"

#include <PhysX/NxPhysics.h>

namespace P3D
{

class CPhysEngine : public IPhysEngine
{
public:
	~CPhysEngine();
	bool Initialize(void* pEngine);
	void SimulationBegin(float deltatime);
	void SimulationEnd();

	// --- subclasses - this module singletons
	DECLARE_SUBCLASS(PhysManager);

	// --- external modules
	static inline IEngine* mEngine(){ return s_pEngine; };
private:
	NxPhysicsSDK* m_pSDK;
	NxScene* m_pScene;
	bool m_bHW;
	NxMaterial* m_pDefaultMaterial;
	float m_fDeltaTime;

	// --- external modules
	static IEngine* s_pEngine;
};

#define CON(type, text, ...) CPhysEngine::mEngine()->iConsole()->AddMsgEx(type, _W(__FUNCTION__), text, ##__VA_ARGS__)

} // namespace