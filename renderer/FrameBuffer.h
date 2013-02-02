/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 09/07/2009 File created - Petr
*/

#pragma once

#include "Texture.h"

namespace P3D
{

	enum eFrameBufferUnitUsage
	{
		P3D_FBUNIT_DIFFUSE,
		P3D_FBUNIT_NORMAL,
		P3D_FBUNIT_DEPTH,
		P3D_FBUNIT_UNKNOWN
	};

	struct sFrameBufferUnit
	{
		sFrameBufferUnit()
		{
			renderTarget = NULL;
			usage = P3D_FBUNIT_UNKNOWN;
		}

		~sFrameBufferUnit()
		{
			SAFE_DELETE(renderTarget);
		}

		CTexture *renderTarget;
		eFrameBufferUnitUsage usage;
	};

	class CFrameBuffer
	{
	public:
		CFrameBuffer();
		~CFrameBuffer();
		bool Create(Vec2i rtSize);
		bool Resize(Vec2i newSize);
		inline UINT GetNumFBUnits() { return m_numGBUnits; };
		inline sFrameBufferUnit* GetFBUnits() { return m_pGBUnits; };
		inline sFrameBufferUnit* GetFBUnit(UINT Id) { assertd(Id>=m_numGBUnits, "Id out of range!"); return &m_pGBUnits[Id]; };
		sFrameBufferUnit* GetFBUnitByUsage(eFrameBufferUnitUsage usage);

	private:
		bool Recreate(Vec2i newSize);
		sFrameBufferUnit *m_pGBUnits;
		UINT m_numGBUnits;
	};

}//NS