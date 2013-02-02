/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#if defined(_DX) && _DX==9
#include "DX/TextureWrapper.h"
#elif defined(_OGL)
#include "OGL/TextureWrapper.h"
#elif defined(_DX) && _DX==10
#include "DX10/TextureWrapper.h"
#endif



namespace P3D
{

	class CTexture : public CTextureWrapper
	{
	public:
		CTexture();
		~CTexture();
		//Increases/decreases number of references and returns it.
		//If the the number of references reaches 0 the texture is erased
		unsigned int AddReference();
		unsigned int EraseReference();
		unsigned int GetNumReferences();
		void SetFilename(const wchar *filename);
		const wchar* GetFilename();
		void EnableAutoRelease(bool enable);
		bool CanRelease();
	private:
		unsigned int m_nNumReferences;
		bool m_bAutoRelease;
	};

};
