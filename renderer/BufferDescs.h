/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"
#include "BufferRequisities.h"

namespace P3D
{	

	/*enum eCPUAccess
	{
		CPUACCESS_NONE,
		CPUACCESS_READ,
		CPUACCESS_WRITE,
		CPUACCESS_READ_WRITE
	};

	enum eResourceUsage
	{
		RESOURCEUSAGE_DEFAULT,
		RESOURCEUSAGE_DYNAMIC,
		RESOURCEUSAGE_IMMUTABLE
	};

	enum eIndexBufferFormat
	{
		IBFORMAT_16,
		IBFORMAT_32
	};*/
	
	/** Describes a CVertexBuffer */
	struct sVBDesc
	{
		sVBDesc() { memset(this, 0, sizeof(sVBDesc)); }
		
		bool IsValid()
		{
			return true;
		}

		UINT VertexCount;
		UINT VertexSize;
		eCPUAccess CPUAccess;
		eResourceUsage ResourceUsage;
		bool PointParticles;
	};

	/** Describes a CIndexBuffer */
	struct sIBDesc
	{
		sIBDesc() { memset(this, 0, sizeof(sIBDesc)); }

		bool IsValid()
		{
			return true;
		}
		
		UINT IndexCount;
		eCPUAccess CPUAccess;
		eResourceUsage ResourceUsage;
		eIndexBufferFormat IndexFormat;
	};

	/** Describes a CShaderBuffer */
	struct sShaderBufferDesc
	{
		UINT BufferSize;
		eCPUAccess CPUAccess;
		eResourceUsage ResourceUsage;
	};

};