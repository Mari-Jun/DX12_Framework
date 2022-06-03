#include <include/client_core.h>
#include <client/object/actor/light.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/level/gamemode/game_mode_base.h>
#include <client/object/actor/default_pawn.h>

#include "object/level/rendering/basic/gbuffer_level.h"
#include "object/levelsetting/rendering/basic/gbuffer_level_node_manager.h"

namespace simulation
{
	UPtr<GBufferLevelInitNodeManager> GBufferLevel::s_init_node_manager = nullptr;
	UPtr<GBufferLevelRuntimeNodeManager> GBufferLevel::s_runtime_node_manager = nullptr;

	GBufferLevel::GBufferLevel()
		: SimulationLevel("g-buffer level")
	{
		m_directional_light = CreateSPtr<DirectionalLight>();
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<GBufferLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<GBufferLevelRuntimeNodeManager>();
	}

	bool GBufferLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		float offset = 500.0f;
		UINT num_of_x = 5;
		UINT num_of_y = 5;
		UINT num_of_z = 5;

		float start_x = static_cast<float>(num_of_x - 1) * -0.5f * offset;
		float start_y = static_cast<float>(num_of_y - 1) * -0.5f * offset;
		float start_z = static_cast<float>(num_of_z - 1) * -0.5f * offset;

		for (UINT x = 0; x < num_of_x; ++x)
		{
			float new_x = start_x + x * offset;
			for (UINT z = 0; z < num_of_z; ++z)
			{
				float new_z = start_z + z * offset;
				for (UINT y = 0; y < num_of_y; ++y)
				{
					float new_y = start_y + y * offset;

					auto static_mesh_actor = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "Contents/Meshes/deathstar/death_star.obj");
					static_mesh_actor->SetPosition(Vec3(new_x, new_y, new_z));
					static_mesh_actor->SetScale(2.5f);
					SpawnActor(static_mesh_actor);
				}
			}
		}

		m_directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		m_directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		m_directional_light->DisableShadow();
		SpawnActor(m_directional_light);

		GetGameMode()->GetDefaultPawn()->SetPosition(Vec3(0.f, 0.f, start_z - 500.0f));

		return ret;
	}

	void GBufferLevel::Shutdown()
	{
	}

	void GBufferLevel::Update(float delta_time)
	{
	}

	void GBufferLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<GBufferLevel>(shared_from_this()));
	}

	void GBufferLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void GBufferLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<GBufferLevel>(shared_from_this()));
	}

	void GBufferLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}
