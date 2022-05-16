#include "stdafx.h"
#include "client/object/component/util/rotating_movement_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	RotatingMovementComponent::RotatingMovementComponent(const std::string& name)
		: MovementComponent(name)
		, m_rotating_rate(Vec3(0.0f, 180.0f, 0.0f))
	{
	}

	void RotatingMovementComponent::Update(float delta_time)
	{
		Vec3 rate = m_rotating_rate * delta_time;
		Quaternion rot = quat::CreateQuaternionFromRollPitchYaw(
			math::ToRadian(rate.x), math::ToRadian(rate.y), math::ToRadian(rate.z));
		
		const auto& owner = m_owner.lock();
		if (owner != nullptr)
			owner->SetRotation(owner->GetRotation() * rot);
	}
}
