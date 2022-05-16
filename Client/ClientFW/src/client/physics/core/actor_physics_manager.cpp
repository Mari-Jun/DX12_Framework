#include "stdafx.h"
#include "client/physics/core/actor_physics_manager.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	ActorPhysicsManager::ActorPhysicsManager()
	{
	}

	void ActorPhysicsManager::Update(float delta_time)
	{
		if (m_owner.expired() == false)
		{
			UpdateGravity(delta_time);
		}
	}

	void ActorPhysicsManager::UpdateGravity(float delta_time)
	{
		const auto& owner = m_owner.lock();

		m_gravity_speed += s_gravity_acceleration * delta_time;
		m_gravity_speed = min(m_gravity_speed, s_max_gravity_speed);

		Vec3 pos = owner->GetPosition();
		pos.y -= m_gravity_speed * delta_time;
		owner->SetPosition(pos);
	}
}
