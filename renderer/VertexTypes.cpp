/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "VertexTypes.h"
#include "renderer.h"

namespace P3D
{

	void RegisterVertexTypes()
	{
		VertexElement VDPosUV[] = 
		{	
			VertexElement(P3DVD_FLOAT3, P3DVU_POSITION), 
			VertexElement(P3DVD_FLOAT2, P3DVU_TEXCOORD),
			P3DVE_END()
		};
		
		VertexElement VDPosUVCol[] = 
		{	
			VertexElement(P3DVD_FLOAT3, P3DVU_POSITION), 
			VertexElement(P3DVD_FLOAT2, P3DVU_TEXCOORD),
			VertexElement(P3DVD_FLOAT4, P3DVU_COLOR),
			P3DVE_END()
		};

		VertexElement VDPosUVNormal[] = 
		{	
			VertexElement(P3DVD_FLOAT3, P3DVU_POSITION), 
			VertexElement(P3DVD_FLOAT2, P3DVU_TEXCOORD),
			VertexElement(P3DVD_FLOAT3, P3DVU_NORMAL), 
			P3DVE_END()
		};

		VertexElement VDStaticMesh[] = 
		{	
			VertexElement(P3DVD_FLOAT3, P3DVU_POSITION),
			P3DVE_END()
		};

		CRenderer::cGraphicsManager()->RegisterVertexDesc(VDPosUVCol, Str( _W("VDPosUV") ));
		CRenderer::cGraphicsManager()->RegisterVertexDesc(VDPosUVCol, Str( _W("VDPosUVCol") ));
		CRenderer::cGraphicsManager()->RegisterVertexDesc(VDPosUVNormal, Str( _W("VDPosUVNormal") ));
		
		// default vertex decls
		CRenderer::cGraphicsManager()->RegisterVertexDesc(VDStaticMesh, Str( _W("VDStaticMesh") ));

	}

};
