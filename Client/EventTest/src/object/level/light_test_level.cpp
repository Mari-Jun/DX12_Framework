#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/actor/light.h>
#include <client/object/actor/sky_cube.h>
#include <client/object/actor/sky_sphere.h>
#include <client/util/octree/octree.h>
#include <client/asset/core/asset_store.h>

#include "object/level/light_test_level.h"
#include "object/actor/rotating_cube.h"
#include "object/actor/local_rotating_cube.h"
#include "object/actor/material_billboard_actor.h"
#include "object/actor/billboard_actor.h"
#include "object/gamemode/third_game_mode.h"
#include "object/actor/collision_test_actor.h"

namespace event_test
{
	LightTestLevel::LightTestLevel()
		: Level("light test level")
	{
	}

	bool LightTestLevel::Initialize()
	{
		auto cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });

		cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ -500.0f, 0.0f, 800.0f });

		cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 500.0f, 0.0f, 800.0f });

		auto local_cube = CreateSPtr<LocalRotatingCube>();
		SpawnActor(local_cube);
		local_cube->SetPosition(Vec3{ 0.0f, 300.0f, -2000.0f });
		local_cube->SetLocalPosition(Vec3(0.0f, 0.0f, 500.0f));
		local_cube->SetRotatingSpeed(300.0f);

		local_cube = CreateSPtr<LocalRotatingCube>();
		SpawnActor(local_cube);
		local_cube->SetPosition(Vec3{ 0.0f, 0.0f, -2000.0f });
		local_cube->SetLocalPosition(Vec3(0.0f, 0.0f, 700.0f));

		local_cube = CreateSPtr<LocalRotatingCube>();
		SpawnActor(local_cube);
		local_cube->SetPosition(Vec3{ 0.0f, -300.0f, -2000.0f });
		local_cube->SetLocalPosition(Vec3(0.0f, 0.0f, 400.0f));
		local_cube->SetRotatingSpeed(120.0f);
			
		auto siege = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/Castle/SiegeRam.obj");
		SpawnActor(siege);
		siege->SetPosition(Vec3{ 0.0f, 0.0f, 2000.0f });
		siege->SetScale(0.5f);

		siege = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/Castle/SiegeRam.obj");
		SpawnActor(siege);
		siege->SetPosition(Vec3{ -2000.0f, 200.0f, 0.0f });
		siege->SetScale(0.5f);

		siege = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/Castle/SiegeRam.obj");
		SpawnActor(siege);
		siege->SetPosition(Vec3{ 2300.0f, -200.0f, 200.0f });
		siege->SetScale(0.5f);

		auto plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(0.0f, 0.0f, 3000.0f));
		plane->SetRotation(quat::CreateQuaternionFromAxis(vec3::AXIS_X, math::ToRadian(-90.0f)));
		plane->SetScale(25.0f);

		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(3000.0f, 0.0f, 0.0f));
		plane->SetRotation(math::ToRadian(90.0f), math::ToRadian(-90.0f), 0.0f);
		plane->SetScale(25.0f);
	
		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(-3000.0f, 0.0f, 0.0f));
		plane->SetRotation(math::ToRadian(90.0f), math::ToRadian(90.0f), 0.0f);
		plane->SetScale(25.0f);

		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(2200.0f, 0.0f, 2200.0f));
		plane->SetRotation(math::ToRadian(90.0f), math::ToRadian(-135.0f), 0.0f);
		plane->SetScale(25.0f);

		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(-2200.0f, 0.0f, 2200.0f));
		plane->SetRotation(math::ToRadian(90.0f), math::ToRadian(135.0f), 0.0f);
		plane->SetScale(25.0f);

		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(0.0f, 0.0f, -3000.0f));
		plane->SetRotation(math::ToRadian(90.0f), math::ToRadian(0.0f), 0.0f);
		plane->SetScale(20.0f);

		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(1000.0f, 0.0f, -2000.0f));
		plane->SetRotation(math::ToRadian(90.0f), math::ToRadian(-90.0f), 0.0f);
		plane->SetScale(20.0f);

		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(-1000.0f, 0.0f, -2000.0f));
		plane->SetRotation(math::ToRadian(90.0f), math::ToRadian(90.0f), 0.0f);
		plane->SetScale(20.0f);

		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(0.0f, 1000.0f, -2000.0f));
		plane->SetRotation(math::ToRadian(180.0f), 0.0f, 0.0f);
		plane->SetScale(20.0f);

		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(0.0f, -1000.0f, -2000.0f));
		plane->SetRotation(math::ToRadian(0.0f), 0.0f, 0.0f);
		plane->SetScale(20.0f);

		auto sphere = CreateSPtr<CollisionTestActor>();
		SpawnActor(sphere);
		sphere->SetPosition(Vec3(-500.0f, 0.0f, 500.0f));

		sphere = CreateSPtr<CollisionTestActor>();
		SpawnActor(sphere);
		sphere->SetPosition(Vec3(-500.0f, 0.0f, 250.0f));

		sphere = CreateSPtr<CollisionTestActor>();
		SpawnActor(sphere);
		sphere->SetPosition(Vec3(-500.0f, 0.0f, 000.0f));

		sphere = CreateSPtr<CollisionTestActor>();
		SpawnActor(sphere);
		sphere->SetPosition(Vec3(-500.0f, 0.0f, -250.0f));

		sphere = CreateSPtr<CollisionTestActor>();
		SpawnActor(sphere);
		sphere->SetPosition(Vec3(-500.0f, 0.0f, -500.0f));

		auto d_light = CreateSPtr<DirectionalLight>();
		d_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		d_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		SpawnActor(d_light);
		//d_light->DisableShadow();

		auto p_light = CreateSPtr<PointLight>();
		p_light->SetLightColor(Vec3(400000.0f, 0.0f, 0.0f));
		SpawnActor(p_light);

		p_light = CreateSPtr<PointLight>();
		p_light->SetLightColor(Vec3(0.0f, 400000.0f, 0.0f));
		p_light->SetPosition(Vec3(-500.0f, 0.0f, 0.0f));
		SpawnActor(p_light);

		p_light = CreateSPtr<PointLight>();
		p_light->SetLightColor(Vec3(0.0f, 0.0f, 400000.0f));
		p_light->SetPosition(Vec3(500.0f, 0.0f, 0.0f));
		SpawnActor(p_light);

		p_light = CreateSPtr<PointLight>();
		p_light->SetLightColor(Vec3(400000.0f, 0.0f, 400000.0f));
		p_light->SetPosition(Vec3(0.0f, 0.0f, -2000.0f));
		p_light->SetAttenuationRadius(1500.0f);
		p_light->SetShadowTextureSize(2000);
		SpawnActor(p_light);
		//p_light->SetShadowVisibility(false);

		p_light = CreateSPtr<PointLight>();
		p_light->SetLightColor(Vec3(400000.0f, 400000.0f, 0.0f));
		p_light->SetPosition(Vec3(-500.0f, 0.0f, -2000.0f));
		p_light->SetAttenuationRadius(1500.0f);
		SpawnActor(p_light);

		p_light = CreateSPtr<PointLight>();
		p_light->SetLightColor(Vec3(0.0f, 400000.0f, 400000.0f));
		p_light->SetPosition(Vec3(500.0f, 0.0f, -2000.0f));
		p_light->SetAttenuationRadius(1500.0f);
		SpawnActor(p_light);

		auto s_light = CreateSPtr<SpotLight>();
		s_light->SetLightColor(Vec3(0.0f, 400000.0f, 400000.0f));
		s_light->SetPosition(Vec3(0.0f, 0.0f, 1200.0f));
		s_light->SetAttenuationRadius(2000.0f);
		s_light->SetConeOuterAngle(22.0f);
		SpawnActor(s_light);
		//s_light->SetShadowVisibility(false);

		s_light = CreateSPtr<SpotLight>();
		s_light->SetLightColor(Vec3(400000.0f, 0.0f, 400000.0f));
		s_light->SetPosition(Vec3(-1200.0f, 0.0f, 0.0f));
		s_light->SetAttenuationRadius(2000.0f);
		s_light->SetRotation(0.0f, math::ToRadian(-90.0f), 0.0f);
		s_light->SetConeOuterAngle(33.0f);
		SpawnActor(s_light);

		s_light = CreateSPtr<SpotLight>();
		s_light->SetLightColor(Vec3(400000.0f, 400000.0f, 0.0f));
		s_light->SetPosition(Vec3(1200.0f, 0.0f, 0.0f));
		s_light->SetAttenuationRadius(2000.0f);
		s_light->SetRotation(0.0f, math::ToRadian(90.0f), 0.0f);
		s_light->SetConeOuterAngle(44.0f);
		SpawnActor(s_light);

		s_light = CreateSPtr<SpotLight>();
		s_light->SetLightColor(Vec3(0.0f, 400000.0f, 0.0f));
		s_light->SetPosition(Vec3(1500.0f, 0.0f, 1500.0f));
		s_light->SetAttenuationRadius(2000.0f);
		s_light->SetRotation(0.0f, math::ToRadian(45.0f), 0.0f);
		s_light->SetConeOuterAngle(30.0f);
		SpawnActor(s_light);

		s_light = CreateSPtr<SpotLight>();
		s_light->SetLightColor(Vec3(400000.0f, 0.0f, 0.0f));
		s_light->SetPosition(Vec3(-1500.0f, 0.0f, 1500.0f));
		s_light->SetAttenuationRadius(3000.0f);
		s_light->SetRotation(0.0f, math::ToRadian(-45.0f), 0.0f);
		s_light->SetConeOuterAngle(45.0f);
		s_light->SetShadowTextureSize(2000);
		SpawnActor(s_light);

		s_light = CreateSPtr<SpotLight>();
		s_light->SetLightColor(Vec3(0.0f, 0.0f, 400000.0f));
		s_light->SetPosition(Vec3(2000.0f, 0.0f, 2000.0f));
		s_light->SetAttenuationRadius(3000.0f);
		s_light->SetRotation(0.0f, math::ToRadian(135.0f), 0.0f);
		s_light->SetConeOuterAngle(45.0f);
		SpawnActor(s_light);

		auto tree = CreateSPtr<MaterialBillboardActor>(eMobilityState::kStatic,
			"../Contents/basic_tree_01.mtl", "Basic_Tree_01", Vec2(200.0f, 400.0f), true);
		SpawnActor(tree);
		tree->SetPosition(Vec3(0.0f, 0.0f, 500.0f));

		tree = CreateSPtr<MaterialBillboardActor>(eMobilityState::kStatic,
			"../Contents/basic_tree_01.mtl", "Basic_Tree_01", Vec2(200.0f, 400.0f), true);
		SpawnActor(tree);
		tree->SetPosition(Vec3(300.0f, 200.0f, 2500.0f));

		auto billboard_tree = CreateSPtr<BillboardActor>(eMobilityState::kStatic, "../Contents/Tree_02.dds", Vec2(200.0f, 400.0f), true);
		SpawnActor(billboard_tree);
		billboard_tree->SetPosition(Vec3(300.0f, -200.0f, 2500.0f));

		auto sky_cube = CreateSPtr<SkyCube>("../Contents/snowcube1024.dds");
		SpawnActor(sky_cube);
		
		sky_cube = CreateSPtr<SkyCube>("../Contents/desertcube1024.dds");
		SpawnActor(sky_cube);

		/*auto sky_sphere = CreateSPtr<SkySphere>();
		SpawnActor(sky_sphere);
		sky_sphere->SetCenterColor(Vec3(0.0f, 0.0f, 0.0f));
		sky_sphere->SetApexColor(Vec3(1.0f, 1.0f, 1.0f));*/

		return true;
	}

	void LightTestLevel::Shutdown()
	{
	}

	void LightTestLevel::Update(float delta_time)
	{
	}

	UPtr<GameMode> LightTestLevel::CreateGameMode() const
	{
		return CreateUPtr<ThirdGameMode>();
	}

	std::vector<SPtr<VisualOctree>> LightTestLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(10000.0f));
		return visual_octrees;
	}
}