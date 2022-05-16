#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
#include <client/object/component/render/billboard_component.h>
#include "object/level/frame_test_level.h"
#include "object/actor/rotating_cube.h"
#include "object/actor/move_cube.h"
#include "object/actor/billboard_actor.h"
#include "object/actor/material_billboard_actor.h"
#include "object/ui/event_test_ui_layer.h"

namespace event_test
{
	FrameTestLevel::FrameTestLevel()
		: Level("frame test level")
	{
	}

	bool FrameTestLevel::Initialize()
	{
		return true;
	}

	void FrameTestLevel::Shutdown()
	{
	}

	void FrameTestLevel::Update(float delta_time)
	{
		static float x = 3000.0f, y = 0.0f, z = -3000.0f;
		static float time = 0.0f;

		static UINT count = 1;

		time += delta_time;

		if (time >= 0.016f && count <= 200)
		{
			for (int i = 0; i < 60; ++i)
			{
				auto test_billboard = CreateSPtr<BillboardActor>(eMobilityState::kDestructible, "../Contents/Tree_02.dds", Vec2(200.0f, 400.0f), false);
				SpawnActor(test_billboard);
				test_billboard->SetPosition(Vec3{ x, y, z });
				test_billboard->SetScale(0.2f);

				++count;

				z += 100.0f;
			}

			x -= 100.0f;
			z = -3000.0f;
			if (x < -3000.0f)
			{
				x = 3000.0f;
				y += 100.0f;
			}

			time -= 0.016f;
		}
	}

	std::vector<SPtr<VisualOctree>> FrameTestLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(6000.0f, vec3::ZERO, 3));
		return visual_octrees;
	}

	std::vector<SPtr<CollisionOctree>> FrameTestLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(10000.0f));
		return collision_octrees;
	}
}