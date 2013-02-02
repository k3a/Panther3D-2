/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once


#include <AL/al.h>
#include <AL/alc.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include "types.h"
#include "ISoundListener.h"

namespace P3D
{

	class CSoundListener : public ISoundListener
	{
	public:
		CSoundListener();
		~CSoundListener();
		bool Create(Vec3 pos, Vec3 lookvec);
		void SetPosition(Vec3 position);
		void SetLookAt(Vec3 position);
		void SetLookVector(Vec3 v);
		void SetVelocity(Vec3 velocity);
		inline Vec3 GetPosition() { return Vec3(m_pos); };
		inline Vec3 GetLookVector() { return Vec3(m_ori[0], m_ori[1],m_ori[2]); };
		inline Vec3 GetVelocity() { return Vec3(m_vel); };
	private:
		ALfloat m_pos[3];
		ALfloat m_vel[3];
		ALfloat m_ori[6];

	};

}