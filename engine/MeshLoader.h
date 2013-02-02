/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "IMeshLoader.h"

namespace P3D{

	class CMeshLoader : public IMeshLoader
	{
	public:
		CMeshLoader();
		~CMeshLoader();
		/// Load rmesh
		/** You can skip loading animation data and treat this mesh as static with ignore_animation_data = true. */
		bool Load(const wchar* path, bool ignore_animation_data=false);
		void Unload();
		inline bool IsLoaded()const{ return m_bLoaded; }; 
		inline const sRMInfo* GetInfo()const{ return m_rinfo; };
		inline bool IsAnimated()const{ return m_bAnimated; };

		inline const sRMSubset* GetSubsets(OUT unsigned int &numSubsets){ numSubsets=m_nSubsets; return m_rsubsets; };
		inline const sRMVertex* GetVertices(OUT unsigned int &numVerts){ numVerts=m_nVerts; return m_rverts; };
		inline const unsigned int* GetIndices(OUT unsigned int &numInds){ numInds=m_nInds; return m_rinds; };
	private:
		bool m_bLoaded; bool m_bAnimated;
		sRMHeader *m_rhead; // only one struct
		sRMInfo *m_rinfo; // only one struct
		// arrays
		sRMSubset *m_rsubsets; unsigned int m_nSubsets;
		sRMVertex *m_rverts; unsigned int m_nVerts;
		sRMIndex *m_rinds; unsigned int m_nInds; unsigned int *m_faceGroups;
	};

}; // namespace