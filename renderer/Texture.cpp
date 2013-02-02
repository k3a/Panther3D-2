/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "Texture.h"

namespace P3D
{

	CTexture::CTexture()
	{
		m_nNumReferences = 0;
		m_bAutoRelease = true;
	}

	CTexture::~CTexture()
	{

	}

	void CTexture::EnableAutoRelease(bool enable)
	{
		m_bAutoRelease = enable;
	}

	unsigned int CTexture::AddReference()
	{
		m_nNumReferences++;
		return m_nNumReferences;
	}

	unsigned int CTexture::EraseReference()
	{
		if(m_nNumReferences > 0) m_nNumReferences--;
		return m_nNumReferences;
	}

	const wchar* CTexture::GetFilename()
	{
		return m_Filename.Get();
	}

	unsigned int CTexture::GetNumReferences()
	{
		return m_nNumReferences;
	}

	void CTexture::SetFilename(const wchar *filename)
	{
		m_Filename = filename;
	}

	bool CTexture::CanRelease()
	{
		return m_bAutoRelease;
	}

};
