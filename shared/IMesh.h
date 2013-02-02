/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)=0
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "ITexture.h"

namespace P3D
{

	class IMesh
	{
	public:
		virtual void LockVertexBuffer(void **data, bool discard)=0;
		virtual void UnLockVertexBuffer()=0;
		virtual void LockIndexBuffer(void **data, bool discard)=0;
		virtual void UnLockIndexBuffer()=0;
		virtual inline unsigned int GetNumFaces()=0;
		virtual inline unsigned int GetNumVertices()=0;
		//virtual void Optimize()=0;
		virtual inline unsigned int GetNumSubsets()=0;
		virtual inline Sphere GetBoundingSphere()=0;
		virtual void RenderSubset(unsigned int subset)=0;
		virtual ITexture* GetTexture(unsigned int subset)=0;
		virtual inline AABB GetAABB()=0;
		virtual const wchar* GetFilename()=0;

	};

};