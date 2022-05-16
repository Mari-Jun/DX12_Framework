#pragma once
#include "client/object/component/util/core/movement_component.h"

namespace client_fw
{
	class RotatingMovementComponent : public MovementComponent
	{
	public:
		RotatingMovementComponent(const std::string& name = "rotating movement component");
		virtual ~RotatingMovementComponent() = default;

		virtual void Update(float delta_time) override;

	protected:
		Vec3 m_rotating_rate;

	public:
		const Vec3& GetRotatingRate() const { return m_rotating_rate; }
		void SetRotatingRate(const Vec3& rotating_rate) { m_rotating_rate = rotating_rate; }
	};
}



