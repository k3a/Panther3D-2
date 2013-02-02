/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#include "precompiled.h"

#include "MeshLoader.h"
#include "Engine.h"

namespace P3D{

CMeshLoader::CMeshLoader()
{
	m_bLoaded=false;

	m_rhead=NULL;
	m_rinfo=NULL;

	m_rsubsets=NULL;
	m_rverts=NULL;
	m_rinds=NULL;
	m_faceGroups=NULL;
}

CMeshLoader::~CMeshLoader()
{
	Unload();
}

bool CMeshLoader::Load(const wchar* path, bool ignore_animation_data/*=false*/)
{
	if (m_bLoaded)
	{
		Unload();	
		m_bLoaded = false;
	}
	IFileSystem* pFS = CEngine::instance()->mFilesystem();

	unsigned int ltim = GetTickCount();

	// open file
	wchar fspath[P3DMAX_PATH];
	wsprintf(fspath, P3DMAX_PATH-1, _W("models/%s.rmesh"), path);
	FSFILE* fp = pFS->Open(fspath, _W("rb"));
	if (!fp)
	{
		CON(MSG_ERR, _W("MeshLoader: Can't find mesh file '%s'!"), fspath);
		return false;
	}
	if (fp->nLen<sizeof(sRMHeader))
	{
		CON(MSG_ERR, _W("MeshLoader: Mesh file '%s' is corrupted!"), fspath);
		pFS->Close(fp);
		return false;
	}

	// load and check header
	m_rhead = new sRMHeader();
	pFS->Read(m_rhead, sizeof(sRMHeader), 1, fp);
	if (m_rhead->magic[0]!='R' || m_rhead->magic[1]!='M')
	{
		CON(MSG_ERR, _W("MeshLoader: File '%s' is not valid RMesh!"), fspath);
		pFS->Close(fp);
		return false;
	}
	if (m_rhead->major!=RM_MAJOR)
	{
		CON(MSG_ERR, _W("MeshLoader: Mesh '%s' is version %d, but engine can load only %d!"), fspath, m_rhead->major, RM_MAJOR);
		pFS->Close(fp);
		return false;
	}
	if (m_rhead->minor!=RM_MINOR)
	{
		CON(MSG_DBG, _W("MeshLoader: Minor version warning. Mesh '%s' is minor version %d, engine can load %d."), fspath, m_rhead->minor, RM_MINOR);
	}

	m_bLoaded=true;

	// LOAD INFO ---------------------------------------------------
	m_rinfo = new sRMInfo();
	pFS->Seek(fp, m_rhead->contents[RM_FILE_INFO].offset, SEEK_SET);
	pFS->Read(m_rinfo, sizeof(sRMInfo), 1, fp);

	// LOAD SUBSETS ---------------------------------------------------
	m_nSubsets = m_rhead->contents[RM_SUBSETS].length/sizeof(sRMSubset);
	m_rsubsets = new sRMSubset[m_nSubsets];
	pFS->Seek(fp, m_rhead->contents[RM_SUBSETS].offset, SEEK_SET);
	pFS->Read(m_rsubsets, sizeof(sRMSubset), m_nSubsets, fp);

	// LOAD VERTICES ---------------------------------------------------
	m_nVerts = m_rhead->contents[RM_VERTICES].length/sizeof(sRMVertex);
	m_rverts = new sRMVertex[m_nVerts];
	pFS->Seek(fp, m_rhead->contents[RM_VERTICES].offset, SEEK_SET);
	pFS->Read(m_rverts, sizeof(sRMVertex), m_nVerts, fp);

	// LOAD INDICES ---------------------------------------------------
	m_nInds = m_rhead->contents[RM_INDICES].length/sizeof(sRMIndex);
	m_rinds = new sRMIndex[m_nInds];
	pFS->Seek(fp, m_rhead->contents[RM_INDICES].offset, SEEK_SET);
	pFS->Read(m_rinds, sizeof(sRMIndex), m_nInds, fp);

	// MAKE FACE GROUPS -----------------------------------------------
	m_faceGroups = new unsigned int[m_nInds/3];
	memset(m_faceGroups, 0, sizeof(unsigned int)*m_nInds/3);
	for (unsigned int s=0; s<m_nSubsets; s++)
	{
		// find faces which belongs to this subset
		unsigned int beginFace = m_rsubsets[s].firstindex/3;
		unsigned int endFace = (m_rsubsets[s].firstindex+m_rsubsets[s].numindices)/3;
		
		// assign face groups to this subset
		for (unsigned int f=beginFace; f<endFace; f++) m_faceGroups[f]=s;
	}

	// IF THIS FILE HAS ANIMATION AND IT IS LAODED MAKE SURE TO SET m_bAnimated TO TRUE
	m_bAnimated = false;
	if (!m_rinfo->idontimportanim)
	{
		// check if file contain animated content
		// TODO:
	}

	// close file
	pFS->Close(fp);

	ltim = GetTickCount() - ltim;
	if (ltim<1000)
		CON(MSG_INFO, _W("MeshLoader: Mesh '%s' loaded. Loading took %d miliseconds."), path, ltim);
	else
		CON(MSG_INFO, _W("MeshLoader: Mesh '%s' loaded. Loading took %d seconds."), path, ltim/1000);
	return true;
}

void CMeshLoader::Unload()
{
	SAFE_DELETE(m_rhead);
	SAFE_DELETE(m_rinfo);

	SAFE_DELETE_ARRAY(m_rsubsets);
	SAFE_DELETE_ARRAY(m_rverts);
	SAFE_DELETE_ARRAY(m_rinds);
	SAFE_DELETE_ARRAY(m_faceGroups);
}


}; // namespace