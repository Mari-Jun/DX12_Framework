#pragma once
#include "client/physics/collision/collisioner/collisioner.h"

namespace client_fw
{
	class SphereCollisioner : public Collisioner
	{
	public:
		SphereCollisioner();
		virtual ~SphereCollisioner() = default;

		virtual bool CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other) override;
		virtual void BlockOtherComponent(const SPtr<SceneComponent>& other) override;
	};
}


