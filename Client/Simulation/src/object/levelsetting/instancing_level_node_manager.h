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
    };

    class InstancingLevelRuntimeNodeManager : public LevelNodeManager<InstancingLevel>
    {
    public:
        InstancingLevelRuntimeNodeManager();
        virtual ~InstancingLevelRuntimeNodeManager() = default;
    };
}



