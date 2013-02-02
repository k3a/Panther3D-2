/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"

#include "SoundListener.h"

namespace P3D
{
	CSoundListener::CSoundListener()
	{

	}

	CSoundListener::~CSoundListener()
	{

	}

	bool CSoundListener::Create(Vec3 pos, Vec3 lookvec)
	{
		m_pos[0] = pos.x;
		m_pos[1] = pos.y;
		m_pos[2] = pos.z;

		m_ori[0] = lookvec.x;
		m_ori[1] = lookvec.y;
		m_ori[2] = lookvec.z;
		m_ori[3] = 0.0f;
		m_ori[4] = 1.0f;
		m_ori[5] = 0.0f;

		m_vel[0] = m_vel[1] = m_vel[2] = 1.0f;

		alListenerfv(AL_POSITION, m_pos);
		alListenerfv(AL_VELOCITY, m_vel);
		alListenerfv(AL_ORIENTATION, m_ori);

		return true;
	}

	void CSoundListener::SetPosition(Vec3 position)
	{
		m_pos[0] = position.x;
		m_pos[1] = position.y;
		m_pos[2] = position.z;
		alListenerfv(AL_POSITION, m_pos);
	}

	void CSoundListener::SetVelocity(Vec3 velocity)
	{
		m_vel[0] = velocity.x;
		m_vel[1] = velocity.y; 
		m_vel[2] = velocity.z;
		alListenerfv(AL_VELOCITY, m_vel);
	}

	void CSoundListener::SetLookVector(Vec3 v)
	{
		m_ori[0] = v.x;
		m_ori[1] = v.y;
		m_ori[2] = v.z;
		alListenerfv(AL_ORIENTATION, m_ori);
	}

	void CSoundListener::SetLookAt(Vec3 position)
	{
		m_ori[0] = position.x - m_pos[0];
		m_ori[1] = position.y - m_pos[1];
		m_ori[2] = position.z - m_pos[2];
		alListenerfv(AL_ORIENTATION, m_ori);
	}

}