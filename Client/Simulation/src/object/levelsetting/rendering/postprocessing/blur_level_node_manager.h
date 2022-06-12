#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
	class BlurLevel;

	class BlurLevelInitNodeManager : public LevelNodeManager<BlurLevel>
	{
	public:
		BlurLevelInitNodeManager();
		virtual ~BlurLevelInitNodeManager() = default;
	};

	class BlurLevelRuntimeNodeManager : public LevelNodeManager<BlurLevel>
	{
	public:
		BlurLevelRuntimeNodeManager();
		virtual ~BlurLevelRuntimeNodeManager() = default;
	};
}



