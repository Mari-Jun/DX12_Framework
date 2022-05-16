#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/static_mesh_actor.h>
#include "object/level/physics_test_level.h"
#include "object/actor/gravity_actor.h"
#include "object/actor/move_cube.h"

namespace event_test
{
	PhysicsTestLevel::PhysicsTestLevel()
		: Level("physics test level")
	{
	}

	bool PhysicsTestLevel::Initialize()
	{
		auto plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(0.0f, -1000.0f, 0.0f));
		plane->SetScale(500.0f);

		auto cube = CreateSPtr<GravityActor>("../Contents/cube.obj");
		SpawnActor(cube);
		cube->SetPosition(Vec3(0.0f, 1000.0f, 2500.0f));

		auto move_cube = CreateSPtr<MoveCube>();
		SpawnActor(move_cube);
		move_cube->SetPosition(Vec3{ 0.0f, 3000.0f, 2500.0f });

		return true;
	}

	void PhysicsTestLevel::Shutdown()
	{
	}

	void PhysicsTestLevel::Update(float delta_time)
	{
	}
}