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

	private:
		Vec3 m_init_pos = Vec3(0.f, 0.f, 500.f);
		Vec3 m_init_scale = Vec3(1.f, 1.f, 1.f);
		bool m_enable_dir_light_shadow = false;

	public:
		const Vec3& GetInitPos() const { return m_init_pos; }
		const Vec3& GetInitScale() const { return m_init_scale; }
		bool EnableDirLightShadow() const { return m_enable_dir_light_shadow; }
	};

	class RotatingCubeLevelRuntimeNodeManager : public LevelNodeManager<RotatingCubeLevel>
	{
	public:
		RotatingCubeLevelRuntimeNodeManager();
		virtual ~RotatingCubeLevelRuntimeNodeManager() = default;
	};
}


