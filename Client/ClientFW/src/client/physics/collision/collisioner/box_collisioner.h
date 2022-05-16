#pragma once
#include "client/physics/collision/collisioner/collisioner.h"

namespace client_fw
{
	class BoxCollisioner : public Collisioner
	{
	public:
		BoxCollisioner();
		virtual ~BoxCollisioner() = default;

		virtual bool CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other) override;
		virtual void BlockOtherComponent(const SPtr<SceneComponent>& other) override;
	};
}
