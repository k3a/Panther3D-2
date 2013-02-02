/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "ShaderManager.h"
#include "Renderer.h"

namespace P3D
{

	
	CShaderManager::CShaderManager()
	{

	}

	CShaderManager::~CShaderManager()
	{
		
		for(unsigned int i=0; i<m_shaders.size(); i++)
		{
			delete m_shaders[i];
		}

	}

	IShader* CShaderManager::CreateShader(sShaderDesc &desc)
	{
		CShader *shader = new CShader;
		if(!shader->Create(desc))
		{
			delete shader;
			CON(MSG_ERR, _W("Loading shader %s failed!"), desc.ShaderName.Get());
			return 0;
		}
		m_shaders.push_back(shader);
		return shader;
	}

	void CShaderManager::DeleteShader(IShader *shader)
	{
		for(unsigned int i=0; i<m_shaders.size(); i++)
		{
			if(shader == m_shaders[i])
			{
				m_shaders.erase(m_shaders.begin()+i);
				delete shader;
			}
		}
	}

	void CShaderManager::OnLostDevice()
	{
		for(unsigned int i=0; i<m_shaders.size(); i++)
		{
			m_shaders[i]->OnLostDevice();
		}
	}

	void CShaderManager::OnResetDevice()
	{
		for(unsigned int i=0; i<m_shaders.size(); i++)
		{
			m_shaders[i]->OnResetDevice();
		}
	}

};
