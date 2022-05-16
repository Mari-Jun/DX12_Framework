#pragma once
#include "client/physics/collision/collisioner/collisioner.h"

namespace client_fw
{
	class RayCollisioner : public Collisioner
	{
	public:
		RayCollisioner();
		virtual ~RayCollisioner() = default;

		virtual bool CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other) override;
		virtual void BlockOtherComponent(const SPtr<SceneComponent>& other) override {}
		
	private:
		//
		// Custum 미사용시는
		// Origin = Component world position
		// Direction = Component world forward
		//
		std::function<Vec3()> m_origin_function;
		std::function<Vec3()> m_direction_function;
		std::function<void(float distance)> m_get_distance_function;

	public:
		void SetOriginFunction(std::function<Vec3()>&& function) { m_origin_function = function; }
		void SetDirectionFunction(std::function<Vec3()>&& function) { m_direction_function = function; }
		void SetDistanceFunction(std::function<void(float distance)>&& function) { m_get_distance_function = function; }
	};

}

