/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "Engine.h"
#include "Texture.h"

namespace P3D {

CTexture::CTexture()
{

	REGISTER_PARAM(Name, "Virtual path to texture (unique texture identificator)");

	// read-only info params
	REGISTER_PARAM_READONLY(Size, "Texture size in pixels");
	REGISTER_PARAM_READONLY(NumMips, "Number of texture mipmaps");
}

bool CTexture::Load(const wchar* name)
{
	m_texture = CEngine::instance()->mRenderer()->iGraphicsManager()->LoadTexture(name);

	//Size = ...
	return false;
}

REGISTER_OBJECT(CTexture);


};//ns