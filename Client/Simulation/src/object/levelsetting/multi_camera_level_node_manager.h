#pragma once
#include "object/levelsetting/core/level_setting_manager.h"
#include <bitset>

namespace simulation
{
    class MultiCameraLevel;

    class MultiCameraLevelInitNodeManager : public LevelNodeManager<MultiCameraLevel>
    {
    public:
        MultiCameraLevelInitNodeManager();
        virtual ~MultiCameraLevelInitNodeManager() = default;

    private:
        UINT m_num_of_camera = 6;
        float m_camera_offset = 500.0f;

    public:
        UINT GetNumOfCamera() const { return m_num_of_camera; }
        float GetCameraOffset() const { return m_camera_offset; }
    };

    class MultiCameraLevelRuntimeNodeManager : public LevelNodeManager<MultiCameraLevel>
    {
    public:
        MultiCameraLevelRuntimeNodeManager();
        virtual ~MultiCameraLevelRuntimeNodeManager() = default;
    };
}



