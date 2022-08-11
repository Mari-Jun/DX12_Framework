#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/asset/texture/texture.h>
#include <client/util/file_dialog.h>
#include <client/object/ui/button_ui.h>
#include <client/object/ui/text_ui.h>
#include <client/object/ui/image_ui.h>
#include <client/object/ui/progress_bar_ui.h>
#include <client/object/ui/text_box_ui.h>

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

			Vec2 pivot = button_ui->GetPivot();
			if (ImGui::DragFloat2("Pivot", (float*)&pivot, 0.01f, 0.0f, 1.0f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				button_ui->SetPivot(pivot);

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

			Vec2 pivot = text_ui->GetPivot();
			if (ImGui::DragFloat2("Pivot", (float*)&pivot, 0.01f, 0.0f, 1.0f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				text_ui->SetPivot(pivot);

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

		static auto ImageUIFunc = [](const SPtr<UILevel>& level)
		{
			const auto& image_ui = level->GetUILayer()->GetImage();

			Vec2 pos = image_ui->GetPosition();
			if (ImGui::DragFloat2("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
				image_ui->SetPosition(pos);

			Vec2 size = image_ui->GetSize();
			if (ImGui::DragFloat2("Size", (float*)&size, 0.5f, 0.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				image_ui->SetSize(size);

			Vec2 pivot = image_ui->GetPivot();
			if (ImGui::DragFloat2("Pivot", (float*)&pivot, 0.01f, 0.0f, 1.0f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				image_ui->SetPivot(pivot);

			const auto& texture = image_ui->GetTexture();
			std::string path;
			if (texture != nullptr)
				path = texture->GetPath();

			ImGui::BulletText("Texture path : ");
			ImGui::SameLine();
			ImGui::Text(path.c_str());

			if (ImGui::Button("Open file dialog"))
			{
				path = FileDialog::OpenFile(L"texture file (.png .dds .bmp .jpg)\0*.png;*.dds;*.bmp;*.jpg");
				if (path.empty() == false)
					image_ui->SetTexture(path);
			}

			Vec4 brush_color = image_ui->GetTextureBrushColor();
			if (ImGui::ColorEdit4("Brush Color", (float*)&brush_color))
				image_ui->SetTextureBrushColor(brush_color);

			Vec2 coordinate = image_ui->GetTextureCoordinate();
			if (ImGui::DragFloat2("Offset", (float*)&coordinate, 0.01f, -FLT_MAX, FLT_MAX, "%.4f"))
				image_ui->SetTextureCoordinate(coordinate);

			Vec2 tilling = image_ui->GetTextureTilling();
			if (ImGui::DragFloat2("Tilling", (float*)&tilling, 0.01f, 0.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				image_ui->SetTextureTilling(tilling);
		};

		RegisterSettingHeaderNode("Image UI", {
			{ "image", ImageUIFunc },
			});

		static auto ProgressBarUIFunc = [](const SPtr<UILevel>& level)
		{
			const auto& progress_bar_ui = level->GetUILayer()->GetProgressBar();

			Vec2 pos = progress_bar_ui->GetPosition();
			if (ImGui::DragFloat2("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
				progress_bar_ui->SetPosition(pos);

			Vec2 size = progress_bar_ui->GetSize();
			if (ImGui::DragFloat2("Size", (float*)&size, 0.5f, 0.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				progress_bar_ui->SetSize(size);

			Vec2 pivot = progress_bar_ui->GetPivot();
			if (ImGui::DragFloat2("Pivot", (float*)&pivot, 0.01f, 0.0f, 1.0f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				progress_bar_ui->SetPivot(pivot);

			float percent = progress_bar_ui->GetPercent();
			percent *= 100.0f;
			if (ImGui::DragFloat("Percent", &percent, 0.1f, 0.0f, 100.f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				progress_bar_ui->SetPercent(percent * 0.01f);

			if (ImGui::TreeNodeEx("Background texture", ImGuiTreeNodeFlags_DefaultOpen))
			{
				const auto& texture = progress_bar_ui->GetBackgroundTexture();
				std::string path;
				if (texture != nullptr)
					path = texture->GetPath();

				ImGui::BulletText("Texture path : ");
				ImGui::SameLine();
				ImGui::Text(path.c_str());

				if (ImGui::Button("Open file dialog"))
				{
					path = FileDialog::OpenFile(L"texture file (.png .dds .bmp .jpg)\0*.png;*.dds;*.bmp;*.jpg");
					if (path.empty() == false)
						progress_bar_ui->SetBackgroundTexture(path);
				}

				Vec4 brush_color = progress_bar_ui->GetBackgroundTextureBrushColor();
				if (ImGui::ColorEdit4("Brush Color", (float*)&brush_color))
					progress_bar_ui->SetBackgroundTextureBrushColor(brush_color);

				Vec2 coordinate = progress_bar_ui->GetBackgroundTextureCoordinate();
				if (ImGui::DragFloat2("Offset", (float*)&coordinate, 0.01f, -FLT_MAX, FLT_MAX, "%.4f"))
					progress_bar_ui->SetBackgroundTextureCoordinate(coordinate);

				Vec2 tilling = progress_bar_ui->GetBackgroundTextureTilling();
				if (ImGui::DragFloat2("Tilling", (float*)&tilling, 0.01f, 0.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
					progress_bar_ui->SetBackgroundTextureTilling(tilling);

				ImGui::TreePop();
				ImGui::Separator();
			}

			if (ImGui::TreeNodeEx("Fill texture", ImGuiTreeNodeFlags_DefaultOpen))
			{
				const auto& texture = progress_bar_ui->GetFillTexture();
				std::string path;
				if (texture != nullptr)
					path = texture->GetPath();

				ImGui::BulletText("Texture path : ");
				ImGui::SameLine();
				ImGui::Text(path.c_str());

				if (ImGui::Button("Open file dialog"))
				{
					path = FileDialog::OpenFile(L"texture file (.png .dds .bmp .jpg)\0*.png;*.dds;*.bmp;*.jpg");
					if (path.empty() == false)
						progress_bar_ui->SetFillTexture(path);
				}

				Vec4 brush_color = progress_bar_ui->GetFillTextureBrushColor();
				if (ImGui::ColorEdit4("Brush Color", (float*)&brush_color))
					progress_bar_ui->SetFillTextureBrushColor(brush_color);

				Vec2 coordinate = progress_bar_ui->GetFillTextureCoordinate();
				if (ImGui::DragFloat2("Offset", (float*)&coordinate, 0.01f, -FLT_MAX, FLT_MAX, "%.4f"))
					progress_bar_ui->SetFillTextureCoordinate(coordinate);

				Vec2 tilling = progress_bar_ui->GetFillTextureTilling();
				if (ImGui::DragFloat2("Tilling", (float*)&tilling, 0.01f, 0.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
					progress_bar_ui->SetFillTextureTilling(tilling);

				ImGui::TreePop();
				ImGui::Separator();
			}
		};

		RegisterSettingHeaderNode("Progress bar UI", {
			{ "progress bar", ProgressBarUIFunc },
			});

		static auto TextBoxUIFunc = [](const SPtr<UILevel>& level)
		{
			const auto& text_box_ui = level->GetUILayer()->GetTextBox();

			Vec2 pos = text_box_ui->GetPosition();
			if (ImGui::DragFloat2("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
				text_box_ui->SetPosition(pos);

			Vec2 size = text_box_ui->GetSize();
			if (ImGui::DragFloat2("Size", (float*)&size, 0.5f, 1.0f, 4096.0f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				text_box_ui->SetSize(size);

			Vec2 pivot = text_box_ui->GetPivot();
			if (ImGui::DragFloat2("Pivot", (float*)&pivot, 0.01f, 0.0f, 1.0f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
				text_box_ui->SetPivot(pivot);

			std::string text = text_box_ui->GetText();
			if (ImGui::InputText("Text", &text))
				text_box_ui->SetText(text);

			Vec4 text_color = text_box_ui->GetColor();
			if (ImGui::ColorEdit4("Text color", (float*)&text_color))
				text_box_ui->SetColor(text_color);

			std::string font_name = utf8_encode(text_box_ui->GetFontName());
			if (ImGui::InputText("Font name", &font_name))
				text_box_ui->SetFontName(utf8_decode(font_name));

			int font_size = text_box_ui->GetFontSize();
			if (ImGui::DragInt("Font size", &font_size, 1.0f, 1, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp))
				text_box_ui->SetFontSize(font_size);

			auto TextUIComboSetting = [text_box_ui]
			(const auto& infos, const auto& search_info, const char* combo_name, int& select_index, std::function<void(const SPtr<TextBoxUI>&)> func)
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
							func(text_box_ui);
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

			TextUIComboSetting(weights, text_box_ui->GetFontWeight(), "Weights", select_weight_index,
				[](const SPtr<TextBoxUI>& text_box_ui) { text_box_ui->SetFontWeight(weights[select_weight_index].second); });

			static int select_style_index = 0;
			static const std::array<std::pair<std::string, DWRITE_FONT_STYLE>, 3> styles = {
				std::pair{"Normal", DWRITE_FONT_STYLE_NORMAL},
				{"Oblique", DWRITE_FONT_STYLE_OBLIQUE},
				{"Italic", DWRITE_FONT_STYLE_ITALIC}
			};

			TextUIComboSetting(styles, text_box_ui->GetFontStyle(), "Style", select_style_index,
				[](const SPtr<TextBoxUI>& text_box_ui) { text_box_ui->SetFontSytle(styles[select_style_index].second); });

			static int select_text_alignment_index = 0;
			static const std::array<std::pair<std::string, DWRITE_TEXT_ALIGNMENT>, 4> text_alignments = {
				std::pair{"Alignment leading", DWRITE_TEXT_ALIGNMENT_LEADING},
				{"Alignment trailing", DWRITE_TEXT_ALIGNMENT_TRAILING},
				{"Alignment center", DWRITE_TEXT_ALIGNMENT_CENTER},
				{"Alignment justified", DWRITE_TEXT_ALIGNMENT_JUSTIFIED}
			};

			TextUIComboSetting(text_alignments, text_box_ui->GetTextAlignment(), "Text alignment", select_text_alignment_index,
				[](const SPtr<TextBoxUI>& text_box_ui) { text_box_ui->SetTextAlignment(text_alignments[select_text_alignment_index].second); });

			static int select_paragraph_alignment_index = 0;
			static const std::array<std::pair<std::string, DWRITE_PARAGRAPH_ALIGNMENT>, 3> paragraph_alignments = {
				std::pair{"Alignment near", DWRITE_PARAGRAPH_ALIGNMENT_NEAR},
				{"Alignment far", DWRITE_PARAGRAPH_ALIGNMENT_FAR},
				{"Alignment center", DWRITE_PARAGRAPH_ALIGNMENT_CENTER}
			};

			TextUIComboSetting(paragraph_alignments, text_box_ui->GetParagraphAlignment(), "Paragraph alignment", select_paragraph_alignment_index,
				[](const SPtr<TextBoxUI>& text_box_ui) { text_box_ui->SetParagraphAlignment(paragraph_alignments[select_paragraph_alignment_index].second); });

			if (ImGui::TreeNodeEx("Background texture", ImGuiTreeNodeFlags_DefaultOpen))
			{
				const auto& texture = text_box_ui->GetBackgroundTexture();
				std::string path;
				if (texture != nullptr)
					path = texture->GetPath();

				ImGui::BulletText("Texture path : ");
				ImGui::SameLine();
				ImGui::Text(path.c_str());

				if (ImGui::Button("Open file dialog"))
				{
					path = FileDialog::OpenFile(L"texture file (.png .dds .bmp .jpg)\0*.png;*.dds;*.bmp;*.jpg");
					if (path.empty() == false)
						text_box_ui->SetBackgroundTexture(path);
				}

				Vec4 brush_color = text_box_ui->GetBackgroundTextureBrushColor();
				if (ImGui::ColorEdit4("Brush Color", (float*)&brush_color))
					text_box_ui->SetBackgroundTextureBrushColor(brush_color);

				Vec2 coordinate = text_box_ui->GetBackgroundTextureCoordinate();
				if (ImGui::DragFloat2("Offset", (float*)&coordinate, 0.01f, -FLT_MAX, FLT_MAX, "%.4f"))
					text_box_ui->SetBackgroundTextureCoordinate(coordinate);

				Vec2 tilling = text_box_ui->GetBackgroundTextureTilling();
				if (ImGui::DragFloat2("Tilling", (float*)&tilling, 0.01f, 0.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
					text_box_ui->SetBackgroundTextureTilling(tilling);

				ImGui::TreePop();
				ImGui::Separator();
			}
		};

		RegisterSettingHeaderNode("Text Box UI", {
			{ "text box", TextBoxUIFunc },
			});
	}
}
