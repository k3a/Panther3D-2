/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

namespace P3D
{	

	enum eCPUAccess
	{
		P3D_CPUACCESS_NONE,
		P3D_CPUACCESS_READ,
		P3D_CPUACCESS_WRITE,
		P3D_CPUACCESS_READ_WRITE
	};

	enum eResourceUsage
	{
		P3D_RESOURCEUSAGE_DEFAULT,
		P3D_RESOURCEUSAGE_DYNAMIC,
		P3D_RESOURCEUSAGE_IMMUTABLE
	};

	enum eIndexBufferFormat
	{
		IBFORMAT_16,
		IBFORMAT_32
	};
	
	/** Defines Vertex Type for P3DVertexElement struct \see P3DVertexElement */
	enum VertexType
	{
		P3DVD_NONE = 0,
		P3DVD_FLOAT1,
		P3DVD_FLOAT2,
		P3DVD_FLOAT3,
		P3DVD_FLOAT4
	};

#define P3DVD_COLORRGB P3DVD_FLOAT3
#define P3DVD_COLORRGBA P3DVD_FLOAT4

	/** Defines Vertex Usage for P3DVertexElement struct \see P3DVertexElement */
	enum VertexUsage
	{
		P3DVU_NONE = 0,
		P3DVU_POSITION,
		P3DVU_BLENDWEIGHT,
		P3DVU_BLENDINDICES,
		P3DVU_NORMAL,
		P3DVU_PSIZE,
		P3DVU_TEXCOORD,
		P3DVU_TANGENT,
		P3DVU_BINORMAL,
		P3DVU_TESSFACTOR,
		P3DVU_POSITIONT,
		P3DVU_COLOR,
		P3DVU_FOG,
		P3DVU_DEPTH,
		P3DVU_SAMPLE
	};

	/** Defines Vertex Element which is used for generating the Vertex Declaration \see IP3DVertexBuffer */
	struct VertexElement
	{
		VertexElement()
		{

		}

		VertexElement(UINT type, UINT usage)
		{
			vertexType = type;
			vertexUsage = usage;
		}

		UINT vertexType;
		UINT vertexUsage;
	};

	// Set this as last element of the P3DVertexElement array
	#define P3DVE_END() P3D::VertexElement(P3DVD_NONE, P3DVU_NONE)

	enum PrimitiveType 
	{
		P3DPT_POINTLIST,
		P3DPT_LINELIST,
		P3DPT_LINESTRIP,
		P3DPT_TRIANGLELIST,
		P3DPT_TRIANGLESTRIP,
		P3DPT_TRIANGLEFAN,
	};

	enum eResourceFormat
	{
		//unsigned normalized <0;1>
		P3D_FORMAT_L8_UNORM,
		P3D_FORMAT_X8R8G8B8_UNORM,
		P3D_FORMAT_A8R8G8B8_UNORM,
		//signed normalized <-1;1>
		P3D_FORMAT_R16G16_SNORM,
		//floating point
		P3D_FORMAT_R32_FLOAT,
		//compressed
		P3D_FORMAT_DXT1,
		P3D_FORMAT_DXT3,
		P3D_FORMAT_DXT5,
		//depth formats
		P3D_FORMAT_D24S8
	};

};