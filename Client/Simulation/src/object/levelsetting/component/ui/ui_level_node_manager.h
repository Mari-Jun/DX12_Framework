#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
	class UILevel;

	class UILevelInitNodeManager : public LevelNodeManager<UILevel>
	{
	public:
		UILevelInitNodeManager();
		virtual ~UILevelInitNodeManager() = default;
	};

	class UILevelRuntimeNodeManager : public LevelNodeManager<UILevel>
	{
	public:
		UILevelRuntimeNodeManager();
		virtual ~UILevelRuntimeNodeManager() = default;

	private:
	};
}


