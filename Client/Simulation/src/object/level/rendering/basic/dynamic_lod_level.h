#pragma once
#include "object/level/core/simulation_level.h"

namespace client_fw
{
    class StaticMeshActor;
    class DirectionalLight;
}

namespace simulation
{
	class DynamicLODLevelInitNodeManager;
	class DynamicLODLevelRuntimeNodeManager;

    class DynamicLODLevel : public SimulationLevel
    {
    public:
        DynamicLODLevel();
        virtual ~DynamicLODLevel() = default;

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

	private:
		static UPtr<DynamicLODLevelInitNodeManager> s_init_node_manager;
		static UPtr<DynamicLODLevelRuntimeNodeManager> s_runtime_node_manager;
    };

}


