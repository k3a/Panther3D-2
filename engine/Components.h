/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 11/21/2009 File created - Petr
*/

#pragma once

#include "types.h"
#include "Component.h"

namespace P3D{

	class CTransformComponent : public CComponent
	{
	public:
		CTransformComponent()
		{
			m_position = Vec3(0.0f, 0.0f, 0.0f);
			m_rotation = Vec3(0.0f, 0.0f, 0.0f);
			m_scale = Vec3(1.0f, 1.0f, 1.0f);
			m_changed = false;
		}
		void SetPosition(const Vec3& position)
		{
			m_position = position;
			m_changed = true;
		}
		void SetRotation(const Vec3& rotation)
		{
			m_rotation = rotation;
			m_changed = true;
		}
		void SetScale(const Vec3& scale)
		{
			m_scale = scale;
			m_changed = true;
		}

		const Str& GetType() { return m_type; }
		bool IsUpdated() { return m_changed; }

		void Update()
		{
			m_changed = false;
		}
		//TODO: reference counter

	private:
		Vec3 m_position;
		Vec3 m_rotation;
		Vec3 m_scale;

		bool m_changed;

		static const Str m_type;
	};

	//TODO: create some REGISTER_COMPONENT macro (if needed)
	const Str CTransformComponent::m_type = _W("transform");

// 	class CGraphicsComponent
// 	{
// 	public:
// 		CGraphicsComponent(CTransformComponent* transform);
// 		bool SetModel(const Str &name);
// 		//set texture, material
// 	private:
// 		CTransformComponent* m_pTransform;
// 	};
// 
// 	class CPhysicsComponent
// 	{
// 	public:
// 		CPhysicsComponent(CTransformComponent* transform);
// 	private:
// 		CTransformComponent* m_pTransform;
// 	};

	class CCameraComponent : public CComponent
	{
	public:
		CCameraComponent(CTransformComponent* transform)
		{
			m_pTransform = transform;
			//inicialize variables...
		}

		void SetNearViewPlane(float distance);
		//.... other methods

	private:
		CTransformComponent* m_pTransform;
		bool m_ortographic;
		float m_fov;
		float m_nearViewPlane;
		float m_farViewPlane;
	};

	class CScriptComponent
	{
	public:
		void SetScript(const Str& script);
	};

	

};//ns