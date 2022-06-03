#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
	class GBufferLevel;

	class GBufferLevelInitNodeManager : public LevelNodeManager<GBufferLevel>
	{
	public:
		GBufferLevelInitNodeManager();
		virtual ~GBufferLevelInitNodeManager() = default;
	};

	class GBufferLevelRuntimeNodeManager : public LevelNodeManager<GBufferLevel>
	{
	public:
		GBufferLevelRuntimeNodeManager();
		virtual ~GBufferLevelRuntimeNodeManager() = default;
	};


}

