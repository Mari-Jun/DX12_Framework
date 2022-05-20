#pragma once
#include <client/imgui/imgui.h>
#include <client/imgui/imgui_helper.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>

#include <client/object/layer/core/layer.h>

namespace simulation
{
	using namespace client_fw;

	struct SelectHeaderInfo
	{
		std::function<void(const std::vector<std::string>&, const std::string&)> header_function;
		std::vector<std::string> level_informations;
		std::string num_of_level;

		void operator() () const
		{
			header_function(level_informations, num_of_level);
		}
	};

	class SimulationLevel;

	class SelectLevelLayer : public Layer
	{
	public:
		SelectLevelLayer(const std::string& name = "select level layer");
		virtual ~SelectLevelLayer() = default;

		virtual void Update(float delta_time) override;

		template<typename Level>
		void RegisterLevel(std::vector<std::string>&& informations)
		{
			auto header_function = [this](const std::vector<std::string>& infos, const std::string& num_of_level)
			{
				auto level = CreateSPtr<Level>();

				if (ImGui::CollapsingHeader(level->GetName().c_str()))
				{
					size_t info_size = std::min(infos.size(), static_cast<size_t>(5));

					ImGuiStyle& style = ImGui::GetStyle();
					float childHeight = ImGui::GetTextLineHeight() * info_size + style.ItemSpacing.y * (info_size - 1) +
						style.ScrollbarSize + style.WindowPadding.y * 2.0f;
					ImGui::BeginChild((level->GetName() + num_of_level).c_str(), ImVec2(0, childHeight), true, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					for (const auto& info : infos)
					{
						ImGui::BulletText(info.c_str());
					}
					ImGui::PopStyleVar();
					ImGui::EndChild();

					if (ImGui::Button(("Open Level##" + num_of_level).c_str(), ImVec2(-FLT_MIN, 0)))
					{
						m_selected_level = level;
						m_selected_level->SetLevelInitNodeOwner();
						ImGui::OpenPopup(("Level Initialize Setting##" + num_of_level).c_str());
					}

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
						ImGuiHelper::AlignFormWitdh(width);

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
				}
			};

			std::string num_of_level = std::to_string(m_registered_levels_header.size());

			m_registered_levels_header.push_back({ header_function, informations, num_of_level });
		}

	private:
		SPtr<SimulationLevel> m_selected_level;
		std::vector<SelectHeaderInfo> m_registered_levels_header;
		std::function<void(const SPtr<SimulationLevel>&)> m_open_event;
		std::function<void(const SPtr<SimulationLevel>&)> m_close_event;

	public:
		void OnOpenEvent(std::function<void(const SPtr<SimulationLevel>&)>&& function) { m_open_event = function; }
		void OnCloseEvent(std::function<void(const SPtr<SimulationLevel>&)>&& function) { m_close_event = function; }
	};	
}



