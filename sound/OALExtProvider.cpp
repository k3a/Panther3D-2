/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kalda
*/
#include "precompiled.h"

#include "OALExtProvider.h"
#include "sound.h"


namespace P3D
{
	COALExtProvider::COALExtProvider()
	{
		m_maxAux = 0;
		m_initialized = false;
	}

	COALExtProvider::~COALExtProvider()
	{

	}

	bool COALExtProvider::ExtPresent(ALCdevice* device, const char* name)
	{
		return (alcIsExtensionPresent(device, name) == AL_TRUE) ? true : false;
	}

	bool COALExtProvider::Create(ALCdevice* device, ALCcontext*& context)
	{
		ALint attribs[4] = { 0 };
		bool efx = false;

		//Try to load effect extension
		if(ExtPresent(device, ALC_EXT_EFX_NAME))
		{
			attribs[0] = ALC_MAX_AUXILIARY_SENDS;
			attribs[1] = 4;
			context = alcCreateContext(device, attribs);
			efx = true;
			CON(MSG_INFO, _W(" Device supports Effect Extension"));
		}
		else
		{
			CON(MSG_INFO, _W(" Device doesn't support Effect Extension"));
			context = alcCreateContext(device, NULL);
		}

		if(context == NULL)
			return false;

		alcMakeContextCurrent(context);

		alcGetIntegerv(device, ALC_MAX_AUXILIARY_SENDS, 1, &m_maxAux);
		CON(MSG_INFO, _W(" Device supports %d Aux Sends per Source"), m_maxAux);

		if(efx)
		{
			alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
			alDeleteEffects = (LPALDELETEEFFECTS )alGetProcAddress("alDeleteEffects");
			alIsEffect = (LPALISEFFECT )alGetProcAddress("alIsEffect");
			alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
			alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
			alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
			alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
			alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
			alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
			alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
			alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
			alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
			alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
			alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
			alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
			alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
			alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
			alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
			alGetFilteri = (LPALGETFILTERI )alGetProcAddress("alGetFilteri");
			alGetFilteriv= (LPALGETFILTERIV )alGetProcAddress("alGetFilteriv");
			alGetFilterf = (LPALGETFILTERF )alGetProcAddress("alGetFilterf");
			alGetFilterfv= (LPALGETFILTERFV )alGetProcAddress("alGetFilterfv");
			alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
			alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
			alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
			alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
			alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
			alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
			alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
			alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
			alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
			alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
			alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");
				
			if(!(alGenEffects && alDeleteEffects && alIsEffect && alEffecti && alEffectiv &&	alEffectf &&
			alEffectfv && alGetEffecti && alGetEffectiv && alGetEffectf && alGetEffectfv &&	alGenFilters &&
			alDeleteFilters && alIsFilter && alFilteri && alFilteriv &&	alFilterf && alFilterfv &&
			alGetFilteri &&	alGetFilteriv && alGetFilterf && alGetFilterfv && alGenAuxiliaryEffectSlots &&
			alDeleteAuxiliaryEffectSlots &&	alIsAuxiliaryEffectSlot && alAuxiliaryEffectSloti &&
			alAuxiliaryEffectSlotiv && alAuxiliaryEffectSlotf && alAuxiliaryEffectSlotfv &&
			alGetAuxiliaryEffectSloti && alGetAuxiliaryEffectSlotiv && alGetAuxiliaryEffectSlotf &&
			alGetAuxiliaryEffectSlotfv))
			{
				CON(MSG_INFO, _W(" Can't load EFX functions!"));
				efx = false;
			}
		}

		m_initialized = efx;
		return efx;
	}

