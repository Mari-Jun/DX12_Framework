#pragma once

namespace client_fw
{
	class SceneComponent;
	class BOrientedBox;
	class BSphere;

	class CollisionResponser
	{
	public:
		static void BlockBetweenBoxAndBox(const SPtr<SceneComponent>& owner, 
			BOrientedBox box, BOrientedBox other_box);

		static void BlockBetweenSphereAndBox(const SPtr<SceneComponent>& owner,
			BSphere sphere, BOrientedBox box);

		static void BlockBetweenBoxAndSphere(const SPtr<SceneComponent>& owner,
			BOrientedBox box, BSphere sphere);

		static void BlockBetweenSphereAndSphere(const SPtr<SceneComponent>& owner,
			BSphere sphere, BSphere other_sphere);

	private:
		template <class Bounding, class OtherBounding>
		static Vec3 BlockTimeSlice(const SPtr<SceneComponent>& owner,
			Bounding bounding, OtherBounding other_bounding)
		{
			Vec3 p_pos = owner->GetWorldPreviousPosition();
			Vec3 velocity = owner->GetWorldPosition() - p_pos;

			Vec3 new_pos = p_pos;

			if (velocity != vec3::ZERO)
			{
				Vec3 time{ -0.01f, -0.01f, -0.01f };

				float t = 1.0f;
				for (UINT i = 0; i < 4; ++i)
				{
					if (time.x < 0.0f)
					{
						bounding.SetCenter(p_pos + Vec3(velocity.x * t, 0.0f, 0.0f));
						if (bounding.Intersects(other_bounding) == false)
							time.x = t;
					}

					if (time.y < 0.0f)
					{
						bounding.SetCenter(p_pos + Vec3(0.0f, velocity.y * t, 0.0f));
						if (bounding.Intersects(other_bounding) == false)
						{
							time.y = t;
						}
					}

					if (time.z < 0.0f)
					{
						bounding.SetCenter(p_pos + Vec3(0.0f, 0.0f, velocity.z * t));
						if (bounding.Intersects(other_bounding) == false)
							time.z = t;
					}
					t *= 0.5f;
				}

				owner->GetOwner().lock()->GetPhysicsManager()->ResetGravitySpeed();

				bounding.SetCenter(p_pos + velocity * time);
				if (bounding.Intersects(other_bounding) == false)
					new_pos = p_pos + velocity * time;
				else
					new_pos = p_pos;
			}

			return new_pos;
		}
};
}


