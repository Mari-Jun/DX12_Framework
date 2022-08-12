#include <include/client_core.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/actor/light.h>

#include "object/actor/local_rotating_cube.h"
#include "object/actor/rotating_cube.h"
#include "object/level/rendering/lighting/light_level.h"
#include "object/levelsetting/rendering/lighting/light_level_node_manager.h"
#include "object/gamemode/third_game_mode.h"

namespace simulation
{
	UPtr<LightLevelInitNodeManager> LightLevel::s_init_node_manager = nullptr;
	UPtr<LightLevelRuntimeNodeManager> LightLevel::s_runtime_node_manager = nullptr;

	LightLevel::LightLevel()
		: SimulationLevel("light level")
	{
		m_game_mode = CreateUPtr<ThirdGameMode>();
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<LightLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<LightLevelRuntimeNodeManager>();

		m_directional_light1 = CreateSPtr<DirectionalLight>(eMobilityState::kMovable);
		m_directional_light2 = CreateSPtr<DirectionalLight>(eMobilityState::kMovable);
		m_point_light1 = CreateSPtr<PointLight>(eMobilityState::kMovable);
		m_point_light2 = CreateSPtr<PointLight>(eMobilityState::kMovable);
		m_point_light3 = CreateSPtr<PointLight>(eMobilityState::kMovable);
		m_spot_light1 = CreateSPtr<SpotLight>(eMobilityState::kMovable);
		m_spot_light2 = CreateSPtr<SpotLight>(eMobilityState::kMovable);
		m_spot_light3 = CreateSPtr<SpotLight>(eMobilityState::kMovable);
	}

	bool LightLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		auto wall = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
		SpawnActor(wall);
		wall->SetPosition(Vec3(0.0f, 0.0f, 4000.0f));
		wall->SetScale(Vec3(40.f, 40.f, 0.01f));

		wall = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
		SpawnActor(wall);
		wall->SetPosition(Vec3(2000.0f, 0.0f, 2000.0f));
		wall->SetScale(Vec3(0.01f, 40.f, 40.f));

		wall = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
		SpawnActor(wall);
		wall->SetPosition(Vec3(-2000.0f, 0.0f, 2000.0f));
		wall->SetScale(Vec3(0.01f, 40.f, 40.f));

		wall = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
		SpawnActor(wall);
		wall->SetPosition(Vec3(0.0f, -2000.0f, 2000.0f));
		wall->SetScale(Vec3(40.f, 0.01f, 40.f));

