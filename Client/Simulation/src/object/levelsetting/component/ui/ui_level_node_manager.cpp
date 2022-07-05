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


		static auto TextUIFunc = [](const SPtr<UILevel>& level)
		{
			const auto& text_ui = level->GetUILayer()->GetText();

			Vec2 pos = text_ui->GetPosition();
			if (ImGui::DragFloat2("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
				text_ui->SetPosition(pos);

			Vec2 size = text_ui->GetSize();
			if (ImGui::DragFloat2("Size", (float*)&size, 0.5f, 1.0f, 4096.0f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				text_ui->SetSize(size);

			std::string text = text_ui->GetText();
			if (ImGui::InputText("Text", &text))
				text_ui->SetText(text);

			Vec4 text_color = text_ui->GetColor();
			if (ImGui::ColorEdit4("Text color", (float*)&text_color))
				text_ui->SetColor(text_color);

			std::string font_name = utf8_encode(text_ui->GetFontName());
			if (ImGui::InputText("Font name", &font_name))
				text_ui->SetFontName(utf8_decode(font_name));

			int font_size = text_ui->GetFontSize();
			if (ImGui::DragInt("Font size", &font_size, 1.0f, 1, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp))
				text_ui->SetFontSize(font_size);

			auto TextUIComboSetting = [text_ui]
			(const auto& infos, const auto& search_info, const char* combo_name, int& select_index, std::function<void(const SPtr<TextUI>&)> func)
			{
				select_index = static_cast<int>(std::distance(infos.begin(),
					std::find_if(infos.begin(), infos.end(), [search_info](const auto& info)
						{ return info.second == search_info; })
				));

				if (ImGui::BeginCombo(combo_name, infos[select_index].first.data()))
				{
					for (int index = 0; index < infos.size(); ++index)
					{
						bool is_selected = (select_index == index);
						if (ImGui::Selectable(infos[index].first.data(), is_selected))
						{
							select_index = index;
							func(text_ui);
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			};

			static int select_weight_index = 0;
			static const std::array<std::pair<std::string, DWRITE_FONT_WEIGHT>, 11> weights = {
				std::pair{"Thin", DWRITE_FONT_WEIGHT_THIN}, {"Extra light", DWRITE_FONT_WEIGHT_EXTRA_LIGHT}, {"Light", DWRITE_FONT_WEIGHT_LIGHT},
				{"Semi light", DWRITE_FONT_WEIGHT_SEMI_LIGHT}, {"Regular", DWRITE_FONT_WEIGHT_REGULAR}, {"Medium", DWRITE_FONT_WEIGHT_MEDIUM},
				{"Semi bold", DWRITE_FONT_WEIGHT_SEMI_BOLD}, {"Bold", DWRITE_FONT_WEIGHT_BOLD}, {"Extra bold", DWRITE_FONT_WEIGHT_EXTRA_BOLD},
				{"Heavy", DWRITE_FONT_WEIGHT_HEAVY} , {"Extra black", DWRITE_FONT_WEIGHT_EXTRA_BLACK} 
			};

			TextUIComboSetting(weights, text_ui->GetFontWeight(), "Weights", select_weight_index,
				[](const SPtr<TextUI>& text_ui) { text_ui->SetFontWeight(weights[select_weight_index].second); });

			static int select_style_index = 0;
			static const std::array<std::pair<std::string, DWRITE_FONT_STYLE>, 3> styles = {
				std::pair{"Normal", DWRITE_FONT_STYLE_NORMAL},
				{"Oblique", DWRITE_FONT_STYLE_OBLIQUE}, 
				{"Italic", DWRITE_FONT_STYLE_ITALIC} 
			};

			TextUIComboSetting(styles, text_ui->GetFontStyle(), "Style", select_style_index,
				[](const SPtr<TextUI>& text_ui) { text_ui->SetFontSytle(styles[select_style_index].second); });

			static int select_text_alignment_index = 0;
			static const std::array<std::pair<std::string, DWRITE_TEXT_ALIGNMENT>, 4> text_alignments = {
				std::pair{"Alignment leading", DWRITE_TEXT_ALIGNMENT_LEADING},
				{"Alignment trailing", DWRITE_TEXT_ALIGNMENT_TRAILING},
				{"Alignment center", DWRITE_TEXT_ALIGNMENT_CENTER},
				{"Alignment justified", DWRITE_TEXT_ALIGNMENT_JUSTIFIED}
			};

			TextUIComboSetting(text_alignments, text_ui->GetTextAlignment(), "Text alignment", select_text_alignment_index,
				[](const SPtr<TextUI>& text_ui) { text_ui->SetTextAlignment(text_alignments[select_text_alignment_index].second); });

			static int select_paragraph_alignment_index = 0;
			static const std::array<std::pair<std::string, DWRITE_PARAGRAPH_ALIGNMENT>, 3> paragraph_alignments = {
				std::pair{"Alignment near", DWRITE_PARAGRAPH_ALIGNMENT_NEAR},
				{"Alignment far", DWRITE_PARAGRAPH_ALIGNMENT_FAR},
				{"Alignment center", DWRITE_PARAGRAPH_ALIGNMENT_CENTER}
			};

			TextUIComboSetting(paragraph_alignments, text_ui->GetParagraphAlignment(), "Paragraph alignment", select_paragraph_alignment_index,
				[](const SPtr<TextUI>& text_ui) { text_ui->SetParagraphAlignment(paragraph_alignments[select_paragraph_alignment_index].second); });
		};

		RegisterSettingHeaderNode("Text UI", {
			{ "text", TextUIFunc },
			});
	}
}