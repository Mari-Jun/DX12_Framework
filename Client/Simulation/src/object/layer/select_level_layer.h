#pragma once
#include <client/imgui/imgui.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>

#include "client/object/layer/core/layer.h"

namespace simulation
{
	using namespace client_fw;

	struct SelectHeaderInfo
	{
		std::function<void(const std::vector<std::string>&)> m_header_function;
		std::vector<std::string> level_informations;

		void operator() () const
		{
		
			m_header_function(level_informations);
		}
	};

	class SelectLevelLayer : public Layer
	{
	public:
		SelectLevelLayer(const std::string& name = "select level layer");
		virtual ~SelectLevelLayer() = default;

		virtual void Update(float delta_time) override;

		template<typename Level>
		void RegisterLevel(std::vector<std::string>&& informations)
		{
			auto header_function = [this](const std::vector<std::string>& infos)
			{
				auto level = CreateSPtr<Level>();

				if (ImGui::CollapsingHeader(level->GetName().c_str()))
				{
					std::string num_of_level = std::to_string(m_registered_levels_header.size());

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
						LevelManager::GetLevelManager().OpenLevel(level, nullptr);
					}
				}
			};

			m_registered_levels_header.push_back({ header_function, informations });
		}

	private:
		std::vector<SelectHeaderInfo> m_registered_levels_header;
	};	
}



