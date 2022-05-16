#include "stdafx.h"
#include "client/physics/collision/collisioner/sphere_collisioner.h"
#include "client/object/component/render/sphere_component.h"
#include "client/object/actor/core/actor.h"
#include "client/physics/collision/collision_responser.h"

namespace client_fw
{
	SphereCollisioner::SphereCollisioner()
		: Collisioner(eMeshCollisionType::kSphere)
	{
	}

	bool SphereCollisioner::CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other)
	{
		BSphere sphere1(GetOwner()->GetWorldPosition(), GetOwner()->GetOrientedBox()->GetExtents().x);

		switch (other->GetCollisioner()->GetMeshCollisionType())
		{
		case eMeshCollisionType::kStaticMesh:
		case eMeshCollisionType::kBox:
			return sphere1.Intersects(*other->GetOrientedBox());
		case eMeshCollisionType::kSphere:
		{
			BSphere sphere2(other->GetWorldPosition(), other->GetOrientedBox()->GetExtents().x);
			return sphere1.Intersects(sphere2);
		}
		case eMeshCollisionType::kRay:
			return other->GetCollisioner()->CheckCollisionWithOtherComponent(GetOwner());
		default:
			break;
		}
		return false;
	}

	void SphereCollisioner::BlockOtherComponent(const SPtr<SceneComponent>& other)
	{
		if (other->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
		{
		}
		else
		{
			

			switch (other->GetCollisioner()->GetMeshCollisionType())
			{
			case eMeshCollisionType::kStaticMesh:
			case eMeshCollisionType::kBox:
			{
				BSphere sphere(GetOwner()->GetWorldPosition(), GetOwner()->GetOrientedBox()->GetExtents().x);
				CollisionResponser::BlockBetweenSphereAndBox(GetOwner(), sphere, *other->GetOrientedBox());
				break;
			}
			case eMeshCollisionType::kSphere:
			{
				BSphere sphere(GetOwner()->GetWorldPosition(), GetOwner()->GetOrientedBox()->GetExtents().x);
				BSphere other_sphere(other->GetWorldPosition(), other->GetOrientedBox()->GetExtents().x);
				CollisionResponser::BlockBetweenSphereAndSphere(GetOwner(), sphere, other_sphere);
			}
			default:
				break;
			}
		}
	}
}
