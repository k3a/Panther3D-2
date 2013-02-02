/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

#if defined(_DX) && _DX == 9

#include "Mesh.h"
#include "symbols.h"

namespace P3D
{

//------------------------------------------
CMeshWrapper::CMeshWrapper(){
	m_pMesh = NULL;
}

//------------------------------------------
CMeshWrapper::~CMeshWrapper(){
	SAFE_RELEASE(m_pMesh);
}

//------------------------------------------
void CMeshWrapper::DrawSubset(DWORD subsetID){
	assert(m_pMesh!=0);
	m_pMesh->DrawSubset(subsetID);
}

//------------------------------------------
bool CMeshWrapper::LoadMeshFromXFile(const char* szFileName){
	//TODO: Nacti dany soubor pouzitim filesystemu do pameti a pak predej do funkce
	//D3DXLoadMeshFromXInMemory(mem, memSize, opt, device, ..., ..., ..., ..., &m_pMesh);
	//tim ziskat mesh a uvolnit pamet.
	return false;
}

};

#endif