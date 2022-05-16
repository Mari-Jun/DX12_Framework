#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/util/octree/octree.h>
#include <client/object/component/render/billboard_component.h>
#include "object/level/event_test_level.h"
#include "object/actor/rotating_cube.h"
#include "object/actor/move_cube.h"
#include "object/actor/billboard_actor.h"
#include "object/actor/material_billboard_actor.h"
#include "object/ui/event_test_ui_layer.h"

namespace event_test
{
	EventTestLevel::EventTestLevel()
		: Level("event test level")
	{
	}

	bool EventTestLevel::Initialize()
	{
		auto sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		SpawnActor(sphere);
		sphere->SetPosition(Vec3{ 300.0f, 0.0f, 1000.0f });
		//sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		//SpawnActor(sphere);
		//sphere->SetPosition(Vec3{ 330.0f, 0.0f, 1000.0f });
		//sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		//SpawnActor(sphere);
		//sphere->SetPosition(Vec3{ 360.0f, 0.0f, 1000.0f });
		//sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		//SpawnActor(sphere);
		//sphere->SetPosition(Vec3{ 390.0f, 0.0f, 1000.0f });
		//sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/sphere.obj");
		//SpawnActor(sphere);
		//sphere->SetPosition(Vec3{ 300.0f, 30.0f, 1000.0f });
		sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/Castle/SiegeRam.obj");
		SpawnActor(sphere);
		sphere->SetPosition(Vec3{ -1000.0f, 60.0f, 1000.0f });
		sphere->SetScale(0.5f);

		sphere = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/penguin.obj");
		SpawnActor(sphere);
		sphere->SetPosition(Vec3{ 1500.0f, 0.0f, 1500.0f });
		sphere->SetScale(200.0f);

		auto cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });

		cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ -500.0f, 0.0f, 800.0f });

		auto move_cube = CreateSPtr<MoveCube>();
		SpawnActor(move_cube);
		move_cube->SetPosition(Vec3{ 0.0f, 0.0f, 2500.0f });
		m_spawn_pos = Vec3(0.0f, 0.0f, 2600.0f);

	/*	auto police = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/police.obj");
		SpawnActor(police);
		police->SetPosition(Vec3{ 0.0f, -110.0f, 650.0f });*/

		auto test_billboard = CreateSPtr<BillboardActor>(eMobilityState::kStatic, "../Contents/Tree_02.dds", Vec2(200.0f, 400.0f), false);
		SpawnActor(test_billboard);
		test_billboard->SetPosition(Vec3(0.0f, 0.0f, 500.0f));

		auto event_test_ui_layer = CreateSPtr<EventTestUILayer>();
		RegisterUILayer(event_test_ui_layer);

		Input::SetInputMode(eInputMode::kGameOnly);

		RegisterPressedEvent("spawn movable actor", { EventKeyInfo{eKey::kP} },
			[this]()->bool {
				auto test_billboard = CreateSPtr<BillboardActor>(eMobilityState::kDestructible,
					"../Contents/Tree_02.dds", Vec2(200.0f, 400.0f), m_move_cube_queue.size() % 2);
				SpawnActor(test_billboard);
				test_billboard->SetPosition(m_spawn_pos);
				m_spawn_pos += Vec3(0.0f, 0.0f, 100.0f);
				m_move_cube_queue.push(test_billboard);
				return true;
			});

		RegisterPressedEvent("kill movable actor", { EventKeyInfo{eKey::kO} },
			[this]()->bool {
				if (m_move_cube_queue.empty() == false)
				{
					m_move_cube_queue.front()->SetActorState(eActorState::kDead);
					m_move_cube_queue.pop();
				}
				return true;
			});

		RegisterPressedEvent("spawn material billboard", { EventKeyInfo{eKey::kL} },
			[this]()->bool {
				auto test_mat_billboard = CreateSPtr<MaterialBillboardActor>(eMobilityState::kDestructible, 
					"../Contents/Castle/SiegeRam.mtl", "SiegeRam_LOD0", Vec2(100.0f, 100.0f), m_mat_bb_queue.size() % 2);
				SpawnActor(test_mat_billboard);
				test_mat_billboard->SetPosition(m_spawn_pos);
				m_spawn_pos += Vec3(0.0f, 0.0f, 100.0f);
				m_mat_bb_queue.push(test_mat_billboard);
				return true;
			});

		RegisterPressedEvent("kill mat movable actor", { EventKeyInfo{eKey::kK} },
			[this]()->bool {
				if (m_mat_bb_queue.empty() == false)
				{
					m_mat_bb_queue.front()->SetActorState(eActorState::kDead);
					m_mat_bb_queue.pop();
				}
				return true;
			});

		return true;
	}

	void EventTestLevel::Shutdown()
	{
		Input::SetInputMode(eInputMode::kUIOnly);
	}

	void EventTestLevel::Update(float delta_time)
	{
	}

	std::vector<SPtr<VisualOctree>> EventTestLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(10000.0f));
		return visual_octrees;
	}

	std::vector<SPtr<CollisionOctree>> EventTestLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(10000.0f));
		return collision_octrees;
	}
}