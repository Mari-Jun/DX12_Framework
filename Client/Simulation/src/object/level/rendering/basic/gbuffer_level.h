#pragma once
#include "object/level/core/simulation_level.h"

namespace client_fw
{
	class DirectionalLight;
	class StaticMeshActor;
}

namespace simulation
{
	class GBufferLevelInitNodeManager;
	class GBufferLevelRuntimeNodeManager;

	class GBufferLevel : public SimulationLevel
	{
    public:
        GBufferLevel();
        virtual ~GBufferLevel() = default;

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
        const SPtr<DirectionalLight>& GetDirectionalLight() const { return m_directional_light; }

    private:
        static UPtr<GBufferLevelInitNodeManager> s_init_node_manager;
        static UPtr<GBufferLevelRuntimeNodeManager> s_runtime_node_manager;
	};

}


