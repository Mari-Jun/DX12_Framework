#include <include/client_core.h>
#include <client/imgui/imgui.h>
#include <client/imgui/imgui_helper.h>
#include <client/object/level/core/level_manager.h>

#include "object/layer/select_level_layer.h"

namespace simulation
{
	SelectLevelLayer::SelectLevelLayer(const std::string& name)
		: Layer(name)
	{
	}

	void SelectLevelLayer::Update(float delta_time)
	{
		if (ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			for (const auto& header : m_registered_levels_header)
			{
				header();
			}
		}
		ImGui::End();
	}
}