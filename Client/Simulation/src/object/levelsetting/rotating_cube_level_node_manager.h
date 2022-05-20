#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
	class RotatingCubeLevel;

	class RotatingCubeLevelInitNodeManager : public LevelNodeManager<RotatingCubeLevel>
	{
	public:
		RotatingCubeLevelInitNodeManager();
		virtual ~RotatingCubeLevelInitNodeManager() = default;
	};

	class RotatingCubeLevelRuntimeNodeManager : public LevelNodeManager<RotatingCubeLevel>
	{
	public:
		RotatingCubeLevelRuntimeNodeManager();
		virtual ~RotatingCubeLevelRuntimeNodeManager() = default;
	};
}


