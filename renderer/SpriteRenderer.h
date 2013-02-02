/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>
#include "ISpriteRenderer.h"
#include "ShaderManager.h"

#include "IGraphicsDevice.h"

namespace P3D
{
	
	struct SpriteVertex
	{
		Vec3 pos;
		//Vec3 dir;
		//float initTime;
	};

	

	struct SpriteInfo
	{
		Vec3 pos;
		ColorRGBA color;
		float size;
	};

	enum eSpriteTechnique
	{
		SPT_POINTSPRITE = 0,
		SPT_POINTSPRITEINSTANCED,
		SPT_BILLBOARD
	};
	
	class CSpriteRenderer : public ISpriteRenderer
	{
	public:
		CSpriteRenderer();
		~CSpriteRenderer();
		bool Create();
		void Render();
		//void SetViewProjMatrix(Matrix &VP);
		const Matrix* GetMatrix() { return &m_matrix; };

	private:
		eSpriteTechnique m_technique;
		CVertexBuffer *m_pVB;
		CIndexBuffer *m_pIB;
		Matrix m_matrix;
		//Matrix m_VPmatrix;
		std::vector<SpriteVertex> m_particles;
		std::vector<SpriteVertex*> m_pDeadParticles;
		std::vector<SpriteVertex*> m_pAliveParticles;
		float m_time;
		unsigned int m_numAliveParticles;
		unsigned int m_maxNumParticles;
		SpriteInfo m_spriteInfos[1000];

		CShader* m_pVS;
		CShader* m_pPS;
		ShaderHandle m_matrixHandle;
		ShaderHandle m_arrayHandle;
	};
}