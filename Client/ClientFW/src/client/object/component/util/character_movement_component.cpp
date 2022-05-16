#include "stdafx.h"
#include "client/object/component/util/character_movement_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	CharacterMovementComponent::CharacterMovementComponent(const std::string& name)
		: SimpleMovementComponent(name)
	{
	}

	void CharacterMovementComponent::Update(float delta_time)
	{
		UpdateVelocity(delta_time);

		if (math::NearZero(m_velocity.Length()) == false)
		{
			if (m_use_orient_rotation_to_movement)
				UpdateOrientRotation(delta_time);
		}

		PawnMovementComponent::Update(delta_time);
	}

	void CharacterMovementComponent::UpdateOrientRotation(float delta_time)
	{
		const auto& owner = m_owner.lock();

		Vec3 direction = Vec3(m_move_input.x, 0.0f, m_move_input.z);
		Vec3 forward = Vec3(owner->GetForward().x, 0.0f, owner->GetForward().z);
		if (math::NearZero(direction.LengthSq()) == false && math::NearZero(forward.LengthSq()) == false)
		{
			direction.Normalize();
			forward.Normalize();

			float cos_angle = vec3::Dot(direction, forward);
			cos_angle = std::clamp(cos_angle, -1.0f, 1.0f);
			float radian = acosf(cos_angle);

			if (math::NearZero(radian) == false)
			{
				Vec3 axis = vec3::Cross(direction, forward, true);
				if (math::NearZero(axis.LengthSq()))
					axis = vec3::AXIS_Y;

				//radian = radian / abs(radian) * std::clamp(m_rotating_speed * delta_time, 0.0f, abs(radian));

				Quaternion rotation = owner->GetRotation() * quat::CreateQuaternionFromNormal(axis, -radian);
				//owner->SetRotation(rotation);

				float t = m_rotating_speed / abs(radian) * delta_time;
				t = std::clamp(t, 0.0f, 1.0f);

				owner->SetRotation(quat::Slerp(owner->GetRotation(), rotation, t));
			}
		}
	}
}
