#pragma once
#include <map>
#include <queue>
#include <sstream>

#include <include/dx12_shader_fw.h>
#include <client/imgui/imgui.h>
#include <client/imgui/imgui_internal.h>
#include <client/imgui/imgui_helper.h>
#include <client/asset/texture/texture.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>

#include <client/object/layer/core/layer.h>

namespace simulation
{
	using namespace client_fw;

	class SimulationLevel;

	struct SelectLevelButtonInfo
	{
		std::vector<std::string> level_informations;
		std::string num_of_level;
		std::function<void(const std::vector<std::string>&, const std::string&, float)> button_function;

		void operator() (float width) const
		{
			button_function(level_informations, num_of_level, width);
		}
	};

	struct SelectFolderInfo
	{
		std::vector<SelectLevelButtonInfo> buttons_info;
		std::map<std::string, SPtr<SelectFolderInfo>> child_folders;

		void operator() (float width) const
		{
			for (const auto& button : buttons_info)
				button(width);
		}
	};

	class SelectLevelLayer : public Layer
	{
	public:
		SelectLevelLayer(const std::string& name = "select level layer");
		virtual ~SelectLevelLayer() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

	private:
		void RecursiveFolderList(const std::string& folder_name, const SPtr<SelectFolderInfo>& folder_info);
		void UpdateLevelList();

	public:
		template<typename Level>
		void RegisterLevel(const std::string& folder_path, std::vector<std::string>&& informations)
		{
			if (folder_path.empty() == false)
			{
				auto SplitPath = [](const std::string& path, char delimiter)
				{
					std::istringstream iss{ path };

					std::string buffer;

					std::queue<std::string> ret;

					while (getline(iss, buffer, delimiter))
						ret.push(buffer);

					return ret;
				};

				std::queue<std::string> folders = SplitPath(folder_path, '/');

				SPtr<SelectFolderInfo> select_folder = m_registered_levels_folder;

				while (folders.empty() == false)
				{
					std::string folder_name = folders.front();
					folders.pop();

					if (select_folder->child_folders.find(folder_name) == select_folder->child_folders.cend())
						select_folder->child_folders[folder_name] = CreateSPtr<SelectFolderInfo>();

					select_folder = select_folder->child_folders[folder_name];
				}

				static auto LevelButtonFunction = [this](const std::vector<std::string>& infos,
					const std::string& num_of_level, float width)
				{
					auto level = CreateSPtr<Level>();

					ImGui::BeginGroup();
					ImGui::ImageButton((ImTextureID)m_level_texture->GetGPUAddress(), ImVec2(width, width));
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltipEx(ImGuiTooltipFlags_None, ImGuiWindowFlags_AlwaysAutoResize);
						ImGui::PushFont(ImGuiHelper::s_jua_20);
						ImGui::Text(level->GetName().c_str());
						ImGui::PopFont();
						ImGui::Separator();
						for (const auto& info : infos)
							ImGui::BulletText(info.c_str());
						ImGui::EndTooltip();

						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							m_selected_level = level;
							m_selected_level->SetLevelInitNodeOwner();
							ImGui::OpenPopup(("Level Initialize Setting##" + num_of_level).c_str());
						}
					}
					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + width);
					ImGui::Text(level->GetName().c_str());
					ImGui::PopTextWrapPos();
					ImGui::EndGroup();
						
					ImGuiStyle& style = ImGui::GetStyle();

					ImGuiViewport* viewport = ImGui::GetMainViewport();
					ImVec2 size = viewport->WorkSize;
					size.x *= 0.3f; size.y *= 0.9f;
					ImGui::SetNextWindowSize(size);
					ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

					if (ImGui::BeginPopupModal(("Level Initialize Setting##" + num_of_level).c_str(), NULL,
						ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
					{
						m_selected_level->ExecuteLevelInitNodes();

						ImGui::Separator();

						ImVec4 rect = ImGuiHelper::GetImGuiWindowBoxSize();

						float width = rect.z * 0.8f;
						width += style.ItemSpacing.x;
						ImGuiHelper::AlignFormWidth(width);

						if (ImGui::Button(("OK##" + num_of_level).c_str(), ImVec2(rect.z * 0.4f, 0.f)))
						{
							LevelManager::GetLevelManager().OpenLevel(m_selected_level, nullptr);
							if (m_open_event != nullptr)
							{
								m_selected_level->SetLevelRuntimeNodeOwner();
								m_open_event(m_selected_level);
							}
							m_selected_level = nullptr;
							ImGui::CloseCurrentPopup();
						}
						ImGui::SameLine();
						if (ImGui::Button(("Cancel##" + num_of_level).c_str(), ImVec2(rect.z * 0.4f, 0.f)))
						{
							ImGui::CloseCurrentPopup();
							m_selected_level = nullptr;
						}
						ImGui::EndPopup();
					}
				};

				std::string num_of_level = std::to_string(m_num_of_level++);
				select_folder->buttons_info.push_back(SelectLevelButtonInfo{ informations, num_of_level, LevelButtonFunction });
			}
		}

	private:
		INT m_num_of_level = 0;
		SPtr<SimulationLevel> m_selected_level;
		SPtr<SelectFolderInfo> m_registered_levels_folder;
		SPtr<SelectFolderInfo> m_selected_folder;

		SPtr<Texture> m_level_texture;

		std::function<void(const SPtr<SimulationLevel>&)> m_open_event;
		std::function<void(const SPtr<SimulationLevel>&)> m_close_event;

	public:
		void OnOpenEvent(std::function<void(const SPtr<SimulationLevel>&)>&& function) { m_open_event = function; }
		void OnCloseEvent(std::function<void(const SPtr<SimulationLevel>&)>&& function) { m_close_event = function; }
	};
}



