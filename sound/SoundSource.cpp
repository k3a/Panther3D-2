/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go, kalda
*/
#include "precompiled.h"

#include "SoundSource.h"

namespace P3D
{

	CSoundSource::CSoundSource()
	{
		m_source = AL_NONE;
		m_effectSlot = AL_NONE;
		m_effect = AL_NONE;
		m_exts = 0;
		m_reverbInitialized = false;
	}

	CSoundSource::~CSoundSource()
	{
		if(m_source) alDeleteSources(1, &m_source);
		m_source = AL_NONE;
	}

	bool CSoundSource::Create(COALExtProvider *exts, P3D::CSoundBuffer *buffer, Vec3 position, bool relativeToListener)
	{
		m_buffer = buffer;
		m_exts = exts;

		if(m_exts)
		{
			if(m_exts->IsEfxSupported())
			{
				if(m_exts->CreateAuxEffectSlot(&m_effectSlot))
				{
					if(m_exts->CreateEffect(&m_effect, AL_EFFECT_EAXREVERB))
					{
						m_reverbInitialized = true;
					}
				}
			}
		}

		alGenSources(1,&m_source);
		alSourcei(m_source, AL_BUFFER, *(ALint*)buffer->GetBuffer());
		SetPosition(position);
		SetSourceRelativity(relativeToListener);
		return true;
	}
	
	void CSoundSource::Play()
	{
		alSourcePlay(m_source);
	}

	void CSoundSource::Pause()
	{
		alSourcePause(m_source);
	}

	void CSoundSource::Stop()
	{
		alSourceStop(m_source);
	}

	void CSoundSource::Rewind()
	{
		alSourceRewind(m_source);
	}

	void CSoundSource::Seek(float seconds)
	{
		alSourcef(m_source, AL_SEC_OFFSET, seconds);
	}

	void CSoundSource::SetState(eSoundState state)
	{
		ALint s = AL_STOPPED;
		
		if(state == Sound_Play)
			s = AL_PLAYING;
		else if(state == Sound_Pause)
			s = AL_PAUSED;

		alSourcei(m_source, AL_SOURCE_STATE, s);
	}

	void CSoundSource::SetPitch(float pitch)
	{
		alSourcef(m_source, AL_PITCH, pitch);
	}

	void CSoundSource::SetGain(float gain)
	{
		alSourcef(m_source, AL_GAIN, gain);
	}

	void CSoundSource::SetVolume(float volume)
	{
		alSourcef(m_source, AL_GAIN, volume);
	}

	void CSoundSource::SetMaxDistance(float maxDist)
	{
		alSourcef(m_source, AL_MAX_DISTANCE, maxDist);
	}

	void CSoundSource::SetRolloffFactor(float rolloff)
	{
		alSourcef(m_source, AL_ROLLOFF_FACTOR, rolloff);
	}

	void CSoundSource::SetReferenceDistance(float refDist)
	{
		alSourcef(m_source, AL_REFERENCE_DISTANCE, refDist);
	}

	void CSoundSource::SetMinGain(float minGain)
	{
		alSourcef(m_source, AL_MIN_GAIN, minGain);
	}

	void CSoundSource::SetMaxGain(float maxGain)
	{
		alSourcef(m_source, AL_MAX_GAIN, maxGain);
	}

	void CSoundSource::SetMinVolume(float minVolume)
	{
		alSourcef(m_source, AL_MIN_GAIN, minVolume);
	}

	void CSoundSource::SetMaxVolume(float maxVolume)
	{
		alSourcef(m_source, AL_MAX_GAIN, maxVolume);
	}

	void CSoundSource::SetConeOuterGain(float outerGain)
	{
		alSourcef(m_source, AL_CONE_OUTER_GAIN, outerGain);
	}

	void CSoundSource::SetConeInnerAngle(float innerAngleDeg)
	{
		alSourcef(m_source, AL_CONE_INNER_ANGLE, innerAngleDeg);
	}

	void CSoundSource::SetConeOuterAngle(float outerAngleDeg)
	{
		alSourcef(m_source, AL_CONE_OUTER_ANGLE, outerAngleDeg);
	}

	void CSoundSource::SetPosition(Vec3& position)
	{
		alSource3f(m_source, AL_POSITION, position.x, position.y, position.z);
	}

