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
		const auto& owner = m_owner.lock();

		if (owner != nullptr)
		{
			Quaternion quat_yaw = quat::CreateQuaternionFromNormal(owner->GetUp(), math::ToRadian(rate.y));
			Quaternion quat_pitch = quat::CreateQuaternionFromNormal(owner->GetRight(), math::ToRadian(rate.x));
			Quaternion quat_roll = quat::CreateQuaternionFromNormal(owner->GetForward(), math::ToRadian(rate.z));

			Quaternion rot, pawn_rot;
			rot *= quat_pitch * quat_roll * quat_yaw;

			owner->SetRotation(owner->GetRotation() * rot);
		}
	}
}
