/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#include "SceneManager.h"
#include "Engine.h"

namespace P3D
{

	CSceneManager::CSceneManager()
	{
		m_pActiveCamera = m_pDefaultCamera = 0;

		m_pVS = m_pPS = 0;

		m_pParticleSystem = 0;
	}

	CSceneManager::~CSceneManager()
	{
		SAFE_DELETE(m_pDefaultCamera);
		for(unsigned int i=0; i<m_cameras.size(); i++) SAFE_DELETE(m_cameras[i]);
		m_cameras.clear();
		//for(unsigned int i=0; i<m_actors.size(); i++) SAFE_DELETE(m_actors[i]);
		//m_actors.clear();
		CON(MSG_DBG, _W("All the remaining cameras deleted..."));
		CON(MSG_DBG, _W("All the remaining actors deleted..."));
		SAFE_DELETE(m_pParticleSystem);
	}

	bool CSceneManager::Create()
	{
		m_pDefaultCamera = new CCamera;
		m_pDefaultCamera->Create(Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 0.0f), false);
		CON(MSG_DBG, _W("SceneManager created..."));

		sShaderDesc vsdesc;
		vsdesc.EntryFunction.SetEx(L"main");
		vsdesc.ShaderName.SetEx(L"temp_test_actor");
		vsdesc.ShaderFile.SetEx(L"vsActor.cg");
		vsdesc.ShaderType = SHADERTYPE_VERTEX_SHADER;
		//TODO: vertex desc ID

		sShaderDesc psdesc;
		psdesc.EntryFunction.SetEx(L"main");
		psdesc.ShaderName.SetEx(L"temp_test_actor");
		psdesc.ShaderFile.SetEx(L"psActor.cg");
		psdesc.ShaderType = SHADERTYPE_PIXEL_SHADER;

		//m_pVS = CEngine::instance()->mRenderer()->iShaderManager()->CreateShader(vsdesc);
		//m_pPS = CEngine::instance()->mRenderer()->iShaderManager()->CreateShader(psdesc);
		//m_texHandle = m_pPS->GetHandle("decal");
		//m_matrixHandle = m_pVS->GetHandle("modelViewProj");


		//m_pParticleSystem = new CParticleSystem;

		return true;
	}
	
	ICamera* CSceneManager::CreateCamera(P3D::Vec3 position, P3D::Vec3 lookat, bool FPS, float nearVP, float farVP, float FOV)
	{
		CCamera *camera = new CCamera;
		camera->Create(position, lookat, FPS, nearVP, farVP, FOV);
		m_cameras.push_back(camera);
		CON(MSG_DBG, _W("Camera created..."));
		return camera;
	}

	void CSceneManager::DeleteCamera(P3D::ICamera *camera)
	{
		if(camera == m_pActiveCamera) m_pActiveCamera = m_pDefaultCamera;
		for(unsigned int i=0; i<m_cameras.size(); i++) 
		{
			if(camera == m_cameras[i])
			{
				m_cameras.erase(m_cameras.begin()+i);
				CCamera *ccamera = (CCamera*)camera;
				delete ccamera;
				CON(MSG_DBG, _W("Camera deleted..."));
				break;
			}
		}
	}

	void CSceneManager::SetActiveCamera(P3D::ICamera *camera)
	{
		m_pActiveCamera = (CCamera*)camera;
	}

	/*IActor* CSceneManager::CreateActor(const wchar *filename, P3D::Vec3 position, P3D::Vec3 rotation, P3D::Vec3 scale)
	{
 		IMesh *mesh = CEngine::instance()->mRenderer()->iGraphicsManager()->LoadMesh(filename);
		if(!mesh) return 0;
		CActor *actor = new CActor;
		if(!actor->Create(mesh, position, rotation, scale)) return 0;
		m_actors.push_back(actor);
		return actor;
	}*/

	void CSceneManager::Update()
	{
		//render objects
// 		Matrix model;
// 		Matrix view;
// 		Matrix proj;
// 		model.SetIdentityMatrix();
// 		model.Translate(0.0f, 0.0f, 0.0f);
// 		view.LookAt(Vec3(0.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)); //this works
// 		proj.PerspectiveFov(45 * 0.0174532925f, 4.0f/3.0f, 1.0f, 1000.0f); //this works

		//Proc nejede DegToRad????

		static Matrix mviewproj;
		static Matrix mworld;

		m_pActiveCamera->Update();
		mviewproj = *m_pActiveCamera->GetViewProjMatrix();

		//Send view/proj matrix to the renderer.
		//CEngine::instance()->mRenderer()->iGraphicsDevice()->SetViewMatrix(*m_pActiveCamera->GetViewMatrix());
		//CEngine::instance()->mRenderer()->iGraphicsDevice()->SetProjectionMatrix(*m_pActiveCamera->GetProjectionMatrix());
		//CEngine::instance()->mRenderer()->iGraphicsDevice()->SetCameraPosition(m_pActiveCamera->GetPosition());
		
		/*for(unsigned int i=0; i<m_actors.size(); i++)
		{
			
			mworld = *m_actors[i]->GetMatrix();
			//m_pVS->SetHandleValueMatrix(m_matrixHandle, &( (*m_actors[i]->GetMatrix()) * (*m_pActiveCamera->GetViewProjMatrix()) ));
			//m_pVS->SetHandleValueMatrix(m_matrixHandle, &(model * view * proj));
			m_pVS->SetHandleValueMatrix(m_matrixHandle, &(mworld * mviewproj ));

			CEngine::instance()->mRenderer()->iGraphicsManager()->SetActiveVS(m_pVS);
			CEngine::instance()->mRenderer()->iGraphicsManager()->SetViewProjMatrix(mviewproj);

			for(unsigned int y=0; y<m_actors[i]->GetNumSubsets(); y++)
			{
				m_pPS->SetHandleValueTexture(m_texHandle, m_actors[i]->GetTexture(y));

				m_pVS->Use();
				m_pPS->Use();

				m_actors[i]->Render(y);
			}
		}*/
	}

}