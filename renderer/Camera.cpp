/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "Camera.h"

namespace P3D
{

	CCamera::CCamera()
	{

	}

	CCamera::~CCamera()
	{

	}

	bool CCamera::Create(Vec3 position, Vec3 lookat, bool FPS, float nearVP, float farVP, float FOV)
	{
		//Is the camera FPS type? If it isn't lets freeze it's movement a rotation.
		if(FPS)
		{
			m_MovementFrozen = false;
			m_RotationFrozen = false;
		}
		else
		{
			m_MovementFrozen = true;
			m_RotationFrozen = true;
		}
		//Copies position and lookat info into global variables
		m_CamPos  = position;
		m_LookPos = lookat;
		//Copies remaining useful info into global variables
		m_FOV			= FOV;
		m_NearVP		= nearVP;
		m_FarVP			= farVP;
		m_AspectRatio	= 4.0f/3.0f; //THIS VALUE HAS TO BE GIVEN BY APPLICATION!!!
		//Creates matrices
		//ComputeMatrices();

		return true;
	}

	void CCamera::FreezeMovement(bool freeze)
	{
		m_MovementFrozen = freeze;
	}

	void CCamera::FreezeRotation(bool freeze)
	{
		m_RotationFrozen = freeze;
	}

	void CCamera::SetFOV(float fov)
	{
		m_FOV = fov;
	}

	float CCamera::GetFOV()
	{
		return m_FOV;
	}

	float CCamera::getAspectRatio()
	{
		return m_AspectRatio;
	}

	bool CCamera::IsMovementFrozen()
	{
		return m_MovementFrozen;
	}

	bool CCamera::IsRotationFrozen()
	{
		return m_RotationFrozen;
	}

	Matrix CCamera::GetViewMatrix()
	{
		return m_matView;
	}

	Matrix CCamera::GetProjectionMatrix()
	{
		return m_matProjection;
	}

	void CCamera::SetMovementSpeed(float speed)
	{
		m_MovementSpeed = speed;
	}

	void CCamera::SetRotationSpeed(float speed)
	{
		m_RotationSpeed = speed;
	}

	float CCamera::GetMovementSpeed()
	{
		return m_MovementSpeed;
	}

	float CCamera::GetRotationSpeed()
	{
		return m_RotationSpeed;
	}

	Vec3 CCamera::GetLookAt()
	{
		return m_LookPos;
	}

	Vec3 CCamera::GetLookVector()
	{
		Vec3 v = m_LookPos - m_CamPos;
		v.Normalize();
		return v;
	}

	Vec3 CCamera::GetPosition()
	{
		return m_CamPos;
	}

	void CCamera::SetPosition(Vec3 position)
	{
		m_CamPos = position;
	}

	void CCamera::SetLookAt(Vec3 position)
	{
		m_LookPos = position;
	}

	void CCamera::SetLookVector(Vec3 vector)
	{
		m_LookPos = m_CamPos + vector;
	}

};
