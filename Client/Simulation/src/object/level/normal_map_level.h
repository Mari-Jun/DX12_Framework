#pragma once
#include "object/level/core/simulation_level.h"

namespace client_fw
{
    class StaticMeshActor;
    class DirectionalLight;
}

namespace simulation
{
    class NormalMapLevelInitNodeManager;
    class NormalMapLevelRuntimeNodeManager;

    class NormalMapLevel : public SimulationLevel
    {
    public:
        NormalMapLevel();
        virtual ~NormalMapLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

        virtual void SetLevelInitNodeOwner() override;
        virtual void ExecuteLevelInitNodes() override;

        virtual void SetLevelRuntimeNodeOwner() override;
        virtual void ExecuteLevelRuntimeNodes() override;

    private:
        SPtr<StaticMeshActor> m_berrel;
        SPtr<StaticMeshActor> m_berrel_normal;
        SPtr<DirectionalLight> m_directional_light;

    public:
        const SPtr<DirectionalLight>& GetDirectionalLight() const { return m_directional_light; }

    private:
        static UPtr<NormalMapLevelInitNodeManager> s_init_node_manager;
        static UPtr<NormalMapLevelRuntimeNodeManager> s_runtime_node_manager;
    };
}