	void CSoundSource::SetVelocity(Vec3& velocity)
	{
		alSource3f(m_source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}

	void CSoundSource::SetDirection(Vec3& direction)
	{
		alSource3f(m_source, AL_DIRECTION, direction.x, direction.y, direction.z);
	}

	void CSoundSource::SetPosition(float x, float y, float z)
	{
		alSource3f(m_source, AL_POSITION, x, y, z);
	}

	void CSoundSource::SetVelocity(float x, float y, float z)
	{
		alSource3f(m_source, AL_VELOCITY, x, y, z);
	}

	void CSoundSource::SetDirection(float x, float y, float z)
	{
		alSource3f(m_source, AL_DIRECTION, x, y, z);	
	}

	void CSoundSource::SetSourceRelativity(bool relativeToListener)
	{
		alSourcei(m_source, AL_SOURCE_RELATIVE, relativeToListener ? AL_TRUE : AL_FALSE);
	}

	void CSoundSource::SetLooping(bool loop)
	{	
		alSourcei(m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	void CSoundSource::SetOffsetInSeconds(float seconds)
	{
		alSourcef(m_source, AL_SEC_OFFSET, seconds);
	}

	void CSoundSource::SetOffsetInSamples(int samples)
	{
		alSourcei(m_source, AL_SAMPLE_OFFSET, samples);
	}

	void CSoundSource::SetOffsetInBytes(int bytes)
	{
		alSourcei(m_source, AL_BYTE_OFFSET, bytes);
	}


	eSoundState CSoundSource::GetState()
	{
		ALint state;
		alGetSourcei(m_source, AL_SOURCE_STATE, &state);

		switch(state)
		{
		case AL_PLAYING:
			return Sound_Play;
		case AL_PAUSED:
			return Sound_Pause;
		}

		return Sound_Stop;
	}

	float CSoundSource::GetPitch()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_PITCH, &value);
		return value;
	}

	float CSoundSource::GetGain()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_GAIN, &value);
		return value;
	}

	float CSoundSource::GetVolume()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_GAIN, &value);
		return value;
	}

	float CSoundSource::GetMaxDistance()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_MAX_DISTANCE, &value);
		return value;
	}

	float CSoundSource::GetRolloffFactor()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_ROLLOFF_FACTOR, &value);
		return value;
	}

	float CSoundSource::GetReferenceDistance()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_REFERENCE_DISTANCE, &value);
		return value;
	}

	float CSoundSource::GetMinGain()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_MIN_GAIN, &value);
		return value;
	}

	float CSoundSource::GetMaxGain()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_MAX_GAIN, &value);
		return value;
	}

	float CSoundSource::GetMinVolume()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_MIN_GAIN, &value);
		return value;
	}

	float CSoundSource::GetMaxVolume()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_MAX_GAIN, &value);
		return value;
	}

	float CSoundSource::GetConeOuterGain()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_CONE_OUTER_GAIN, &value);
		return value;
	}

	float CSoundSource::GetConeInnerAngle()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_CONE_INNER_ANGLE, &value);
		return value;
	}

	float CSoundSource::GetConeOuterAngle()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_CONE_OUTER_ANGLE, &value);
		return value;
	}

	Vec3  CSoundSource::GetPosition()
	{
		Vec3 value;
		GetPosition(value);
		return value;
	}

	Vec3  CSoundSource::GetVelocity()
	{
		Vec3 value;
		GetVelocity(value);
		return value;
	}

	Vec3  CSoundSource::GetDirection()
	{
		Vec3 value;
		GetDirection(value);
		return value;
	}

	void  CSoundSource::GetPosition(Vec3& position)
	{
		alGetSource3f(m_source, AL_POSITION, &position.x, &position.y, &position.z);
	}

	void  CSoundSource::GetVelocity(Vec3& velocity)
	{
		alGetSource3f(m_source, AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z);
	}

	void  CSoundSource::GetDirection(Vec3& direction)
	{
		alGetSource3f(m_source, AL_DIRECTION, &direction.x, &direction.y, &direction.z);
	}

	bool  CSoundSource::GetSourceRelativity()
	{
		ALint value;
		alGetSourcei(m_source, AL_SOURCE_RELATIVE, &value);
		return (value == AL_TRUE) ? true : false;
	}

	bool  CSoundSource::GetLooping()
	{
		ALint value;
		alGetSourcei(m_source, AL_LOOPING, &value);
		return (value == AL_TRUE) ? true : false;
	}

	float CSoundSource::GetOffsetInSeconds()
	{
		ALfloat value;
		alGetSourcef(m_source, AL_SEC_OFFSET, &value);
		return value;
	}

	int   CSoundSource::GetOffsetInSamples()
	{
		ALint value;
		alGetSourcei(m_source, AL_SAMPLE_OFFSET, &value);
		return value;
	}

	int   CSoundSource::GetOffsetInBytes()
	{
		ALint value;
		alGetSourcei(m_source, AL_BYTE_OFFSET, &value);
		return value;
	}

	bool  CSoundSource::IsStreamed()
	{
		ALint value;
		alGetSourcei(m_source, AL_SOURCE_TYPE, &value);
		return (value == AL_STREAMING) ? true : false;
	}


	void  CSoundSource::SetReverb(sSoundReverbProperties reverb)
	{
		if(!m_exts || !m_reverbInitialized)
			return;

		m_reverb = reverb;

		EFXEAXREVERBPROPERTIES props;
		m_exts->CopyPreset(m_reverb, props);
		m_exts->SetEaxReverb(m_effect, m_effectSlot, m_source, props);
	}

	sSoundReverbProperties CSoundSource::GetReverb()
	{
		return m_reverb;
	}

	void CSoundSource::GetReverb(sSoundReverbProperties& reverb)
	{
		reverb = m_reverb;
	}

};