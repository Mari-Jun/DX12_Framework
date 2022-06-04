#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
    class PossessPawnLevel;

    class PossessPawnLevelInitNodeManager : public LevelNodeManager<PossessPawnLevel>
    {
    public:
        PossessPawnLevelInitNodeManager();
        virtual ~PossessPawnLevelInitNodeManager() = default;
    };

    class PossessPawnLevelRuntimeNodeManager : public LevelNodeManager<PossessPawnLevel>
    {
    public:
        PossessPawnLevelRuntimeNodeManager();
        virtual ~PossessPawnLevelRuntimeNodeManager() = default;
    };
}



