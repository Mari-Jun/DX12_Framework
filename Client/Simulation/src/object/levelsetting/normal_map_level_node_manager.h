#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
    class NormalMapLevel;

    class NormalMapLevelInitNodeManager : public LevelNodeManager<NormalMapLevel>
    {
    public:
        NormalMapLevelInitNodeManager();
        virtual ~NormalMapLevelInitNodeManager() = default;
    };

    class NormalMapLevelRuntimeNodeManager : public LevelNodeManager<NormalMapLevel>
    {
    public:
        NormalMapLevelRuntimeNodeManager();
        virtual ~NormalMapLevelRuntimeNodeManager() = default;
    };
}


