/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "IEngine.h"
#include "PhysEngine.h"

namespace P3D
{
	NxScene *g_pPhysScene = 0;
	IEngine* CPhysEngine::s_pEngine=NULL;

	//#define DEFAULT_GRAVITY NxVec3(0,-900.0f,0) // in cm/second2
	#define DEFAULT_GRAVITY NxVec3(0,-9.8f,0) // in m/second2

	REGISTER_MODULE_CLASS(CPhysEngine, _W("physics"), _W("physx"));

	
	static class _PXDebugOutput : public NxUserOutputStream
	{
	public:
		void  reportError (NxErrorCode code, const char *message, const char *file, int line){CON(MSG_ERR, _W("Phys: %s"), message);};
		NxAssertResponse  reportAssertViolation (const char *message, const char *file, int line){CON(MSG_ERR, _W("Phys: Assert Violation %s"), message); return NX_AR_CONTINUE;};
		void  print (const char *message){CON(MSG_INFO, _W("Phys: %s"), message);};
	} s_debugOutput;  


	CPhysEngine::~CPhysEngine()
	{
		// unload subclasses
		UNLOAD_SUBCLASS(PhysManager);

		if (m_pScene) m_pSDK->releaseScene(*m_pScene);
		NxReleasePhysicsSDK(m_pSDK);
		m_pSDK=NULL;
		CON(MSG_INFO, _W("PhysX released successfuly."));
	}

	bool CPhysEngine::Initialize(void* pEngine)
	{
		// memory leaks detection
#if defined(_DEBUG) && defined(_MSC_VER) && _MSC_VER >= 800
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		//_CrtSetBreakAlloc(76);
#endif

		s_pEngine = (IEngine*)pEngine;

		CON(MSG_INFO, _W("= Physics Engine initialization ="), );
		CON(MSG_INFO, _W("... loading nVidia PhysX version %d.%d.%d"), NX_SDK_VERSION_MAJOR, NX_SDK_VERSION_MINOR, NX_SDK_VERSION_BUGFIX);

		// create physX object
		NxSDKCreateError err;
		m_pSDK = NxCreatePhysicsSDKWithID(NX_PHYSICS_SDK_VERSION, "Reversity Studios", "Panther3D Engine", NULL,
			"PhysX rocks! ;) --- www.reversity.org", NULL, &s_debugOutput, NxPhysicsSDKDesc(), &err);

		if (!m_pSDK)
		{
			if (err == NXCE_PHYSX_NOT_FOUND)
				CON(MSG_ERR_BOX, _W("Can't find PhysX runtime! Please try to reinstall PhysX drivers..."));
			else if(err == NXCE_WRONG_VERSION)
				CON(MSG_ERR_BOX, _W("Wrong version of PhysX drivers! Please try to install recent PhysX drivers..."));
			else if(err == NXCE_DESCRIPTOR_INVALID)
				CON(MSG_ERR_BOX, _W("Wrong PhysX Descriptor! Please try to install recent PhysX drivers..."));
			else if(err == NXCE_CONNECTION_ERROR)
				CON(MSG_ERR_BOX, _W("A PhysX card was found, but there are problems when communicating with the card!"));
			else if(err == NXCE_RESET_ERROR)
				CON(MSG_ERR_BOX, _W("A PhysX card was found, but it did not reset (or initialize) properly!"));
			else if(err == NXCE_IN_USE_ERROR)
				CON(MSG_ERR_BOX, _W("A PhysX card was found, but it is already in use by another application!"));
			else if(err == NXCE_BUNDLE_ERROR)
				CON(MSG_ERR_BOX, _W("A PhysX card was found, but there are issues with loading the firmware!"));
			else
				CON(MSG_ERR_BOX, _W("Can't initialize PhysX runtime for unknown reason! That's bad... :( Try to reinstall drivers."));
			return false;
		}

		// basic settings
		m_pSDK->setParameter(NX_SKIN_WIDTH, 0.01f);
		m_pSDK->setParameter(NX_VISUALIZATION_SCALE, 1);

		NxSceneDesc sceneDesc;
		sceneDesc.gravity = DEFAULT_GRAVITY;
		sceneDesc.flags |= NX_SF_ENABLE_ACTIVETRANSFORMS;
		m_pScene = NULL;
		//if (m_pSDK->getNbPPUs() > 0)
		//NxHWVersion hwCheck = m_pSDK->getHWVersion();
		//if (!hwCheck == NX_HW_VERSION_NONE)
		if(false) //temporary hack
		{
			NxSceneDesc sceneDesc;
			sceneDesc.simType = NX_SIMULATION_HW;
			m_pScene = m_pSDK->createScene(sceneDesc);
			CON(MSG_INFO, _W(" ...using hardware physics simulation"));
			m_bHW = true;
		}

		if(!m_pScene)
		{ 
			sceneDesc.simType = NX_SIMULATION_SW; 
			m_pScene = m_pSDK->createScene(sceneDesc);  
			if(!m_pScene) 
			{
				CON(MSG_ERR, _W("Can't create PhysX scene!"));
				return false;
			}
			m_bHW = false;
			CON(MSG_INFO, _W(" ...using software physics simulation"));
		}

		// create default material, FIXME: is this necessary? Can't it be in some other place like material management?
		m_pDefaultMaterial = m_pScene->getMaterialFromIndex(0); 
		m_pDefaultMaterial->setRestitution(0.5);
		m_pDefaultMaterial->setStaticFriction(0.5);
		m_pDefaultMaterial->setDynamicFriction(0.5);

		// Connect to Visual Remote Debugger, TODO: this time only local host, but make possible to change it in settings
		m_pSDK->getFoundationSDK().getRemoteDebugger()->connect("127.0.0.1", 5425);

		// make first simulation step... to have some simulation results ;)
		SimulationBegin(0.0f);
		SimulationEnd();

		g_pPhysScene = m_pScene;

		return true;
	}

	void CPhysEngine::SimulationBegin(float deltaTime)
	{
		m_fDeltaTime = deltaTime;
		m_pScene->simulate(deltaTime);
		m_pScene->flushStream();

		// Get active bodies (which changet matrix from previous frame) and apply new matrix to Actor
		NxU32 nbTransforms = 0;   
		NxActiveTransform *activeTransforms = m_pScene->getActiveTransforms(nbTransforms);

		if(nbTransforms && activeTransforms)    
		{
			sNxActorUserData *udata = 0;

			for(NxU32 i = 0; i < nbTransforms; ++i)
			{
				// activeTransforms[i].userData can contain some binding to Actor
// 				udata = (sNxActorUserData*)activeTransforms[i].userData;
// 				udata->changed = true;
			}
		}
	}

	void CPhysEngine::SimulationEnd()
	{
		m_pScene->fetchResults(NX_RIGID_BODY_FINISHED, true);

		cPhysManager()->UpdateControllers();
	}

} // namespace