#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
#include <client/object/actor/light.h>
#include "object/level/player_test_level.h"

namespace render_test
{
	PlayerTestLevel::PlayerTestLevel()
		: Level("player test level")
	{
	}

	bool PlayerTestLevel::Initialize()
	{
		auto police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/police.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });

	/*	police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ 300.0f, 0.0f, 1000.0f });

		police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/police.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ -300.0f, 0.0f, 1000.0f });*/

		/*police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ -600.0f, 0.0f, 1000.0f });

		police->SetScale(0.2f);*/

		auto d_light = CreateSPtr<DirectionalLight>();
		d_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		d_light->SetRotation(math::ToRadian(45.0f), math::ToRadian(90.0f), 0.0f);
		SpawnActor(d_light);

	/*	auto p_light = CreateSPtr<PointLight>(eMobilityState::kMovable, "Hello");
		p_light->SetLightColor(Vec3(400000.0f, 0.0f, 0.0f));
		p_light->SetPosition(Vec3(-2000.0f, 400.0f, 2000.0f));
		p_light->SetAttenuationRadius(2048.0f);
		SpawnActor(p_light);*/

		return true;
	}

	void PlayerTestLevel::Shutdown()
	{
	}

	void PlayerTestLevel::Update(float delta_time)
	{
		
		static float x = -300.0f, y = 0.0f, z = 1100.0f;
		static float time = 0.0f;

		static UINT count = 1;

		time += delta_time;

		if (time >= 0.016f && count <= 60000)
		{
			for (int i = 0; i < 40; ++i)
			{
				auto police = CreateSPtr<StaticMeshActor>(eMobilityState::kDestructible, "../Contents/police.obj");
				SpawnActor(police);
				police->SetPosition(Vec3{ x, y, z });
				police->SetScale(0.2f);
				++count;

				z += 100.0f;
			}

			if (z > 4000.0f)
			{
				x -= 100.0f;
				z = 1000.0f;
				if (x < -4300.0f)
				{
					x = -300.0f;
					y += 100.0f;
				}
			}

			time -= 0.016f;
		}

		/*if (count % 100 == 1)
			LOG_INFO(count);*/
	}

	std::vector<SPtr<VisualOctree>> PlayerTestLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(10000.0f));
		return visual_octrees;
	}

	std::vector<SPtr<CollisionOctree>> PlayerTestLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(10000.0f));
		return collision_octrees;
	}
}
