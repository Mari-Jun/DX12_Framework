#pragma once
#include "object/level/core/simulation_level.h"

namespace simulation
{
    class TextureBillboardActor;

    class TextureBillboardLevelInitNodeManager;
    class TextureBillboardLevelRuntimeNodeManager;

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
        static UPtr<TextureBillboardLevelInitNodeManager> s_init_node_manager;
        static UPtr<TextureBillboardLevelRuntimeNodeManager> s_runtime_node_manager;
    };

    class TextureBillboardAnimationLevelInitNodeManager;
    class TextureBillboardAnimationLevelRuntimeNodeManager;

    class TextureBillboardAnimationLevel : public SimulationLevel
    {
    public:
        TextureBillboardAnimationLevel();
        virtual ~TextureBillboardAnimationLevel() = default;

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
        static UPtr<TextureBillboardAnimationLevelInitNodeManager> s_init_node_manager;
        static UPtr<TextureBillboardAnimationLevelRuntimeNodeManager> s_runtime_node_manager;
    };


}

