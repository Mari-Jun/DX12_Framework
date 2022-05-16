#include "stdafx.h"
#include "client/object/component/util/projectile_movement_component.h"
#include "client/object/actor/core/actor.h"
#include "client/physics/core/actor_physics_manager.h"

namespace client_fw
{
	ProjectileMovementComponent::ProjectileMovementComponent(const std::string& name)
		: MovementComponent(name)
	{
	}

	bool ProjectileMovementComponent::Initialize()
	{
		bool ret = MovementComponent::Initialize();

		m_velocity = m_owner.lock()->GetForward() * m_initial_speed;

		return ret;
	}

	void ProjectileMovementComponent::Update(float delta_time)
	{
		const auto& owner = m_owner.lock();

		if (owner != nullptr && owner->GetPhysicsManager()->IsActive())
		{
			Vec3 pos = owner->GetPosition();
			pos += m_velocity * delta_time;
			owner->SetPosition(pos);
		}
	}
}
