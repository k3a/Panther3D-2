/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 11/23/2009 File created - Petr
*/

#include "precompiled.h"

#include "Engine.h"
#include "Entity.h"

namespace P3D
{

	CEntity::CEntity()
	{
		//set default name
		m_name << _W("Entity_") << m_numCreatedEntities;
		m_numCreatedEntities++;
	}

	CEntity::~CEntity()
	{

	}

	bool CEntity::AddComponent(CComponent *component)
	{
		//TODO: each entity should be of a unique type
		m_components.AddEx(component);
		return true;
	}

	CComponent* CEntity::GetComponent(const Str& type)
	{
		for(UINT i=0; i<m_components.Size(); i++)
			if(m_components[i]->GetType() == type) return m_components[i]; 	

		return NULL;
	}

}//ns