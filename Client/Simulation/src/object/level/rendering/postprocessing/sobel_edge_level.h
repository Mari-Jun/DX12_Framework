#pragma once

#include "object/level/core/simulation_level.h"

namespace simulation
{
    class SobelEdgeLevelInitNodeManager;
    class SobelEdgeLevelRuntimeNodeManager;

	class SobelEdgeLevel : public SimulationLevel
	{
    public:
        SobelEdgeLevel();
        virtual ~SobelEdgeLevel() = default;

        virtual bool Initialize() override;

        virtual void SetLevelInitNodeOwner() override;
        virtual void ExecuteLevelInitNodes() override;

        virtual void SetLevelRuntimeNodeOwner() override;
        virtual void ExecuteLevelRuntimeNodes() override;

    private:
        static UPtr<SobelEdgeLevelInitNodeManager> s_init_node_manager;
        static UPtr<SobelEdgeLevelRuntimeNodeManager> s_runtime_node_manager;
	};

}

