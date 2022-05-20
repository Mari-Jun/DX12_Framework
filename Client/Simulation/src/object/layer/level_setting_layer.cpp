#include <include/client_core.h>
#include <client/imgui/imgui.h>
#include <client/imgui/imgui_helper.h>

#include "object/layer/level_setting_layer.h"

#include "object/level/core/simulation_level.h"

namespace simulation
{
	LevelSettingLayer::LevelSettingLayer()
		: Layer("level setting layer")
	{
	}

	void LevelSettingLayer::Update(float delta_time)
	{
		if (ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			if (m_cur_level != nullptr)
			{
				m_cur_level->ExecuteLevelRuntimeNodes();
			}
			ImGui::End();
		}
	}
}