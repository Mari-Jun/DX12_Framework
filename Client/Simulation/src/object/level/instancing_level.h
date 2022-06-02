#pragma once
#include "object/level/core/simulation_level.h"

namespace client_fw
{
	class StaticMesh;
    class DirectionalLight;
}

namespace simulation
{
	class InstancingLevelInitNodeManager;
	class InstancingLevelRuntimeNodeManager;

    class InstancingLevel : public SimulationLevel
    {
	public:
		InstancingLevel();
		virtual ~InstancingLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

		virtual void SetLevelInitNodeOwner() override;
		virtual void ExecuteLevelInitNodes() override;

		virtual void SetLevelRuntimeNodeOwner() override;
		virtual void ExecuteLevelRuntimeNodes() override;

	private:
		SPtr<DirectionalLight> m_directional_light;

	public:
		virtual std::vector<SPtr<VisualOctree>> CreateVisualOctrees() const override;

	private:
		static UPtr<InstancingLevelInitNodeManager> s_init_node_manager;
		static UPtr<InstancingLevelRuntimeNodeManager> s_runtime_node_manager;
    };
};



