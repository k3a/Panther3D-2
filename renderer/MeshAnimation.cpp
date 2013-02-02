/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "MeshAnimation.h"
#include "Renderer.h"

namespace P3D
{

	bool CMeshAnimation::Create()
	{
		Matrix identity;
		identity.SetIdentityMatrix();
		UpdateBone(m_pRootBone, identity);
		return true;
	}
	
	void CMeshAnimation::UpdateBone(P3D::Bone *bone, Matrix &parentsToRoot)
	{
		//TODO: update transform matrix
		bone->ToRoot = bone->TransformMatrix * parentsToRoot;

		if(bone->pNextSibling) UpdateBone(bone->pNextSibling, parentsToRoot);
		if(bone->pFirstChild) UpdateBone(bone->pFirstChild, bone->ToRoot);
	}

}