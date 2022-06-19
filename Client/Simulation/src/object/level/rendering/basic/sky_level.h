#pragma once
#include "object/level/core/simulation_level.h"

namespace simulation
{
	class SkyCubeLevelInitNodeManager;
	class SkyCubeLevelRuntimeNodeManager;

	class SkyCubeLevel : public SimulationLevel
	{
    public:
        SkyCubeLevel();
        virtual ~SkyCubeLevel() = default;

        virtual bool Initialize() override;

        virtual void SetLevelInitNodeOwner() override;
        virtual void ExecuteLevelInitNodes() override;

        virtual void SetLevelRuntimeNodeOwner() override;
        virtual void ExecuteLevelRuntimeNodes() override;

    public:
        const SPtr<SkyCube>& GetSkyCube() const { return m_sky_cube; }

    private:
        static UPtr<SkyCubeLevelInitNodeManager> s_init_node_manager;
        static UPtr<SkyCubeLevelRuntimeNodeManager> s_runtime_node_manager;
	};
}



