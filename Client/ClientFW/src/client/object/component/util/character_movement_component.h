#pragma once
#include "client/object/component/util/simple_movement_component.h"

namespace client_fw
{
	class CharacterMovementComponent : public SimpleMovementComponent
	{
	public:
		CharacterMovementComponent(const std::string& name = "character movement component");
		virtual ~CharacterMovementComponent() = default;

		virtual void Update(float delta_time) override;

	protected:
		virtual void UpdateOrientRotation(float delta_time);

	protected:
		bool m_use_orient_rotation_to_movement = false;
		float m_rotating_speed = math::ToRadian(360.0f);

	public:
		bool IsUseOrientRotationToMovement() const { return m_use_orient_rotation_to_movement; }
		void UseOrientRotationToMovement(bool use) { m_use_orient_rotation_to_movement = use; }
		float GetRotatingSpeed() const { return m_rotating_speed; }
		void SetRotatingSpeed(float degrees) { m_rotating_speed = math::ToRadian(degrees); }
	};
}


