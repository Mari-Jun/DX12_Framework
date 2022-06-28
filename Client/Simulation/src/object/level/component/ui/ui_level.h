#pragma once
#include "object/level/core/simulation_level.h"

namespace simulation
{
    class UILevelUILayer;

    class UILevelInitNodeManager;
    class UILevelRuntimeNodeManager;

	class UILevel : public SimulationLevel
	{
    public:
        UILevel();
        virtual ~UILevel() = default;

        virtual bool Initialize() override;

        virtual void SetLevelInitNodeOwner() override;
        virtual void ExecuteLevelInitNodes() override;

        virtual void SetLevelRuntimeNodeOwner() override;
        virtual void ExecuteLevelRuntimeNodes() override;

    private:
        SPtr<UILevelUILayer> m_ui_layer;

    public:
        const SPtr<UILevelUILayer>& GetUILayer() const { return m_ui_layer; }

    private:
        static UPtr<UILevelInitNodeManager> s_init_node_manager;
        static UPtr<UILevelRuntimeNodeManager> s_runtime_node_manager;
	};
}
