/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 09/08/2009 File created - Petr
*/

#include "precompiled.h"

#include "FrameBuffer.h"
#include "Renderer.h"

namespace P3D
{

//---------------------------------------------------------------------------
	CFrameBuffer::CFrameBuffer()
	{
		m_numGBUnits = 0;
		m_pGBUnits = NULL;
	}
//---------------------------------------------------------------------------
	CFrameBuffer::~CFrameBuffer()
	{
		SAFE_DELETE_ARRAY(m_pGBUnits);
	}
//---------------------------------------------------------------------------
	bool CFrameBuffer::Create(Vec2i rtSize)
	{
		return Recreate(rtSize);
	}
//---------------------------------------------------------------------------
	sFrameBufferUnit* CFrameBuffer::GetFBUnitByUsage(eFrameBufferUnitUsage usage)
	{
		for(UINT i=0; i<m_numGBUnits; i++)
			if( m_pGBUnits[i].usage == usage )return  &m_pGBUnits[i];

		return NULL;
	}
//---------------------------------------------------------------------------
	bool CFrameBuffer::Resize(P3D::Vec2i newSize)
	{
		if( !Recreate(newSize) ) {
			CON(MSG_ERR, _W("Unable to resize a GBuffer"));
			return false;
		}
		else {
			CON(MSG_DBG, _W("GBuffer resized to %fx%f"), newSize.x, newSize.y);
			return true;
		}
	}
//---------------------------------------------------------------------------
	bool CFrameBuffer::Recreate(P3D::Vec2i newSize)
	{
		SAFE_DELETE_ARRAY(m_pGBUnits);
		
		sTextureDesc desc;
		desc.ArraySize = 1;
		desc.AutoGenerateMips = false;
		desc.BindFlag = P3D_TEXBINDFLAG_RENDER_TARGET;
		desc.CPUAccess = P3D_CPUACCESS_NONE;
		desc.MipLevels = 1;
		desc.ResourceUsage = P3D_RESOURCEUSAGE_DEFAULT;
		desc.Width = newSize.x;
		desc.Height = newSize.y;

		//TODO: check for available formats and eventually provide some fallback

		// Initialize Diffuse Render Target
		desc.Format = P3D_FORMAT_X8R8G8B8_UNORM;
		CTexture *diffuse = new CTexture;
		if( !diffuse->CreateTexture(desc, NULL, 0) ) 
		{
			CON(MSG_ERR_FATAL, _W("Failed to create diffuse RT!") );
			SAFE_DELETE(diffuse);
			return false;
		}
		diffuse->SetFilename(_W("GBuffer-diffuse"));

		// Initialize Normal Render Target
		desc.Format = P3D_FORMAT_R16G16_SNORM;
		CTexture *normal = new CTexture;
		if( !normal->CreateTexture(desc, NULL, 0) ) 
		{
			CON(MSG_ERR_FATAL, _W("Failed to create normal RT!") );
			SAFE_DELETE(diffuse);
			SAFE_DELETE(normal);
			return false;
		}
		normal->SetFilename(_W("GBuffer-normal"));

		// Initialize Depth Render Target
		desc.Format = P3D_FORMAT_R32_FLOAT;
		CTexture *depth = new CTexture;
		if( !depth->CreateTexture(desc, NULL, 0) ) 
		{
			CON(MSG_ERR_FATAL, _W("Failed to create depth RT!") );
			SAFE_DELETE(diffuse);
			SAFE_DELETE(normal);
			SAFE_DELETE(depth);
			return false;
		}
		depth->SetFilename(_W("GBuffer-depth"));

		m_numGBUnits = 3;
		m_pGBUnits = new sFrameBufferUnit[3];

		m_pGBUnits[0].renderTarget = diffuse;
		m_pGBUnits[0].usage = P3D_FBUNIT_DIFFUSE;
		m_pGBUnits[1].renderTarget = normal;
		m_pGBUnits[1].usage = P3D_FBUNIT_NORMAL;
		m_pGBUnits[2].renderTarget = depth;
		m_pGBUnits[2].usage = P3D_FBUNIT_DEPTH;

		return true;
	}

}//NS