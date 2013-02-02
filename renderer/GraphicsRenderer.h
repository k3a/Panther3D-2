/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

#include "IGraphicsRenderer.h"

namespace P3D{

	class CGraphicsRenderer : public IGraphicsRenderer
	{
	public:
		CGraphicsRenderer();
		~CGraphicsRenderer();

		/// Initialize graphics renderer
		bool Initialize();

		void RenderScene();//TODO: delete this method

		void RenderScene(sFrameBufferUnit *FBUnits, UINT numUnits);

		// for renderer proejct
		void OnResetDevice();
		void OnLostDevice();

		// from iface
		void SendChunks(const tArray<GeometryChunk> &chunks);

	private:
		tArray<GeometryChunk> m_chunks;

		// MRT
		ITexture *m_mrt_diffuse;
		ITexture *m_mrt_normal;
		ITexture *m_mrt_depth;

	};

}; // ns