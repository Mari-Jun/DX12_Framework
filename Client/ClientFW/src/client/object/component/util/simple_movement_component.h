#pragma once
#include "client/object/component/util/core/movement_component.h"

namespace client_fw
{
	class SimpleMovementComponent : public PawnMovementComponent
	{
	public:
		SimpleMovementComponent(const std::string& name = "simple movement component");
		virtual ~SimpleMovementComponent() = default;

		virtual void Update(float delta_time) override;

	protected:
		void UpdateVelocity(float delta_time);

	protected:
		float m_cur_speed = 0.0f;
		float m_max_speed = 1200.0f;
		float m_acceleration = 4000.0f;
		float m_deceleration = 8000.0f;

	public:
		float GetMaxSpeed() const { return m_max_speed; }
		void SetMaxSpeed(float max_speed) { m_max_speed = max_speed; }
		void SetAcceleration(float acceleration) { m_acceleration = acceleration; }
		void SetDeceleration(float deceleration) { m_deceleration = deceleration; }
	};
}



