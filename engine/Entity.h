/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 11/23/2009 File created - Petr
*/

#pragma once

#include "types.h"
#include "Components.h"

namespace P3D{

	class CEntity
	{
	public:
		CEntity();
		~CEntity();
		void SetName(const Str& name) { m_name = name; };
		//void SetEnabled(bool enable);
		bool AddComponent(CComponent* component);
		CComponent* GetComponent(const Str& type);

	private:
		Str m_name;
		tArray<CComponent*> m_components;
		static UINT m_numCreatedEntities;
	};

	UINT CEntity::m_numCreatedEntities = 0;

}//ns