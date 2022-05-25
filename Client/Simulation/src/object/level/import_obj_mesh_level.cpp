#include <include/client_core.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/actor/light.h>
#include <client/object/component/mesh/static_mesh_component.h>

#include "object/level/import_obj_mesh_level.h"
#include "object/levelsetting/import_obj_mesh_level_node_manager.h"

namespace simulation
{
	UPtr<ImportObjMeshLevelInitNodeManager> ImportObjMeshLevel::m_init_node_manager = nullptr;
	UPtr<ImportObjMeshLevelRuntimeNodeManager> ImportObjMeshLevel::m_runtime_node_manager = nullptr;

	ImportObjMeshLevel::ImportObjMeshLevel()
		: SimulationLevel("import obj mesh level")
	{
		m_directional_light = CreateSPtr<DirectionalLight>();
		m_static_mesh_actor = CreateSPtr<StaticMeshActor>(eMobilityState::kMovable, "");
		if (m_init_node_manager == nullptr)
			m_init_node_manager = CreateUPtr<ImportObjMeshLevelInitNodeManager>();
		if (m_runtime_node_manager == nullptr)
			m_runtime_node_manager = CreateUPtr<ImportObjMeshLevelRuntimeNodeManager>();
	}

	bool ImportObjMeshLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		SpawnActor(m_static_mesh_actor);
		m_directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		m_directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		m_directional_light->DisableShadow();
		SpawnActor(m_directional_light);

		return ret;
	}

	void ImportObjMeshLevel::Shutdown()
	{
	}

	void ImportObjMeshLevel::Update(float delta_time)
	{
	}

	void ImportObjMeshLevel::SetLevelInitNodeOwner()
	{
		m_init_node_manager->SetOwner(std::static_pointer_cast<ImportObjMeshLevel>(shared_from_this()));
	}

	void ImportObjMeshLevel::ExecuteLevelInitNodes()
	{
		m_init_node_manager->ExecuteLevelSettingNodes();
	}

	void ImportObjMeshLevel::SetLevelRuntimeNodeOwner()
	{
		m_runtime_node_manager->SetOwner(std::static_pointer_cast<ImportObjMeshLevel>(shared_from_this()));
	}

	void ImportObjMeshLevel::ExecuteLevelRuntimeNodes()
	{
		m_runtime_node_manager->ExecuteLevelSettingNodes();
	}

	void ImportObjMeshLevel::SetStaticMeshPath(const std::string& path)
	{
		m_static_mesh_actor->GetStaticMeshComponent()->SetMesh(path);
	}
}