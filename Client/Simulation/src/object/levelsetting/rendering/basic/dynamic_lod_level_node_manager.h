#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
    class DynamicLODLevel;

    class DynamicLODLevelInitNodeManager : public LevelNodeManager<DynamicLODLevel>
    {
    public:
        DynamicLODLevelInitNodeManager();
        virtual ~DynamicLODLevelInitNodeManager() = default;
    };

    class DynamicLODLevelRuntimeNodeManager : public LevelNodeManager<DynamicLODLevel>
    {
    public:
        DynamicLODLevelRuntimeNodeManager();
        virtual ~DynamicLODLevelRuntimeNodeManager() = default;
    };

}



