/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k, am!go
*/
#pragma once

#include <d3dx9.h>
#include <vector>
#include <string>

namespace P3D
{

class CMeshWrapper
{
public:
	CMeshWrapper();
	~CMeshWrapper();

	/**
	\brief Draws

	Render part of mesh using current device world, view and projection matrices.

	\param subsetID Subset number to be drawn.
	*/
	void DrawSubset(DWORD subsetID);

	/**
	\brief Loads mesh from file.

	Loads mesh from specified .x file using file system.

	\param szFileName Path to desired .x file.
	*/
	bool LoadMeshFromXFile(const char* szFileName);

	// TODO: funkcionalita podobna ID3DXMesh

private:
	ID3DXMesh *m_pMesh;
	std::vector<std::string> m_TexFilenames; //an array of mesh's texture filenames
	std::string m_MeshDirectory; //relative path into mesh's directory
	std::string m_MeshName; //Filename of the mesh
};

//CMesh could be a class derived from CMeshWrapper or etc. so we take it into consideration.
typedef CMeshWrapper CMesh;

};