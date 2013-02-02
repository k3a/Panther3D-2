/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once
#include "IConsole.h"
#include "Vector.h"
#include "Matrix.h"




namespace P3D
{

	class ICamera
	{
	public:
		/** Creates a new camera. */
		//virtual bool		Create(Vec3 position, Vec3 lookat, bool FPS, float nearVP = 1.0f, float farVP = 1000.0f, float FOV = 45.0f)=0;
		/** Sets the camera's world position. */
		virtual void		SetPosition(Vec3 position)=0;
		/** Sets the point the camera is looking at. This method has effect only if rotation is frozen. */
		virtual void		SetLookAt(Vec3 position)=0;
		/** Sets the vector of camera's direction. This method has effect only if rotation is frozen. */
		virtual void		SetLookVector(Vec3 vector)=0;
		virtual Vec3	GetPosition()=0;
		virtual Vec3	GetLookAt()=0;
		virtual Vec3	GetLookVector()=0;
		virtual void		FreezeMovement(bool freeze)=0;
		virtual void		FreezeRotation(bool freeze)=0;
		virtual bool		IsMovementFrozen()=0;
		virtual bool		IsRotationFrozen()=0;
		virtual void		SetFOV(float fov)=0;
		virtual float		GetFOV()=0;
		virtual float		getAspectRatio()=0;
		virtual const Matrix*	GetViewMatrix()=0;
		virtual const Matrix*	GetProjectionMatrix()=0;
		virtual const Matrix*	GetViewProjMatrix()=0;
		virtual void		SetMovementSpeed(float speed)=0;
		virtual void		SetRotationSpeed(float speed)=0;
		virtual float		GetMovementSpeed()=0;
		virtual float		GetRotationSpeed()=0;
	};

};
