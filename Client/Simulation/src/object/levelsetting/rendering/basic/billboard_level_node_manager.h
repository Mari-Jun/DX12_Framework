#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
	class TextureBillboardLevel;

	class TextureBillboardLevelInitNodeManager : public LevelNodeManager<TextureBillboardLevel>
	{
	public:
		TextureBillboardLevelInitNodeManager();
		virtual ~TextureBillboardLevelInitNodeManager() = default;
	};

	class TextureBillboardLevelRuntimeNodeManager : public LevelNodeManager<TextureBillboardLevel>
	{
	public:
		TextureBillboardLevelRuntimeNodeManager();
		virtual ~TextureBillboardLevelRuntimeNodeManager() = default;
	};

	class TextureBillboardAnimationLevel;

	class TextureBillboardAnimationLevelInitNodeManager : public LevelNodeManager<TextureBillboardAnimationLevel>
	{
	public:
		TextureBillboardAnimationLevelInitNodeManager();
		virtual ~TextureBillboardAnimationLevelInitNodeManager() = default;
	};

	class TextureBillboardAnimationLevelRuntimeNodeManager : public LevelNodeManager<TextureBillboardAnimationLevel>
	{
	public:
		TextureBillboardAnimationLevelRuntimeNodeManager();
		virtual ~TextureBillboardAnimationLevelRuntimeNodeManager() = default;

	private:
		bool m_show_animation = true;
		float m_animation_period = 1.0f;
		Vec2 m_previous_tilling;

	public:
		bool IsShowAnimation() const { return m_show_animation; }
		float GetAnimationPeriod() const { return m_animation_period; }
	};

}


