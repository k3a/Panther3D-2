/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once

#include "types.h"
#include <vector>

namespace P3D
{
	struct Bone
	{
		wchar Name[32]; //name of the bone
		unsigned int ID; //identificator
		Matrix TransformMatrix; //or quaternion?
		Matrix ToRoot; //to root bone transform
		Bone *pNextSibling; //pointer to this bone's sibling bone
		Bone *pFirstChild; //pointer to this bone's child bone
	};

	struct Animation
	{
		wchar Name[32]; //name of the animation
		float length;
		BYTE NumBobes;
		unsigned int StartID;
		unsigned int EndID;
	};

	struct BoneAnimation
	{
		unsigned int ID;
		float time;
		BYTE boneid;
		Vec3 position;
		Vec3 orientation;
	};

	struct Keyframe
	{

	};

	class CMeshAnimation
	{
	public:
		bool Create();
	private:
		void UpdateBone(Bone *bone, Matrix &parentsToRoot);

		Bone *m_pRootBone;
		static const int MAX_NUM_BONES_SUPPORTED = 35;
		Bone *m_sortedBones; //bones sorted by their id for fast accesing
		double *m_currentTime; //current animation time
	};
};