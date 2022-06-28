#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/asset/texture/texture.h>
#include <client/util/file_dialog.h>
#include <client/object/ui/button_ui.h>
#include <client/object/ui/text_ui.h>

#include "object/level/component/ui/ui_level.h"
#include "object/levelsetting/component/ui/ui_level_node_manager.h"
#include "object/ui/ui_level_ui_layer.h"

namespace simulation
{
	UILevelInitNodeManager::UILevelInitNodeManager()
	{
	}

	UILevelRuntimeNodeManager::UILevelRuntimeNodeManager()
	{
		static auto ButtonUIFunc = [](const SPtr<UILevel>& level)
		{
			const auto& button_ui = level->GetUILayer()->GetButton();

			Vec2 pos = button_ui->GetPosition();
			if (ImGui::DragFloat2("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
				button_ui->SetPosition(pos);

			Vec2 size = button_ui->GetSize();
			if (ImGui::DragFloat2("Size", (float*)&size, 0.5f, 0.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				button_ui->SetSize(size);

			const auto& buttons = button_ui->GetButtonTextures();

			for (const auto& [type, button] : buttons)
			{
				std::string tree_name;

				switch (type)
				{
				case eButtonState::kNormal: tree_name = "normal button"; break;
				case eButtonState::kHovered: tree_name = "hovered button"; break;
				case eButtonState::kPressed: tree_name = "pressed button"; break;
				default: break;
				}

				if (ImGui::TreeNodeEx(tree_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					const auto& texture = button->GetTexture();
					std::string path;
					if (texture != nullptr)
						path = button->GetTexture()->GetPath();

					ImGui::BulletText("Texture path : ");
					ImGui::SameLine();
					ImGui::Text(path.c_str());

					if (ImGui::Button("Open file dialog"))
					{
						path = FileDialog::OpenFile(L"texture file (.png .dds .bmp .jpg)\0*.png;*.dds;*.bmp;*.jpg");
						if (path.empty() == false)
							button->SetTexture(path);
					}

					Vec4 brush_color = button->GetBrushColor();
					if (ImGui::ColorEdit4("Brush Color", (float*)&brush_color))
						button->SetBrushColor(brush_color);

					Vec2 coordinate = button->GetCoordinate();
					if (ImGui::DragFloat2("Offset", (float*)&coordinate, 0.01f, -FLT_MAX, FLT_MAX, "%.4f"))
						button->SetCoordinate(coordinate);

					Vec2 tilling = button->GetTilling();
					if (ImGui::DragFloat2("Tilling", (float*)&tilling, 0.01f, 0.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
						button->SetTilling(tilling);

					ImGui::TreePop();
					ImGui::Separator();
				}
			}
		};
		
		RegisterSettingHeaderNode("Button UI", {
			{ "button", ButtonUIFunc },
			});
	}
}
