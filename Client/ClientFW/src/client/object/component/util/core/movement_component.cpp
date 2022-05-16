#include "stdafx.h"
#include "client/object/component/util/core/movement_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	MovementComponent::MovementComponent(const std::string& name, int update_order)
		: Component(name, update_order)
	{
	}

	PawnMovementComponent::PawnMovementComponent(const std::string& name)
		: MovementComponent(name)
	{
	}

	void PawnMovementComponent::Update(float delta_time)
	{
		if (math::NearZero(m_velocity.Length()) == false)
		{
			const auto& owner = m_owner.lock();
			if (owner != nullptr)
			{
				Vec3 pos = owner->GetPosition();
				pos += m_velocity * delta_time;
				owner->SetPosition(pos);
			}
		}

		m_move_input = vec3::ZERO;
	}

	void PawnMovementComponent::AddInputVector(const Vec3& vec)
	{
		m_move_input += vec;
	}
}
