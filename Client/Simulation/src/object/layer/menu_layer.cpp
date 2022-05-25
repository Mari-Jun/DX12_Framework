#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/imgui/imgui.h>
#include <client/imgui/imgui_helper.h>
#include <client/imgui/imgui_internal.h>
#include <client/asset/texture/texture.h>
#include <client/asset/core/asset_store.h>

#include "object/layer/menu_layer.h"

namespace simulation
{
	MenuLayer::MenuLayer()
		: Layer("menu layer")
	{
	}

	bool MenuLayer::Initialize()
	{
		m_log_texture = AssetStore::LoadTexture("Contents/Layer/log.png");
		m_resume_level_texture = AssetStore::LoadTexture("Contents/Layer/resume_level.png");
		m_pause_level_texture = AssetStore::LoadTexture("Contents/Layer/pause_level.png");
		m_close_level_texture = AssetStore::LoadTexture("Contents/Layer/close_level.png");
		m_shutdown_texture = AssetStore::LoadTexture("Contents/Layer/shutdown.png");
		return true;
	}

	void MenuLayer::Update(float delta_time)
	{
		static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersOuter |
			ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

		static float width = 60.0f;

		static auto ShowMenuButton = [](const SPtr<Texture>& texture, const std::string& text, const std::function<void()>& click_event)
		{
			ImGui::BeginGroup();
			if (ImGui::ImageButton((ImTextureID)texture->GetGPUAddress(), ImVec2(width, width)))
			{
				if(click_event != nullptr)
					click_event();
			}
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + width);
			ImGui::Text(text.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndGroup();

			ImGui::SameLine();
		};



		if (ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar))
		{
			ShowMenuButton(m_log_texture, "Log", []() {});
			if (m_is_level_pause)
				ShowMenuButton(m_resume_level_texture, "Resume", m_resume_level_event);
			else
				ShowMenuButton(m_pause_level_texture, "Pause", m_pause_level_event);
			ShowMenuButton(m_close_level_texture, "Close", m_close_level_event);
			ShowMenuButton(m_shutdown_texture, "Shutdown", m_shutdown_event);
		}
		ImGui::End();
	}
}