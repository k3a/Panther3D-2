/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kalda
*/
#pragma once


#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>
#include <AL/EFX-Util.h>
#include <AL/efx-creative.h>
#include "SoundReverb.h"

#include "types.h"

namespace P3D
{

	class COALExtProvider
	{
	public:
		COALExtProvider();
		~COALExtProvider();

		bool ExtPresent(ALCdevice* device, const char* name);
		bool Create(ALCdevice* device, ALCcontext*& context);
		ALboolean CreateAuxEffectSlot(ALuint *puiAuxEffectSlot);
		ALboolean CreateEffect(ALuint *puiEffect, ALenum eEffectType);
		ALvoid PlaySource(ALuint uiSource);
		ALboolean SetEFXEAXReverbProperties(EFXEAXREVERBPROPERTIES *pEFXEAXReverb, ALuint uiEffect);
		void CopyPreset(sSoundReverbProperties& input, EFXEAXREVERBPROPERTIES& output);
		bool IsEfxSupported();
		void SetEaxReverb(ALuint effect, ALuint slot, ALuint source, EFXEAXREVERBPROPERTIES& output);

	private:
		ALint m_maxAux;
		bool m_initialized;
		
		// Effect objects
		LPALGENEFFECTS alGenEffects;
		LPALDELETEEFFECTS alDeleteEffects;
		LPALISEFFECT alIsEffect;
		LPALEFFECTI alEffecti;
		LPALEFFECTIV alEffectiv;
		LPALEFFECTF alEffectf;
		LPALEFFECTFV alEffectfv;
		LPALGETEFFECTI alGetEffecti;
		LPALGETEFFECTIV alGetEffectiv;
		LPALGETEFFECTF alGetEffectf;
		LPALGETEFFECTFV alGetEffectfv;
		
		// Filter objects
		LPALGENFILTERS alGenFilters;
		LPALDELETEFILTERS alDeleteFilters;
		LPALISFILTER alIsFilter;
		LPALFILTERI alFilteri;
		LPALFILTERIV alFilteriv;
		LPALFILTERF alFilterf;
		LPALFILTERFV alFilterfv;
		LPALGETFILTERI alGetFilteri;
		LPALGETFILTERIV alGetFilteriv;
		LPALGETFILTERF alGetFilterf;
		LPALGETFILTERFV alGetFilterfv;
		
		// Auxiliary slot object
		LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
		LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
		LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
		LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
		LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
		LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
		LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
		LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
		LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
		LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
		LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;
	};

}