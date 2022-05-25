#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
    class ImportObjMeshLevel;

    class ImportObjMeshLevelInitNodeManager : public LevelNodeManager<ImportObjMeshLevel>
    {
    public:
        ImportObjMeshLevelInitNodeManager();
        virtual ~ImportObjMeshLevelInitNodeManager() = default;
    };

    class ImportObjMeshLevelRuntimeNodeManager : public LevelNodeManager<ImportObjMeshLevel>
    {
    public:
        ImportObjMeshLevelRuntimeNodeManager();
        virtual ~ImportObjMeshLevelRuntimeNodeManager() = default;
    };
}