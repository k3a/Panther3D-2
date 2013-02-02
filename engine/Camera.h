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
		bool		Create(Vec3 position, Vec3 lookat, bool FPS, float nearVP = 1.0f, float farVP = 1000.0f, float FOV = 45.0f);
		void		SetPosition(Vec3 position);
		void		SetLookAt(Vec3 position);
		void		SetLookVector(Vec3 vector);
		Vec3		GetPosition();
		Vec3		GetLookAt();
		Vec3		GetLookVector();
		void		FreezeMovement(bool freeze);
		void		FreezeRotation(bool freeze);
		bool		IsMovementFrozen();
		bool		IsRotationFrozen();
		void		SetFOV(float fov);
		float		GetFOV();
		float		getAspectRatio();
		const Matrix*	GetViewMatrix();
		const Matrix*	GetProjectionMatrix();
		const Matrix*	GetViewProjMatrix();
		void		SetMovementSpeed(float speed);
		void		SetRotationSpeed(float speed);
		float		GetMovementSpeed();
		float		GetRotationSpeed();
		//Not in the interface
		void		SetAspectRatio(float asprat);
		void		Update();
	private:
		void		ComputeMatrixes();
		Matrix		m_matView;	//View matrix
		Matrix		m_matProjection;	//Projection matrix
		float		m_rotSpeed;	//Rotation speed (for FPS mode)
		float		m_movSpeed;	//Movement speed (for FPS mode)
		float		m_FOV;	//Field of view
		float		m_aspRat; //Aspect Ratio
		bool		m_rotFrozen;	//Is rotation frozen? (for FPS mode)
		bool		m_movFrozen;	//Is movement frozen? (for FPS mode)
		float		m_nVP;	//Near view plane
		float		m_fVP;	//Far view plane
		Vec3		m_camPos;	//Camera's position
		Vec3		m_lookPos;	//What point is the camera looking at
		Vec3		m_up;
		Vec3		m_right;
		Vec3		m_look;
	};

};
