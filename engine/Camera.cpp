/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "Camera.h"
#include "Engine.h"

namespace P3D
{

	bool CCamera::Create(P3D::Vec3 position, P3D::Vec3 lookat, bool FPS, float nearVP, float farVP, float FOV)
	{
		m_camPos = position;
		m_lookPos  = lookat;

		if(FPS)
		{
			m_movFrozen = false;
			m_rotFrozen = false;
		}
		else 
		{
			m_movFrozen = true;
			m_rotFrozen = true;
		}

		m_nVP = nearVP;
		m_fVP = farVP;
		m_FOV = FOV;
		m_aspRat = 4.0f/3.0f; //Engine must handle this variable. I don't know how yet.

		ComputeMatrixes();

		m_movSpeed = 5.0f;
		m_rotSpeed = 0.1f;

		m_up = Vec3(0.0f, 1.0f, 0.0f);
		m_right = Vec3(1.0f, 0.0f, 0.0f);
		m_look = Vec3(0.0f, 0.0f, 1.0f);

		return true;
	}

	void CCamera::ComputeMatrixes()
	{
		//FIXME:
/*		m_aspRat = (float)CEngine::instance()->mRenderer()->iGraphicsWindow()->GetSize().x / (float)CEngine::instance()->mRenderer()->iGraphicsWindow()->GetSize().y;
		m_matView.LookAt(m_camPos, m_lookPos, Vec3(0.0f, 1.0f, 0.0f));
		m_matProjection.PerspectiveFov(m_FOV * 0.0174532925f, m_aspRat, m_nVP, m_fVP);*/
	}

	void CCamera::SetPosition(Vec3 position)
	{
		m_camPos = position;
	}

	void CCamera::SetLookAt(Vec3 position)
	{
		m_lookPos = position;
	}

	void CCamera::SetLookVector(Vec3 vector)
	{
		m_lookPos = m_camPos + vector;
	}

	Vec3 CCamera::GetPosition()
	{
		return m_camPos;
	}

	Vec3 CCamera::GetLookAt()
	{
		return m_lookPos;
	}

	Vec3 CCamera::GetLookVector()
	{
		return m_lookPos - m_camPos;
	}

	void CCamera::FreezeMovement(bool freeze)
	{
		m_movFrozen = freeze;
	}

	void CCamera::FreezeRotation(bool freeze)
	{
		m_rotFrozen = freeze;
	}

	bool CCamera::IsMovementFrozen()
	{
		return m_movFrozen;
	}

	bool CCamera::IsRotationFrozen()
	{
		return m_rotFrozen;
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
		return m_aspRat;
	}

	const Matrix* CCamera::GetViewMatrix()
	{
		return &m_matView;
	}

	const Matrix* CCamera::GetProjectionMatrix()
	{
		return &m_matProjection;
	}

	void CCamera::SetMovementSpeed(float speed)
	{
		m_movSpeed = speed;
	}

	void CCamera::SetRotationSpeed(float speed)
	{
		m_rotSpeed = speed;
	}

	float CCamera::GetMovementSpeed()
	{
		return m_movSpeed;
	}

	float CCamera::GetRotationSpeed()
	{
		return m_rotSpeed;
	}

	void CCamera::Update()
	{
		if(!m_movFrozen)
		{
			///////////////////////////////////
			if(CEngine::cInputManager()->IsKeyDown(KEY_W)) m_camPos += m_look * m_movSpeed * CEngine::instance()->GetDeltaTime();
			if(CEngine::cInputManager()->IsKeyDown(KEY_S)) m_camPos -= m_look * m_movSpeed * CEngine::instance()->GetDeltaTime();
			if(CEngine::cInputManager()->IsKeyDown(KEY_D)) m_camPos += m_right * m_movSpeed * CEngine::instance()->GetDeltaTime();
			if(CEngine::cInputManager()->IsKeyDown(KEY_A)) m_camPos -= m_right * m_movSpeed * CEngine::instance()->GetDeltaTime();
			///////////////////////////////////
		}

		if(!m_rotFrozen)
		{
			

			Vec2 mouse;
			CEngine::cInputManager()->GetMousePosChange(mouse);
// 			wchar str[30];
// 			wsprintf(str, "%d", mouse.x);    
// 			CEngine::instance()->mRenderer()->iTextRenderer()->Print(str, Vec2i(100, 300), ColorRGBA(1.0f, 0.0f, 0.0f, 0.0f));
// 			wsprintf(str, "%d", mouse.y ); 
// 			CEngine::instance()->mRenderer()->iTextRenderer()->Print(str, Vec2i(100, 350), ColorRGBA(1.0f, 0.0f, 0.0f, 0.0f));
			mouse.x = mouse.x;
			mouse.y = mouse.y;
			float deltaY = mouse.y * 0.005f;
			float deltaX = mouse.x * 0.005f;
			//if(abs(deltaX) <= 0.00001f) deltaX = 0.0f;
			//if(abs(deltaY) <= 0.00001f) deltaY = 0.0f;

// 			wchar str[30];
// 			wsprintf(str, "%f", deltaY );    
// 			CEngine::instance()->mRenderer()->iTextRenderer()->Print(str, Vec2i(100, 300), ColorRGBA(1.0f, 0.0f, 0.0f, 0.0f));
// 			wsprintf(str, "%f", deltaX ); 
// 			CEngine::instance()->mRenderer()->iTextRenderer()->Print(str, Vec2i(100, 350), ColorRGBA(1.0f, 0.0f, 0.0f, 0.0f));
			Matrix r;
			
			r.SetIdentityMatrix();
			r.Rotate(m_right, deltaY);
			r.TransformVector(m_look);
			r.TransformVector(m_up);

			r.SetIdentityMatrix(); //maybe
			r.RotateY(deltaX);
			r.TransformVector(m_look);
			r.TransformVector(m_right);

			m_look.Normalize();
			m_up = m_look.Cross(m_right);
			m_up.Normalize();
			m_right = m_up.Cross(m_look);
			m_right.Normalize();

			float x = -m_camPos.Dot(m_right);
			float y = -m_camPos.Dot(m_up);
			float z = -m_camPos.Dot(m_look);

			m_matView.m[0][0] = m_right.x;
			m_matView.m[1][0] = m_right.y;
			m_matView.m[2][0] = m_right.z;
			m_matView.m[3][0] = x;

			m_matView.m[0][1] = m_up.x;
			m_matView.m[1][1] = m_up.y;
			m_matView.m[2][1] = m_up.z;
			m_matView.m[3][1] = y;

			m_matView.m[0][2] = m_look.x;
			m_matView.m[1][2] = m_look.y;
			m_matView.m[2][2] = m_look.z;
			m_matView.m[3][2] = z;

			m_matView.m[0][3] = 0.0f;
			m_matView.m[1][3] = 0.0f;
			m_matView.m[2][3] = 0.0f;
			m_matView.m[3][3] = 1.0f;

//			m_aspRat = (float)CEngine::instance()->mRenderer()->iGraphicsWindow()->GetSize().x / (float)CEngine::instance()->mRenderer()->iGraphicsWindow()->GetSize().y;
			m_matProjection.PerspectiveFov(m_FOV * 0.0174532925f, m_aspRat, m_nVP, m_fVP);

			return;
		}


		ComputeMatrixes();
	}

	const Matrix* CCamera::GetViewProjMatrix()
	{
		return &(m_matView * m_matProjection);
	}


};
