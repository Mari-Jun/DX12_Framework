#pragma once
#include "object/level/core/simulation_level.h"

namespace simulation
{
    class CameraPawn;

    class BlurLevelInitNodeManager;
    class BlurLevelRuntimeNodeManager;

    class BlurLevel : public SimulationLevel
    {
    public:
        BlurLevel();
        virtual ~BlurLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

        virtual void SetLevelInitNodeOwner() override;
        virtual void ExecuteLevelInitNodes() override;

        virtual void SetLevelRuntimeNodeOwner() override;
        virtual void ExecuteLevelRuntimeNodes() override;

    private:
        std::vector<SPtr<CameraPawn>> m_spawned_pawns;

    public:
        const std::vector<SPtr<CameraPawn>>& GetSpawnedPawns() const { return m_spawned_pawns; }

    private:
        static UPtr<BlurLevelInitNodeManager> s_init_node_manager;
        static UPtr<BlurLevelRuntimeNodeManager> s_runtime_node_manager;
    };
}


