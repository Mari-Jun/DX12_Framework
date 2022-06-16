#pragma once
#include "object/level/core/simulation_level.h"

namespace simulation
{
    class TextureBillboardActor;

    class BillboardLevelInitNodeManager;
    class BillboardLevelRuntimeNodeManager;

    class TextureBillboardLevel : public SimulationLevel
    {
    public:
        TextureBillboardLevel();
        virtual ~TextureBillboardLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

        virtual void SetLevelInitNodeOwner() override;
        virtual void ExecuteLevelInitNodes() override;

        virtual void SetLevelRuntimeNodeOwner() override;
        virtual void ExecuteLevelRuntimeNodes() override;

    private:
        SPtr<TextureBillboardActor> m_texture_billboard_actor;

    public:
        const SPtr<TextureBillboardActor>& GetTextureBillboardActor() const { return m_texture_billboard_actor; }

    private:
        static UPtr<BillboardLevelInitNodeManager> s_init_node_manager;
        static UPtr<BillboardLevelRuntimeNodeManager> s_runtime_node_manager;
    };


}

