#pragma once
#include "object/level/core/simulation_level.h"

namespace client_fw
{
    class DirectionalLight;
    class Pawn;
}

namespace simulation
{
    class CameraPawn;

    class PossessPawnLevelInitNodeManager;
    class PossessPawnLevelRuntimeNodeManager;

    class PossessPawnLevel : public SimulationLevel
    {
    public:
        PossessPawnLevel();
        virtual ~PossessPawnLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

        virtual void SetLevelInitNodeOwner() override;
        virtual void ExecuteLevelInitNodes() override;

        virtual void SetLevelRuntimeNodeOwner() override;
        virtual void ExecuteLevelRuntimeNodes() override;

    private:
        SPtr<DirectionalLight> m_directional_light;
        std::vector<SPtr<Pawn>> m_spawned_pawns;

    public:
        const std::vector<SPtr<Pawn>>& GetSpawnedPawns() const { return m_spawned_pawns; }

    private:
        static UPtr<PossessPawnLevelInitNodeManager> s_init_node_manager;
        static UPtr<PossessPawnLevelRuntimeNodeManager> s_runtime_node_manager;
    };

}


