#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
    class SkyCubeLevel;

    class SkyCubeLevelInitNodeManager : public LevelNodeManager<SkyCubeLevel>
    {
    public:
        SkyCubeLevelInitNodeManager();
        virtual ~SkyCubeLevelInitNodeManager() = default;
    };

    class SkyCubeLevelRuntimeNodeManager : public LevelNodeManager<SkyCubeLevel>
    {
    public:
        SkyCubeLevelRuntimeNodeManager();
        virtual ~SkyCubeLevelRuntimeNodeManager() = default;
    };
}



