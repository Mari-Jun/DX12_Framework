#include "stdafx.h"
#include "client/physics/collision/collision_responser.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/core/scene_component.h"
#include "client/physics/core/bounding_mesh.h"
#include "client/physics/core/actor_physics_manager.h"

namespace client_fw
{
	void CollisionResponser::BlockBetweenBoxAndBox(const SPtr<SceneComponent>& owner,
		BOrientedBox box, BOrientedBox other_box)
	{
		Vec3 new_pos = BlockTimeSlice(owner, box, other_box);

		if (new_pos == owner->GetWorldPreviousPosition())
		{
			const auto& corners = box.GetCorners();
			float distance = 0.0f, length = box.GetExtents().Length();
			Vec3 direction;
			for (const auto& corner : corners)
			{
				if (other_box.Contains(corner) != ContainmentType::DISJOINT)
				{
					float dis;
					Vec3 dir = vec3::Normalize(corner - box.GetCenter());
					if (other_box.Intersects(box.GetCenter(), dir, dis) &&
						distance <= fabs(dis) && fabs(dis) < length)
					{
						distance = dis;
						direction = dir;
					}
				}
			}

			if (distance == 0.0f)
			{
				for (const auto& corner : corners)
				{
					float dis;
					Vec3 dir = vec3::Normalize(box.GetCenter() - corner);
					if (other_box.Intersects(corner, dir, dis) &&
						distance <= fabs(dis) && fabs(dis) < length)
					{
						distance = dis;
						direction = dir;
					}
				}

				new_pos += direction * distance;
			}
			else
			{
				new_pos -= direction * (length - distance);
			}
		}

		owner->GetOwner().lock()->SetPosition(new_pos);
		owner->GetOwner().lock()->UpdateWorldMatrix();
	}

	void CollisionResponser::BlockBetweenSphereAndBox(const SPtr<SceneComponent>& owner,
		BSphere sphere, BOrientedBox box)
	{
		Vec3 new_pos = BlockTimeSlice(owner, sphere, box);
		owner->GetOwner().lock()->SetPosition(new_pos);
		owner->GetOwner().lock()->UpdateWorldMatrix();
	}

	void CollisionResponser::BlockBetweenBoxAndSphere(const SPtr<SceneComponent>& owner,
		BOrientedBox box, BSphere sphere)
	{
		Vec3 new_pos = BlockTimeSlice(owner, box, sphere);
		owner->GetOwner().lock()->SetPosition(new_pos);
		owner->GetOwner().lock()->UpdateWorldMatrix();
	}

	void CollisionResponser::BlockBetweenSphereAndSphere(const SPtr<SceneComponent>& owner,
		BSphere sphere, BSphere other_sphere)
	{
		Vec3 new_pos = BlockTimeSlice(owner, sphere, other_sphere);
		owner->GetOwner().lock()->SetPosition(new_pos);
		owner->GetOwner().lock()->UpdateWorldMatrix();
	}
}
