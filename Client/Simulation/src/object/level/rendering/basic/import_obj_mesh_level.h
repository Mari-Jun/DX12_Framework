#pragma once
#include "object/level/core/simulation_level.h"

namespace client_fw
{
	class StaticMeshActor;
	class DirectionalLight;
}

namespace simulation
{
	class ImportObjMeshLevelInitNodeManager;
	class ImportObjMeshLevelRuntimeNodeManager;

	class ImportObjMeshLevel : public SimulationLevel
	{
	public:
		ImportObjMeshLevel();
		virtual ~ImportObjMeshLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

		virtual void SetLevelInitNodeOwner() override;
		virtual void ExecuteLevelInitNodes() override;

		virtual void SetLevelRuntimeNodeOwner() override;
		virtual void ExecuteLevelRuntimeNodes() override;

	private:
		SPtr<StaticMeshActor> m_static_mesh_actor;
		SPtr<DirectionalLight> m_directional_light;

	public:
		const SPtr<StaticMeshActor>& GetStaticMeshActor() const { return m_static_mesh_actor; }
		void SetStaticMeshPath(const std::string& path);

	private:
		static UPtr<ImportObjMeshLevelInitNodeManager> m_init_node_manager;
		static UPtr<ImportObjMeshLevelRuntimeNodeManager> m_runtime_node_manager;
	};
}



