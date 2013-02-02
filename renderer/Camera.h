/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once
#include "ICamera.h"

namespace P3D
{

	class CCamera : public ICamera
	{
	public:
					CCamera();
					~CCamera();
		bool		Create(Vec3 position, Vec3 lookat, bool FPS, float nearVP = 1.0f, float farVP = 1000.0f, float FOV = 45.0f);
		void		SetPosition(Vec3 position);
		void		SetLookAt(Vec3 position);
		void		SetLookVector(Vec3 vector);
		Vec3	GetPosition();
		Vec3	GetLookAt();
		Vec3	GetLookVector();
		void		FreezeMovement(bool freeze);
		void		FreezeRotation(bool freeze);
		bool		IsMovementFrozen();
		bool		IsRotationFrozen();
		void		SetFOV(float fov);
		float		GetFOV();
		float		getAspectRatio();
		Matrix		GetViewMatrix();
		Matrix		GetProjectionMatrix();
		void		SetMovementSpeed(float speed);
		void		SetRotationSpeed(float speed);
		float		GetMovementSpeed();
		float		GetRotationSpeed();
	private:
		Matrix		m_matView;
		Matrix		m_matProjection;
		Vec3 m_CamPos;
		Vec3 m_LookPos;
		float		m_NearVP;
		float		m_FarVP;
		float		m_FOV;
		float		m_AspectRatio;
		bool		m_MovementFrozen;
		bool		m_RotationFrozen;
		float		m_MovementSpeed;
		float		m_RotationSpeed;
	};

};
