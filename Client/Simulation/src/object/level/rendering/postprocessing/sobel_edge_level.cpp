#include <include/client_core.h>
#include <client/object/actor/light.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/level/gamemode/game_mode_base.h>
#include <client/object/actor/default_pawn.h>

#include "object/level/rendering/postprocessing/sobel_edge_level.h"
#include "object/levelsetting/rendering/postprocessing/sobel_edge_level_node_manager.h"

namespace simulation
{
	UPtr<SobelEdgeLevelInitNodeManager> SobelEdgeLevel::s_init_node_manager = nullptr;
	UPtr<SobelEdgeLevelRuntimeNodeManager> SobelEdgeLevel::s_runtime_node_manager = nullptr;

	SobelEdgeLevel::SobelEdgeLevel()
		: SimulationLevel("sobel edge level")
	{
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<SobelEdgeLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<SobelEdgeLevelRuntimeNodeManager>();
	}

	bool SobelEdgeLevel::Initialize()
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

		GetGameMode()->GetDefaultPawn()->SetPosition(Vec3(0.f, 0.f, start_pos - 500.0f));

		return ret;
	}

	void SobelEdgeLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<SobelEdgeLevel>(shared_from_this()));
	}

	void SobelEdgeLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void SobelEdgeLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<SobelEdgeLevel>(shared_from_this()));
	}

	void SobelEdgeLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}