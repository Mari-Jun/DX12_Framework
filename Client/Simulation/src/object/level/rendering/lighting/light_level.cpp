#include <include/client_core.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/actor/default_pawn.h>
#include <client/util/octree/octree.h>

#include "object/actor/light.h"
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

	UPtr<MultiplePointLightLevelInitNodeManager> MultiplePointLightLevel::s_init_node_manager = nullptr;
	UPtr<MultiplePointLightLevelRuntimeNodeManager> MultiplePointLightLevel::s_runtime_node_manager = nullptr;

	MultiplePointLightLevel::MultiplePointLightLevel()
		: SimulationLevel("multiple point light level")
	{
		m_game_mode = CreateUPtr<ThirdGameMode>();
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<MultiplePointLightLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<MultiplePointLightLevelRuntimeNodeManager>();
	}

	bool MultiplePointLightLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		const bool use_shadow = s_init_node_manager->IsUseShadow();
		const bool update_intensity = s_init_node_manager->IsUpdateIntensity();
		const float offset = s_init_node_manager->GetOffset();
		const int num_of_point_lights = s_init_node_manager->GetNumOfPointLights();
		m_point_lights.reserve(num_of_point_lights * num_of_point_lights);

		const float start_point = static_cast<float>(num_of_point_lights - 1) * -0.5f * offset;
		for (int x = 0; x < num_of_point_lights; ++x)
		{
			const float new_x = start_point + x * offset;
			for (int z = 0; z < num_of_point_lights; ++z)
			{
				const float new_z = start_point + z * offset;

				SPtr<PointLight> point_light;
				if (update_intensity)
					point_light = CreateSPtr<IntensityPointLight>();
				else
					point_light = CreateSPtr<PointLight>();

				Vec3 light_color;
				switch (m_point_lights.size() % 7)
				{
				case 0: light_color = Vec3(1.f, 1.f, 1.f); break;
				case 1: light_color = Vec3(1.f, 0.f, 0.f); break;
				case 2: light_color = Vec3(0.f, 1.f, 0.f); break;
				case 3: light_color = Vec3(0.f, 0.f, 1.f); break;
				case 4: light_color = Vec3(0.f, 1.f, 1.f); break;
				case 5: light_color = Vec3(1.f, 1.f, 0.f); break;
				case 6: light_color = Vec3(1.f, 0.f, 1.f); break;
				default: break;
				}

				point_light->SetLightColor(light_color);
				point_light->SetLightIntensity(100'000);
				if (use_shadow)
				{
					point_light->SetPosition(Vec3(new_x, 0.0f, new_z));
					point_light->SetAttenuationRadius(offset);
					point_light->SetShadowTextureSize(256);
				}
				else
				{
					point_light->SetPosition(Vec3(new_x, 0.0f, new_z));
					point_light->SetAttenuationRadius(offset);
					point_light->DisableShadow();
				}

				SpawnActor(point_light);
				m_point_lights.emplace_back(point_light);
			}
		}

		auto plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(0.0f, -offset * 0.5f, 0.0f));
		plane->SetScale(static_cast<float>(num_of_point_lights) * offset / 100.f);

		plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/plane.obj");
		SpawnActor(plane);
		plane->SetPosition(Vec3(0.0f, offset * 0.5f, 0.0f));
		plane->SetRotation(math::ToRadian(180.f), 0.0f, 0.0f);
		plane->SetScale(static_cast<float>(num_of_point_lights) * offset / 100.f);
		
		GetGameMode()->GetDefaultPawn()->SetPosition(Vec3(0.f, 0.f, start_point - 500.0f));

		return ret;
	}

	void MultiplePointLightLevel::Shutdown()
	{
	}

	void MultiplePointLightLevel::Update(float delta_time)
	{
	}

	void MultiplePointLightLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<MultiplePointLightLevel>(shared_from_this()));
	}

	void MultiplePointLightLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void MultiplePointLightLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<MultiplePointLightLevel>(shared_from_this()));
	}

	void MultiplePointLightLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}

	std::vector<SPtr<VisualOctree>> MultiplePointLightLevel::CreateVisualOctrees() const
	{
		const bool use_shadow = s_init_node_manager->IsUseShadow();
		const float offset = s_init_node_manager->GetOffset();
		const int num_of_point_lights = s_init_node_manager->GetNumOfPointLights();
		float width = offset * num_of_point_lights;

		const float octree_offset = offset * 200.f;
		const int num_of_line = static_cast<int>(ceilf(width / octree_offset));
		const float start_point = static_cast<float>(num_of_line - 1) * -0.5f * octree_offset;

		std::vector<SPtr<VisualOctree>> visual_octrees;
		for (int x = 0; x < num_of_line; ++x)
		{
			const float new_x = start_point + x * octree_offset;
			for (int z = 0; z < num_of_line; ++z)
			{
				const float new_z = start_point + z * octree_offset;

				LOG_INFO(Vec3(new_x, 0.f, new_z));
				visual_octrees.emplace_back(CreateSPtr<VisualOctree>(width, Vec3(new_x, 0.0f, new_z), 3));
			}
		}

		return visual_octrees;
	}

	UPtr<MultipleSpotLightLevelInitNodeManager> MultipleSpotLightLevel::s_init_node_manager = nullptr;
	UPtr<MultipleSpotLightLevelRuntimeNodeManager> MultipleSpotLightLevel::s_runtime_node_manager = nullptr;

	MultipleSpotLightLevel::MultipleSpotLightLevel()
		: SimulationLevel("multiple spot light level")
	{
		m_game_mode = CreateUPtr<ThirdGameMode>();
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<MultipleSpotLightLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<MultipleSpotLightLevelRuntimeNodeManager>();
	}

	bool MultipleSpotLightLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		const bool use_shadow = s_init_node_manager->IsUseShadow();
		const bool update_intensity = s_init_node_manager->IsUpdateIntensity();
		const float offset = s_init_node_manager->GetOffset();
		const float track_offset = offset * 2.0f;
		const int num_of_spot_lights = s_init_node_manager->GetNumOfSpotLights();
		const int num_of_track = s_init_node_manager->GetNumOfTrack();

		m_spot_lights.reserve(num_of_spot_lights * num_of_track * num_of_track);

		const float track_start_point = static_cast<float>(num_of_track - 1) * -0.5f * track_offset;
		for (int x = 0; x < num_of_track; ++x)
		{
			const float new_x = track_start_point + x * track_offset;
			for (int y = 0; y < num_of_track; ++y)
			{
				const float new_y = track_start_point + y * track_offset;

				for (int z = 0; z < num_of_spot_lights; ++z)
				{
					const float new_z = z * offset * 0.5f;
					auto spot_light = CreateSPtr<CubeSpotLight>(update_intensity);

					for (const auto& light : spot_light->GetSpotLights())
					{
						light->SetAttenuationRadius(offset);
						if (use_shadow)
						{
							light->SetShadowTextureSize(512);
						}
						else
						{
							light->DisableShadow();
						}
					}

					spot_light->SetPosition(Vec3(new_x, new_y, new_z));
					SpawnActor(spot_light);
					m_spot_lights.emplace_back(spot_light);

					auto plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
					SpawnActor(plane);
					plane->SetPosition(Vec3(new_x, new_y - offset * 0.5f, new_z));
					plane->SetRotation(0.0f, 0.0f, math::ToRadian(0.0f));
					plane->SetScale(Vec3(offset / 100.f, offset / 10000.f, offset / 100.f));

					plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
					SpawnActor(plane);
					plane->SetPosition(Vec3(new_x + offset * 0.5f, new_y, new_z));
					plane->SetRotation(0.0f, 0.0f, math::ToRadian(90.0f));
					plane->SetScale(Vec3(offset / 100.f, offset / 10000.f, offset / 100.f));

					plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
					SpawnActor(plane);
					plane->SetPosition(Vec3(new_x, new_y + offset * 0.5f, new_z));
					plane->SetRotation(0.0f, 0.0f, math::ToRadian(180.0f));
					plane->SetScale(Vec3(offset / 100.f, offset / 10000.f, offset / 100.f));

					plane = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "../Contents/cube.obj");
					SpawnActor(plane);
					plane->SetPosition(Vec3(new_x - offset * 0.5f, new_y, new_z));
					plane->SetRotation(0.0f, 0.0f, math::ToRadian(270.0f));
					plane->SetScale(Vec3(offset / 100.f, offset / 10000.f, offset / 100.f));
				}
			}
		}

		const float new_x = 0.0f;
		const float new_y = 0.0f;

		

		GetGameMode()->GetDefaultPawn()->SetPosition(Vec3(0.f, 0.f, -offset));

		return ret;
	}

	void MultipleSpotLightLevel::Shutdown()
	{
	}

	void MultipleSpotLightLevel::Update(float delta_time)
	{
	}

	void MultipleSpotLightLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<MultipleSpotLightLevel>(shared_from_this()));
	}

	void MultipleSpotLightLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void MultipleSpotLightLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<MultipleSpotLightLevel>(shared_from_this()));
	}

	void MultipleSpotLightLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}

	std::vector<SPtr<VisualOctree>> MultipleSpotLightLevel::CreateVisualOctrees() const
	{
		const float offset = s_init_node_manager->GetOffset();
		const int num_of_point_lights = std::max(s_init_node_manager->GetNumOfSpotLights(), s_init_node_manager->GetNumOfTrack());
		float width = offset * num_of_point_lights;

		const float octree_offset = offset * 200.f;
		const int num_of_line = static_cast<int>(ceilf(width / octree_offset));
	
		std::vector<SPtr<VisualOctree>> visual_octrees;
		for (int z = 0; z < num_of_line; ++z)
		{
			const float new_z = width * 0.5f + z * octree_offset;

			LOG_INFO(Vec3(new_x, 0.f, new_z));
			visual_octrees.emplace_back(CreateSPtr<VisualOctree>(width, Vec3(0.0f, 0.0f, new_z), 3));
		}

		return visual_octrees;
	}
}