		auto cube = CreateSPtr<RotatingCube>();
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 0.0f, -1900.0f, 2000.0f });

		auto local_cube = CreateSPtr<LocalRotatingCube>();
		SpawnActor(local_cube);
		local_cube->SetPosition(Vec3{ 0.0f, -1000.0f, 2000.0f });
		local_cube->SetLocalPosition(Vec3(0.0f, 0.0f, 1000.0f));
		local_cube->SetRotatingSpeed(300.0f);

		local_cube = CreateSPtr<LocalRotatingCube>();
		SpawnActor(local_cube);
		local_cube->SetPosition(Vec3{ 0.0f, -500.0f, 2000.0f });
		local_cube->SetLocalPosition(Vec3(0.0f, 0.0f, 800.f));
		local_cube->SetRotatingSpeed(240.0f);

		local_cube = CreateSPtr<LocalRotatingCube>();
		SpawnActor(local_cube);
		local_cube->SetPosition(Vec3{ 0.0f, 0.0f, 2000.0f });
		local_cube->SetLocalPosition(Vec3(0.0f, 0.0f, 500.0f));

		local_cube = CreateSPtr<LocalRotatingCube>();
		SpawnActor(local_cube);
		local_cube->SetPosition(Vec3{ 0.0f, 500.0f, 2000.0f });
		local_cube->SetLocalPosition(Vec3(0.0f, 0.0f, 1250.f));
		local_cube->SetRotatingSpeed(120.0f);

		local_cube = CreateSPtr<LocalRotatingCube>();
		SpawnActor(local_cube);
		local_cube->SetPosition(Vec3{ 0.0f, 1000.0f, 2000.0f });
		local_cube->SetLocalPosition(Vec3(0.0f, 0.0f, 1500.0f));
		local_cube->SetRotatingSpeed(60.0f);

		auto siege = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/Castle/SiegeRam.obj");
		SpawnActor(siege);
		siege->SetPosition(Vec3{ -1000.0f, 0.0f, 3000.0f });
		siege->SetScale(0.5f);

		siege = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/Castle/SiegeRam.obj");
		SpawnActor(siege);
		siege->SetPosition(Vec3{ 0.0f, 2000.0f, 2000.0f });
		siege->SetScale(0.5f);

		siege = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/Castle/SiegeRam.obj");
		SpawnActor(siege);
		siege->SetPosition(Vec3{ 1500.0f, -1500.0f, 1500.0f });
		siege->SetScale(0.5f);

		INT shadow_size = s_init_node_manager->GetDirectionalLightShadowSize();
		m_directional_light1->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		m_directional_light1->SetLightIntensity(1.0f);
		m_directional_light1->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		m_directional_light1->SetShadowTextureSize(shadow_size);
		SpawnActor(m_directional_light1);

		m_directional_light2->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		m_directional_light2->SetLightIntensity(0.5f);
		m_directional_light2->SetRotation(math::ToRadian(45.0f), math::ToRadian(90.f), 0.0f);
		m_directional_light2->SetShadowTextureSize(shadow_size);
		SpawnActor(m_directional_light2);

		shadow_size = s_init_node_manager->GetPointLightShadowSize();
		m_point_light1->SetLightColor(Vec3(1.0f, 0.0f, 0.0f));
		m_point_light1->SetLightIntensity(100'000.0f);
		m_point_light1->SetShadowTextureSize(shadow_size);
		m_point_light1->SetAttenuationRadius(500.f);
		m_point_light1->SetPosition(Vec3(0.0f, -1700.0f, 2200.0f));
		SpawnActor(m_point_light1);

		m_point_light2->SetLightColor(Vec3(0.0f, 1.0f, 0.0f));
		m_point_light2->SetLightIntensity(100'000.0f);
		m_point_light2->SetShadowTextureSize(shadow_size);
		m_point_light2->SetAttenuationRadius(500.f);
		m_point_light2->SetPosition(Vec3(-200.0f, -1700.0f, 2000.0f));
		SpawnActor(m_point_light2);

		m_point_light3->SetLightColor(Vec3(0.0f, 0.0f, 1.0f));
		m_point_light3->SetLightIntensity(100'000.0f);
		m_point_light3->SetShadowTextureSize(shadow_size);
		m_point_light3->SetAttenuationRadius(500.f);
		m_point_light3->SetPosition(Vec3(200.0f, -1700.0f, 2000.0f));
		SpawnActor(m_point_light3);

		shadow_size = s_init_node_manager->GetSpotLightShadowSize();
		m_spot_light1->SetLightColor(Vec3(1.0f, 0.0f, 0.0f));
		m_spot_light1->SetLightIntensity(100'000.0f);
		m_spot_light1->SetShadowTextureSize(shadow_size);
		m_spot_light1->SetAttenuationRadius(2048.f);
		m_spot_light1->SetPosition(Vec3(0.0f, 0.0f, 2000.f));
		m_spot_light1->SetRotation(0.0f, math::ToRadian(0.0f), 0.0f);
		SpawnActor(m_spot_light1);

		m_spot_light2->SetLightColor(Vec3(0.0f, 1.0f, 0.0f));
		m_spot_light2->SetLightIntensity(100'000.0f);
		m_spot_light2->SetShadowTextureSize(shadow_size);
		m_spot_light2->SetAttenuationRadius(2048.f);
		m_spot_light2->SetPosition(Vec3(0.0f, 0.0f, 2000.0f));
		m_spot_light2->SetRotation(0.0f, math::ToRadian(-90.0f), 0.0f);
		SpawnActor(m_spot_light2);

		m_spot_light3->SetLightColor(Vec3(0.0f, 0.0f, 1.0f));
		m_spot_light3->SetLightIntensity(100'000.0f);
		m_spot_light3->SetShadowTextureSize(shadow_size);
		m_spot_light3->SetAttenuationRadius(2048.f);
		m_spot_light3->SetPosition(Vec3(0.0f, 0.0f, 2000.0f));
		m_spot_light3->SetRotation(0.0f, math::ToRadian(90.0f), 0.0f);
		SpawnActor(m_spot_light3);

		return ret;
	}

	void LightLevel::Shutdown()
	{
	}

	void LightLevel::Update(float delta_time)
	{
	}

	void LightLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<LightLevel>(shared_from_this()));
	}

	void LightLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void LightLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<LightLevel>(shared_from_this()));
	}

	void LightLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}

	SPtr<DirectionalLight> LightLevel::GetDirectionalLight(int index) const
	{
		switch (index)
		{
		case 0: return m_directional_light1; 
		case 1: return m_directional_light2;
		default: return nullptr;
		}
	}

	SPtr<PointLight> LightLevel::GetPointLight(int index) const
	{
		switch (index)
		{
		case 0: return m_point_light1;
		case 1: return m_point_light2;
		case 2: return m_point_light3;
		default: return nullptr;
		}
	}

	SPtr<SpotLight> LightLevel::GetSpotLight(int index) const
	{
		switch (index)
		{
		case 0: return m_spot_light1;
		case 1: return m_spot_light2;
		case 2: return m_spot_light3;
		default: return nullptr;
		}
	}
}