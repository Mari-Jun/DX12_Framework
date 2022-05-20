#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
	class RotatingCubeLevel;

	class RotatingCubeLevelInitNodeManager : public LevelInitNodeManager<RotatingCubeLevel>
	{
	public:
		RotatingCubeLevelInitNodeManager();
		virtual ~RotatingCubeLevelInitNodeManager() = default;

	};

}


