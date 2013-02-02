/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once
#include "Camera.h"
//#include "Actor.h"
#include "IShader.h"
#include "ISceneManager.h"

#include "ParticleSystem.h"

namespace P3D
{

	class CSceneManager : public ISceneManager
	{
	public:
		CSceneManager();
		~CSceneManager();
		bool Create();
		ICamera* CreateCamera(Vec3 position, Vec3 lookat, bool FPS, float nearVP = 1.0f, float farVP = 1000.0f, float FOV = 45.0f);
		void DeleteCamera(ICamera *camera);
		void SetActiveCamera(ICamera *camera);
		//IActor* CreateActor(const wchar *filename, Vec3 position, Vec3 rotation, Vec3 scale);
		//IActor* CreateBoxActor(Vec3 size, Vec3 position, Vec3 rotation, Vec3 scale);
		//IActor* CreateSphereActor(float radius, float slices, float stacks, Vec3 position, Vec3 rotation, Vec3 scale);
		//IActor* CreateCylinderActor(Vec2 radiusxy, float height, unsigned int slices, unsigned int stacks, Vec3 position, Vec3 rotation, Vec3 scale);
		void Update();
	private:
		CCamera *m_pActiveCamera;
		CCamera *m_pDefaultCamera;
		std::vector<CCamera*> m_cameras;
		//std::vector<CActor*> m_actors;

		//for rendering
		IShader *m_pVS;
		IShader *m_pPS;
		ShaderHandle m_texHandle;
		ShaderHandle m_matrixHandle;

		//temporary
		CParticleSystem *m_pParticleSystem;
	};

}