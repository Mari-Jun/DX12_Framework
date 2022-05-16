#include "stdafx.h"
#include "client/physics/collision/collisioner/collisioner.h"
#include "client/object/component/core/scene_component.h"


namespace client_fw
{
	Collisioner::Collisioner(eMeshCollisionType type)
		: m_type(type)
	{
	}

	void Collisioner::SetCollisionInfo(bool is_collision, bool is_blocking, bool generate_collision_event)
	{
		m_collision_info.is_collision = is_collision;
		m_collision_info.is_blocking = is_blocking;
		m_collision_info.generate_collision_event = generate_collision_event;
	}

	void Collisioner::SetCollisionInfo(bool is_collision, bool is_blocking,
		std::string&& collision_type, std::set<std::string>&& collisionable_types, bool generate_collision_event)
	{
		SetCollisionInfo(is_collision, is_blocking, generate_collision_event);
		m_collision_info.collision_type = std::move(collision_type);
		m_collision_info.collisionable_types = std::move(collisionable_types);
	}
}
