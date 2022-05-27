#include <include/client_core.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/actor/light.h>
#include <client/object/component/mesh/static_mesh_component.h>

#include "object/level/dynamic_lod_level.h"
#include "object/levelsetting/dynamic_lod_level_node_manager.h"

namespace simulation
{
	UPtr<DynamicLODLevelInitNodeManager> DynamicLODLevel::s_init_node_manager = nullptr;
	UPtr<DynamicLODLevelRuntimeNodeManager> DynamicLODLevel::s_runtime_node_manager = nullptr;

	DynamicLODLevel::DynamicLODLevel()
		: SimulationLevel("dynamic lod level")
	{
		m_directional_light = CreateSPtr<DirectionalLight>();
		m_static_mesh_actor = CreateSPtr<StaticMeshActor>(eMobilityState::kMovable, "Contents/Meshes/deathstar/death_star.obj");
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<DynamicLODLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<DynamicLODLevelRuntimeNodeManager>();
	}

	bool DynamicLODLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		m_static_mesh_actor->SetPosition(Vec3(0.f, 0.f, 500.f));
		SpawnActor(m_static_mesh_actor);
		m_directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		m_directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		m_directional_light->DisableShadow();
		SpawnActor(m_directional_light);

		return ret;
	}

	void DynamicLODLevel::Shutdown()
	{
	}

	void DynamicLODLevel::Update(float delta_time)
	{
	}

	void DynamicLODLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<DynamicLODLevel>(shared_from_this()));
	}

	void DynamicLODLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void DynamicLODLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<DynamicLODLevel>(shared_from_this()));
	}

	void DynamicLODLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}
