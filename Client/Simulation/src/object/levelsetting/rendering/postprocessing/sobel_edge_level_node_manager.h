#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
	class SobelEdgeLevel;

	class SobelEdgeLevelInitNodeManager :public LevelNodeManager<SobelEdgeLevel>
	{
	public:
		SobelEdgeLevelInitNodeManager();
		virtual ~SobelEdgeLevelInitNodeManager() = default;
	};

	class SobelEdgeLevelRuntimeNodeManager :public LevelNodeManager<SobelEdgeLevel>
	{
	public:
		SobelEdgeLevelRuntimeNodeManager();
		virtual ~SobelEdgeLevelRuntimeNodeManager() = default;
	};
}


