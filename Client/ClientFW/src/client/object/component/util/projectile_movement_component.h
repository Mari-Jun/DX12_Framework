#pragma once
#include "client/object/component/util/core/movement_component.h"

namespace client_fw
{
	class ProjectileMovementComponent :	public MovementComponent
	{
	public:
		ProjectileMovementComponent(const std::string& name = "projectile movement component");
		virtual ~ProjectileMovementComponent() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

	protected:
		Vec3 m_velocity;
		float m_initial_speed = 0.0f;

	public:
		const Vec3& GetVelocity() const { return m_velocity; }
		float GetInitialSpeed() const { return m_initial_speed; }
		void SetInitialSpeed(float speed) { m_initial_speed = speed; }
	};
}


