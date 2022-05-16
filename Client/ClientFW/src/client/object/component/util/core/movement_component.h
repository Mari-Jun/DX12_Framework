#pragma once
#include "client/object/component/core/component.h"

namespace client_fw
{
	class MovementComponent : public Component
	{
	protected:
		MovementComponent(const std::string& name = "movement component", int update_order = 10);
		virtual ~MovementComponent() = default;
	};

	class PawnMovementComponent : public MovementComponent
	{
	protected:
		PawnMovementComponent(const std::string& name = "pawn movement component");
		virtual ~PawnMovementComponent() = default;

		virtual void Update(float delta_time) override;

	protected:
		Vec3 m_move_input;
		Vec3 m_velocity;

	public:
		const Vec3& GetVelocity() const { return m_velocity; }
		void AddInputVector(const Vec3& vec);
	};
}