	ALboolean COALExtProvider::CreateAuxEffectSlot(ALuint *puiAuxEffectSlot)
	{
		if(!m_initialized) return AL_FALSE;

		ALboolean bReturn = AL_FALSE;
	
		// Clear AL Error state
		alGetError();
	
		// Generate an Auxiliary Effect Slot
		alGenAuxiliaryEffectSlots(1, puiAuxEffectSlot);
		if (alGetError() == AL_NO_ERROR)
			bReturn = AL_TRUE;
	
		return bReturn;
	}
	ALboolean COALExtProvider::CreateEffect(ALuint *puiEffect, ALenum eEffectType)
	{
		if(!m_initialized) return AL_FALSE;

		ALboolean bReturn = AL_FALSE;
	
		if (puiEffect)
		{
			// Clear AL Error State
			alGetError();
	
			// Generate an Effect
			alGenEffects(1, puiEffect);
			if (alGetError() == AL_NO_ERROR)
			{
				// Set the Effect Type
				alEffecti(*puiEffect, AL_EFFECT_TYPE, eEffectType);
				if (alGetError() == AL_NO_ERROR)
					bReturn = AL_TRUE;
				else
					alDeleteEffects(1, puiEffect);
			}
		}
	
		return bReturn;
	}
	ALboolean COALExtProvider::SetEFXEAXReverbProperties(EFXEAXREVERBPROPERTIES *pEFXEAXReverb, ALuint uiEffect)
	{
		if(!m_initialized) return AL_FALSE;

		ALboolean bReturn = AL_FALSE;
	
		if (pEFXEAXReverb)
		{
			// Clear AL Error code
			alGetError();
	
			alEffectf(uiEffect, AL_EAXREVERB_DENSITY, pEFXEAXReverb->flDensity);
			alEffectf(uiEffect, AL_EAXREVERB_DIFFUSION, pEFXEAXReverb->flDiffusion);
			alEffectf(uiEffect, AL_EAXREVERB_GAIN, pEFXEAXReverb->flGain);
			alEffectf(uiEffect, AL_EAXREVERB_GAINHF, pEFXEAXReverb->flGainHF);
			alEffectf(uiEffect, AL_EAXREVERB_GAINLF, pEFXEAXReverb->flGainLF);
			alEffectf(uiEffect, AL_EAXREVERB_DECAY_TIME, pEFXEAXReverb->flDecayTime);
			alEffectf(uiEffect, AL_EAXREVERB_DECAY_HFRATIO, pEFXEAXReverb->flDecayHFRatio);
			alEffectf(uiEffect, AL_EAXREVERB_DECAY_LFRATIO, pEFXEAXReverb->flDecayLFRatio);
			alEffectf(uiEffect, AL_EAXREVERB_REFLECTIONS_GAIN, pEFXEAXReverb->flReflectionsGain);
			alEffectf(uiEffect, AL_EAXREVERB_REFLECTIONS_DELAY, pEFXEAXReverb->flReflectionsDelay);
			alEffectfv(uiEffect, AL_EAXREVERB_REFLECTIONS_PAN, pEFXEAXReverb->flReflectionsPan);
			alEffectf(uiEffect, AL_EAXREVERB_LATE_REVERB_GAIN, pEFXEAXReverb->flLateReverbGain);
			alEffectf(uiEffect, AL_EAXREVERB_LATE_REVERB_DELAY, pEFXEAXReverb->flLateReverbDelay);
			alEffectfv(uiEffect, AL_EAXREVERB_LATE_REVERB_PAN, pEFXEAXReverb->flLateReverbPan);
			alEffectf(uiEffect, AL_EAXREVERB_ECHO_TIME, pEFXEAXReverb->flEchoTime);
			alEffectf(uiEffect, AL_EAXREVERB_ECHO_DEPTH, pEFXEAXReverb->flEchoDepth);
			alEffectf(uiEffect, AL_EAXREVERB_MODULATION_TIME, pEFXEAXReverb->flModulationTime);
			alEffectf(uiEffect, AL_EAXREVERB_MODULATION_DEPTH, pEFXEAXReverb->flModulationDepth);
			alEffectf(uiEffect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, pEFXEAXReverb->flAirAbsorptionGainHF);
			alEffectf(uiEffect, AL_EAXREVERB_HFREFERENCE, pEFXEAXReverb->flHFReference);
			alEffectf(uiEffect, AL_EAXREVERB_LFREFERENCE, pEFXEAXReverb->flLFReference);
			alEffectf(uiEffect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, pEFXEAXReverb->flRoomRolloffFactor);
			alEffecti(uiEffect, AL_EAXREVERB_DECAY_HFLIMIT, pEFXEAXReverb->iDecayHFLimit);
	
			if (alGetError() == AL_NO_ERROR)
				bReturn = AL_TRUE;
		}
	
		return bReturn;
	}
	void COALExtProvider::CopyPreset(sSoundReverbProperties& input, EFXEAXREVERBPROPERTIES& output)
	{
		EAXREVERBPROPERTIES props;
		memcpy(&props, &input, sizeof(EAXREVERBPROPERTIES));
		ConvertReverbParameters(&props, &output);
	}

	bool COALExtProvider::IsEfxSupported()
	{
		return m_initialized;
	}
	void COALExtProvider::SetEaxReverb(ALuint effect, ALuint slot, ALuint source, EFXEAXREVERBPROPERTIES& output)
	{
		if(!m_initialized)
			return;

		if(!SetEFXEAXReverbProperties(&output, effect))
			return;

		alAuxiliaryEffectSloti(slot, AL_EFFECTSLOT_EFFECT, effect);
		alSource3i(source, AL_AUXILIARY_SEND_FILTER, slot, 0, AL_FILTER_NULL);
	}
}