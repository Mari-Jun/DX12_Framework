#include <include/client_core.h>
#include <client/imgui/imgui.h>
#include <client/imgui/imgui_helper.h>
#include <client/asset/core/asset_store.h>
#include <client/object/level/core/level_manager.h>

#include "object/layer/select_level_layer.h"

namespace simulation
{
	SelectLevelLayer::SelectLevelLayer(const std::string& name)
		: Layer(name)
	{
		m_registered_levels_folder = CreateSPtr<SelectFolderInfo>();
	}

	bool SelectLevelLayer::Initialize()
	{
		m_level_texture = AssetStore::LoadTexture("Contents/Layer/level_texture.png");
		return true;
	}

	void SelectLevelLayer::Update(float delta_time)
	{
		if (ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			if (ImGui::BeginChild("Folder List", ImVec2(0, 200), true))
			{
				for (const auto& [folder_name, folder_info] : m_registered_levels_folder->child_folders)
				{
					RecursiveFolderList(folder_name, folder_info);
				}
			}
			ImGui::EndChild();


			if (ImGui::BeginChild("Level List", ImVec2(0, 0), true))
			{
				UpdateLevelList();
			}
			ImGui::EndChild();

		}
		ImGui::End();
	}

	void SelectLevelLayer::UpdateLevelList()
	{
		if (m_selected_folder != nullptr)
		{
			ImVec4 rect = ImGuiHelper::GetImGuiWindowSize();
			static float width = 90.0f;
			float window_width = rect.z;

			if (window_width > 0.0f)
			{
				window_width -= ImGui::GetStyle().WindowPadding.x * 2 + ImGui::GetStyle().FramePadding.x * 2;

				int line = static_cast<int>(window_width / width);
				int count = 0;

				if (line != 0)
				{
					float button_width = (window_width - ((line - 1) * ImGui::GetStyle().ItemSpacing.x * 2)) / line;

					for (const auto& button : m_selected_folder->buttons_info)
					{
						button(button_width);
						if (++count % line != 0)
							ImGui::SameLine();
					}
				}
				else
				{
					for (const auto& button : m_selected_folder->buttons_info)
					{
						button(window_width);
					}
				}
			}
		}
	}

	void SelectLevelLayer::RecursiveFolderList(const std::string& folder_name, const SPtr<SelectFolderInfo>& folder_info)
	{
		static ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_FramePadding |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Selected;

		if (ImGui::TreeNodeEx(folder_name.c_str(), tree_flags))
		{
			if (ImGui::IsItemClicked())
				m_selected_folder = folder_info;
			for (const auto& [child_folder_name, child_folder_info] : folder_info->child_folders)
			{
				RecursiveFolderList(child_folder_name, child_folder_info);
			}
			ImGui::TreePop();
		}
	}
}