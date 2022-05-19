#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/input/input.h>
#include <client/imgui/imgui.h>
#include <client/imgui/imgui_helper.h>
#include <client/renderer/core/render.h>
#include <client/renderer/core/render_viewport.h>
#include <client/asset/texture/texture.h>

#include "object/layer/viewport_layer.h"

namespace simulation
{
	IVec2 ViewportLayer::s_viewport_pos = IVec2(0, 0);
	IVec2 ViewportLayer::s_viewport_size = IVec2(0, 0);

	ViewportLayer::ViewportLayer(const std::string& name)
		: Layer(name)
	{
	}

	void ViewportLayer::Update(float delta_time)
	{
		if (ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar))
		{
			ImVec4 rect = ImGuiHelper::GetImGuiWindowBoxSize();

			IVec2 pos(static_cast<INT>(rect.x), static_cast<INT>(rect.y));
			IVec2 size(static_cast<INT>(rect.z), static_cast<INT>(rect.w));

			if (s_viewport_size == IVec2(0, 0) || (s_viewport_size != size && Input::IsKeyReleased(eKey::kLButton)))
			{
				s_viewport_size = size;
				Render::EnableIngameViewport(pos, size);
			}

			const auto& ingame_viewport = Render::GetIngameViewport();
			if (ingame_viewport != nullptr)
			{
				const auto& texture = ingame_viewport->GetViewportTexture();
				if (texture != nullptr && texture->GetResource() != nullptr)
				{
					ImGui::Image((ImTextureID)texture->GetGPUAddress(), ImVec2(rect.z, rect.w));
				}
			}
		}
		ImGui::End();
	}
}
