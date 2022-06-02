#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
    class InstancingLevel;

    class InstancingLevelInitNodeManager : public LevelNodeManager<InstancingLevel>
    {
    public:
        InstancingLevelInitNodeManager();
        virtual ~InstancingLevelInitNodeManager() = default;

    private:
        UINT m_num_of_x_actors = 10;
        UINT m_num_of_y_actors = 10;
        UINT m_num_of_z_actors = 10;
        float m_offset = 500.f;

    public:
        UINT GetNumOfXActors() const { return m_num_of_x_actors; }
        UINT GetNumOfYActors() const { return m_num_of_y_actors; }
        UINT GetNumOfZActors() const { return m_num_of_z_actors; }
        float GetOffset() const { return m_offset; }
    };

    class InstancingLevelRuntimeNodeManager : public LevelNodeManager<InstancingLevel>
    {
    public:
        InstancingLevelRuntimeNodeManager();
        virtual ~InstancingLevelRuntimeNodeManager() = default;
    };
}



