#pragma once
#include "object/level/core/simulation_level.h"

namespace client_fw
{
    class DirectionalLight;
}

namespace simulation
{
    class MultiCameraLevelInitNodeManager;
    class MultiCameraLevelRuntimeNodeManager;

    class CameraActor;

    class MultiCameraLevel : public SimulationLevel
    {
    public:
        MultiCameraLevel();
        virtual ~MultiCameraLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

        virtual void SetLevelInitNodeOwner() override;
        virtual void ExecuteLevelInitNodes() override;

        virtual void SetLevelRuntimeNodeOwner() override;
        virtual void ExecuteLevelRuntimeNodes() override;

    private:
        SPtr<DirectionalLight> m_directional_light;
        std::vector<SPtr<CameraActor>> m_camera_actors;

    public:
        const std::vector<SPtr<CameraActor>>& GetCameraActors() const { return m_camera_actors; }

    private:
        static UPtr<MultiCameraLevelInitNodeManager> s_init_node_manager;
        static UPtr<MultiCameraLevelRuntimeNodeManager> s_runtime_node_manager;
 
    };

}


