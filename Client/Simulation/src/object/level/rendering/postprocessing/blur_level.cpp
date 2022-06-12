#include <include/client_core.h>
#include <client/object/actor/light.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/level/gamemode/game_mode_base.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/component/util/render_camera_component.h>

#include "object/actor/camera_actor.h"
#include "object/level/rendering/postprocessing/blur_level.h"
#include "object/levelsetting/rendering/postprocessing/blur_level_node_manager.h"
#include "object/gamemode/camera_game_mode.h"

namespace simulation
{
	UPtr<BlurLevelInitNodeManager> BlurLevel::s_init_node_manager = nullptr;
	UPtr<BlurLevelRuntimeNodeManager> BlurLevel::s_runtime_node_manager = nullptr;

	BlurLevel::BlurLevel()
		: SimulationLevel("blur level")
	{
		m_game_mode = CreateUPtr<CameraGameMode>(false);

		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<BlurLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<BlurLevelRuntimeNodeManager>();
	}

	bool BlurLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		auto directional_light = CreateSPtr<DirectionalLight>();
		directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		directional_light->DisableShadow();
		SpawnActor(directional_light);

		float offset = 500.0f;
		UINT num_of_line = 5;

		float start_pos = static_cast<float>(num_of_line - 1) * -0.5f * offset;

		for (UINT x = 0; x < num_of_line; ++x)
		{
			float new_x = start_pos + x * offset;
			for (UINT z = 0; z < num_of_line; ++z)
			{
				float new_z = start_pos + z * offset;
				for (UINT y = 0; y < num_of_line; ++y)
				{
					float new_y = start_pos + y * offset;

					auto static_mesh_actor = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "Contents/Meshes/deathstar/death_star.obj");
					static_mesh_actor->SetPosition(Vec3(new_x, new_y, new_z));
					static_mesh_actor->SetScale(2.5f);
					SpawnActor(static_mesh_actor);
				}
			}
		}

		m_spawned_pawns.push_back(std::static_pointer_cast<CameraPawn>(GetGameMode()->GetDefaultPawn()));

		UINT num_of_camera = 4;
		for (UINT i = 1; i < num_of_camera; ++i)
		{
			auto camera_pawn = CreateSPtr<CameraPawn>();
			SpawnActor(camera_pawn);
			m_spawned_pawns.push_back(camera_pawn);
		}

		for (UINT i = 0; i < num_of_camera; ++i)
		{
			float new_offset = offset * num_of_line;
			float radian = i * (XM_2PI / num_of_camera) + XM_PI;

			m_spawned_pawns[i]->SetPosition(Vec3(sin(radian) * new_offset, 0.0f, cos(radian) * new_offset));
			m_spawned_pawns[i]->SetRotation(0.0f, radian + XM_PI, 0.0f);
		}

		return ret;
	}

	void BlurLevel::Shutdown()
	{
	}

	void BlurLevel::Update(float delta_time)
	{
	}

	void BlurLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<BlurLevel>(shared_from_this()));
	}

	void BlurLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void BlurLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<BlurLevel>(shared_from_this()));
	}

	void BlurLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}
