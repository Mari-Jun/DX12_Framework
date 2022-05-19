#include <include/client_core.h>
#include <client/imgui/imgui.h>

#include "object/layer/imgui_demo_layer.h"


namespace simulation
{
	ImGuiDemoLayer::ImGuiDemoLayer(const std::string& name)
		: Layer(name)
	{
	}

	void ImGuiDemoLayer::Update(float delta_time)
	{
		ImGui::ShowDemoWindow();
	}
}
