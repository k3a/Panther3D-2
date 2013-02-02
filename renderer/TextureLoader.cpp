/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#include "Renderer.h"
#include "TextureLoader.h"

namespace P3D
{

// ------------------------------------------------
bool CTextureLoader::Load(const wchar* path)
{
	IFileSystem *fs = CRenderer::mEngine()->mFilesystem();

	Unload();
	//memset(&m_info, 0, sizeof(sRTInfo));

	// make physical FS path
	wchar physPath[512];
	wsprintf(physPath, 511, _W("textures/%s.rtex"), path);
	for (unsigned int i=0; i<wstrlen(physPath); i++)
		if (physPath[i]==',') physPath[i]='/';

	FSFILE* fp;
	fp =  fs->Open(physPath, _W("rb"));
	if (!fp)
	{
		return false;
	}

	m_dataLen = fp->nLen;

	if (m_dataLen < sizeof(sRTHeader))
	{
		fs->Close(fp);
		return false;
	}

	// read header
	sRTHeader head;
	fs->Read(&head, 1, sizeof(sRTHeader), fp);

	// check header
	if (head.magic[0]!='R' || head.magic[1]!='T')
	{
		CON(MSG_ERR, _W("Texture %s is not valid rtex!"), path);
		fs->Close(fp);
		return false;
	}

	// load texture info
	fs->Seek(fp, head.contents[RT_FILE_INFO].offset, SEEK_SET);
	fs->Read(&m_info, 1, sizeof(sRTInfo), fp);

	// load texture data
	unsigned long rawDataLen = fp->nLen-head.contents[RT_IMAGE_SUBSET].offset-head.contents[RT_IMAGE_SUBSET].length;
	m_data = new BYTE[rawDataLen];
	fs->Seek(fp, head.contents[RT_IMAGE_SUBSET].offset+head.contents[RT_IMAGE_SUBSET].length, SEEK_SET);
	fs->Read(m_data, 1, rawDataLen, fp);

	m_bLoaded=true;

	// load all subsets
	fs->Seek(fp, head.contents[RT_IMAGE_SUBSET].offset, SEEK_SET);
	unsigned int numSubs = head.contents[RT_IMAGE_SUBSET].length / sizeof(sRTImageSubset);

	for (unsigned int s = 0; s < numSubs; s++)
	{
		sImageData* ida = new sImageData();
		fs->Read(&ida->subset, 1, sizeof(sRTImageSubset), fp);
		ida->size = ida->subset.len;
		ida->pData = &m_data[ida->subset.offset];
		m_subs.push_back(ida);
	}

	fs->Close(fp);

	int i = 0; // debug

	return true;
}

const sImageData* CTextureLoader::GetMipmapSubset(unsigned int mipmapID)const
{
	for (unsigned int i=0; i<m_subs.size(); i++)
		if (m_subs[i]->subset.mipmapID==mipmapID)
			return m_subs[i];
	return NULL;
}

const sImageData* CTextureLoader::GetCubemapSubset(eRTCubeSide cubeSide, unsigned int mipmapID/*=0*/)const
{
	for (unsigned int i=0; i<m_subs.size(); i++)
		if (m_subs[i]->subset.cubeFaceID==cubeSide && m_subs[i]->subset.mipmapID==mipmapID)
			return m_subs[i];
	return NULL;
}

const sImageData* CTextureLoader::GetSubsetByID(unsigned int subsetID)const
{
	if (subsetID>=m_subs.size()) return NULL;
	return m_subs[subsetID];
}



}; // namespace