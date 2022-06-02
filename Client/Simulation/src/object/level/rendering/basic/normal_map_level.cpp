#include <include/client_core.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/actor/light.h>

#include "object/level/rendering/basic/normal_map_level.h"
#include "object/levelsetting/rendering/basic/normal_map_level_node_manager.h"

namespace simulation
{
	UPtr<NormalMapLevelInitNodeManager> NormalMapLevel::s_init_node_manager = nullptr;
	UPtr<NormalMapLevelRuntimeNodeManager> NormalMapLevel::s_runtime_node_manager = nullptr;

	NormalMapLevel::NormalMapLevel()
		: SimulationLevel("normal map level")
	{
		m_berrel = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "Contents/Meshes/barrel/barrel.obj");
		m_berrel_normal = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic, "Contents/Meshes/barrel/barrel_normal.obj");
		m_directional_light = CreateSPtr<DirectionalLight>();
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<NormalMapLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<NormalMapLevelRuntimeNodeManager>();
	}

	bool NormalMapLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		m_berrel->SetPosition(Vec3(-250.0f, 0.0f, 500.0f));
		m_berrel->SetScale(2.0f);
		m_berrel_normal->SetPosition(Vec3(250.0f, 0.0f, 500.0f));
		m_berrel_normal->SetScale(2.0f);
		SpawnActor(m_berrel);
		SpawnActor(m_berrel_normal);

		m_directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		m_directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		m_directional_light->DisableShadow();
		SpawnActor(m_directional_light);

		return ret;
	}

	void NormalMapLevel::Shutdown()
	{
	}

	void NormalMapLevel::Update(float delta_time)
	{
	}

	void NormalMapLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<NormalMapLevel>(shared_from_this()));
	}

	void NormalMapLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void NormalMapLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<NormalMapLevel>(shared_from_this()));
	}

	void NormalMapLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}
