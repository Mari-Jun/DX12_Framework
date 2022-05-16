#include "stdafx.h"
#include "client/physics/collision/collisioner/ray_collisioner.h"
#include "client/object/component/core/scene_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	RayCollisioner::RayCollisioner()
		: Collisioner(eMeshCollisionType::kRay)
	{
	}

	bool RayCollisioner::CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other)
	{
		Vec3 origin = (m_origin_function == nullptr) ? GetOwner()->GetWorldPosition() : m_origin_function();
		Vec3 direction = (m_direction_function == nullptr) ? GetOwner()->GetWorldForward() : m_direction_function();

		float distance = 0.0f;

		switch (other->GetCollisioner()->GetMeshCollisionType())
		{
		case eMeshCollisionType::kStaticMesh:
		case eMeshCollisionType::kBox:
		case eMeshCollisionType::kSphere:
		{
			bool ret = other->GetOrientedBox()->Intersects(origin, direction, distance);
			if (ret)
				m_get_distance_function(distance);
			return ret;
		}
		default:
			break;
		}
		return false;
	}
}
