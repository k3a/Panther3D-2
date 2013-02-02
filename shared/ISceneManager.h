/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once
#include "interface.h"
#include "ICamera.h"
//#include "IActor.h"

namespace P3D
{

	class ISceneManager : public IBaseInterface
	{
	public:
		///Creates a new Camera object
		virtual ICamera* CreateCamera(Vec3 position, Vec3 lookat, bool FPS, float nearVP = 1.0f, float farVP = 1000.0f, float FOV = 45.0f)=0;
		///Deletes the given Camera object
		virtual void DeleteCamera(ICamera *camera)=0;
		///Sets the Camera object which will be used during the rendering.
		virtual void SetActiveCamera(ICamera *camera)=0;
		///Creates a new Actor object
//		virtual IActor* CreateActor(const wchar *filename, P3D::Vec3 position, P3D::Vec3 rotation, P3D::Vec3 scale)=0;

		virtual void Update()=0;
	};

}