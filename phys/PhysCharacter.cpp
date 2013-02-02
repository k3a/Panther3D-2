/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "PhysCharacter.h"
#include "CharacterHitReport.h"



namespace P3D
{

	extern NxControllerManager *g_pControllerManager;
	extern NxScene *g_pPhysScene;


	


	CPhysCharacter::CPhysCharacter()
	{
		m_pController = 0;
	}

	CPhysCharacter::~CPhysCharacter()
	{
		if(m_pController) g_pControllerManager->releaseController(*m_pController);
		m_pController = 0;
	}

	bool CPhysCharacter::Create(P3D::sPhysControllerDesc desc, P3D::Vec3 position)
	{
		if(!g_pControllerManager) return false;

		NxCapsuleControllerDesc nxdesc;
		nxdesc.height = desc.height;
		nxdesc.radius = desc.radius;
		nxdesc.stepOffset = desc.stepOffset;
		nxdesc.slopeLimit = cosf(NxMath::degToRad(desc.slopeLimit));
		nxdesc.callback = &gControllerHitReport;
		nxdesc.skinWidth = desc.skinWidth;
		nxdesc.upDirection	= NX_Y;
		m_pController = g_pControllerManager->createController(g_pPhysScene, nxdesc);
		if(!m_pController) return false;
		m_pController->setPosition(NxExtendedVec3(position.x, position.y, position.z));
		return true;
	}

	void CPhysCharacter::SetPosition(P3D::Vec3 position)
	{
		m_pController->setPosition(NxExtendedVec3(position.x, position.y, position.z));
	}

	Vec3 CPhysCharacter::Move(P3D::Vec3 displacement)
	{
		NxU32 collisionFlags = 0;
		m_pController->move(NxVec3(displacement.x, displacement.y, displacement.z), -1, 0.0001f, collisionFlags, 1.0f);
		static NxExtendedVec3 pos;
		pos = m_pController->getFilteredPosition();
		return Vec3((float)pos.x, (float)pos.y, (float)pos.z);
	}

}