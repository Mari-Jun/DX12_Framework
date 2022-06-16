#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
	class TextureBillboardLevel;

	class BillboardLevelInitNodeManager : public LevelNodeManager<TextureBillboardLevel>
	{
	public:
		BillboardLevelInitNodeManager();
		virtual ~BillboardLevelInitNodeManager() = default;
	};

	class BillboardLevelRuntimeNodeManager : public LevelNodeManager<TextureBillboardLevel>
	{
	public:
		BillboardLevelRuntimeNodeManager();
		virtual ~BillboardLevelRuntimeNodeManager() = default;
	};

}


