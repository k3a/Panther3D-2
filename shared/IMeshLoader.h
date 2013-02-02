/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once
#include "interface.h"
#include "rmesh.h"

namespace P3D
{
	class IMeshLoader
	{
	public:
		virtual bool Load(const wchar* path, bool ignore_animation_data=false)=0;
		virtual void Unload()=0;
		virtual inline bool IsLoaded()const=0; 
		virtual inline const sRMInfo* GetInfo()const=0;
		virtual inline bool IsAnimated()const=0;

		virtual inline const sRMSubset* GetSubsets(OUT unsigned int &numSubsets)=0;
		virtual inline const sRMVertex* GetVertices(OUT unsigned int &numVerts)=0;
		virtual inline const unsigned int* GetIndices(OUT unsigned int &numInds)=0;
	};
}