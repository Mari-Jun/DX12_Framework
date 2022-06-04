#include <include/client_core.h>
#include <client/object/actor/light.h>
#include <client/object/level/gamemode/game_mode_base.h>
#include <client/object/actor/default_pawn.h>

#include "object/actor/camera_actor.h"
#include "object/level/component/player/possess_pawn_level.h"
#include "object/levelsetting/component/player/possess_pawn_level_node_manager.h"
#include "object/gamemode/camera_game_mode.h"

namespace simulation
{
	UPtr<PossessPawnLevelInitNodeManager> PossessPawnLevel::s_init_node_manager = nullptr;
	UPtr<PossessPawnLevelRuntimeNodeManager> PossessPawnLevel::s_runtime_node_manager = nullptr;

	PossessPawnLevel::PossessPawnLevel()
		: SimulationLevel("possess pawn level")
	{
		m_game_mode = CreateUPtr<CameraGameMode>();
		m_directional_light = CreateSPtr<DirectionalLight>();
	
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<PossessPawnLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<PossessPawnLevelRuntimeNodeManager>();
	}

	bool PossessPawnLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		m_directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		m_directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		m_directional_light->DisableShadow();
		SpawnActor(m_directional_light);

		m_spawned_pawns.push_back(GetGameMode()->GetDefaultPawn());

		UINT num_of_camera = 6;
		float offset = 500.f;
		for (UINT i = 0; i < num_of_camera; ++i)
		{
			float radian = i * (XM_2PI / num_of_camera);

			auto camera_pawn = CreateSPtr<CameraPawn>();
			camera_pawn->SetPosition(Vec3(sin(radian) * offset, 0.0f, cos(radian) * offset));
			camera_pawn->SetRotation(0.0f, radian + XM_PI, 0.0f);
			SpawnActor(camera_pawn);

			m_spawned_pawns.push_back(camera_pawn);
		}

		return ret;
	}

	void PossessPawnLevel::Shutdown()
	{
	}

	void PossessPawnLevel::Update(float delta_time)
	{
	}

	void PossessPawnLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<PossessPawnLevel>(shared_from_this()));
	}

	void PossessPawnLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void PossessPawnLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<PossessPawnLevel>(shared_from_this()));
	}

	void PossessPawnLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}
