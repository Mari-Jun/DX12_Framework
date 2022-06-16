#include <include/client_core.h>
#include <include/dx12_shader_fw.h>

#include <client/asset/texture/texture.h>
#include <client/object/component/render/texture_billboard_component.h>
#include <client/util/file_dialog.h>

#include "object/actor/billboard_actor.h"

#include "object/level/rendering/basic/billboard_level.h"
#include "object/levelsetting/rendering/basic/billboard_level_node_manager.h"

namespace simulation
{
	BillboardLevelInitNodeManager::BillboardLevelInitNodeManager()
	{
	}

	BillboardLevelRuntimeNodeManager::BillboardLevelRuntimeNodeManager()
	{
		static auto TextureBillboardFunc = [](const SPtr<TextureBillboardLevel>& level)
		{
			const auto& actor = level->GetTextureBillboardActor();
			if (ImGui::TreeNodeEx(actor->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::BulletText("Transform");

				Vec3 pos = actor->GetPosition();
				if (ImGui::DragFloat3("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
				{
					actor->SetPosition(pos);
				}

				const auto& texture_billboard_component = actor->GetTextureBillboardComponent();
				if (texture_billboard_component != nullptr)
				{
					Vec2 billboard_size = texture_billboard_component->GetSize();
					if (ImGui::DragFloat2("Size", (float*)&billboard_size, 1.0f, 0.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
						texture_billboard_component->SetSize(billboard_size);

					bool fix_up_vector = texture_billboard_component->IsFixUpVector();
					if (ImGui::Checkbox("Fix up vector", &fix_up_vector))
						texture_billboard_component->SetFixUpVector(fix_up_vector);

					Vec2 offset = texture_billboard_component->GetOffset();
					if (ImGui::DragFloat2("Offset", (float*)&offset, 0.1f, -FLT_MAX, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
						texture_billboard_component->SetOffset(offset);

					Vec2 tilling = texture_billboard_component->GetTilling();
					if (ImGui::DragFloat2("Tilling", (float*)&tilling, 0.1f, 0.0, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp))
						texture_billboard_component->SetTilling(tilling);

					const auto& texture = texture_billboard_component->GetTexture();
					std::string path;
					if (texture != nullptr)
						path = texture_billboard_component->GetTexture()->GetPath();

					ImGui::BulletText("Texture path : ");
					ImGui::SameLine();
					ImGui::Text(path.c_str());

					if (ImGui::Button("Open file dialog"))
					{
						path = FileDialog::OpenFile(L"texture file (.png .dds .bmp .jpg)\0*.png;*.dds;*.bmp;*.jpg");
						if (path.empty() == false)
						{
							texture_billboard_component->SetTexture(path);
						}
					}

					ImGui::TreePop();
					ImGui::Separator();
				}
			}
		};

		RegisterSettingHeaderNode("Static mesh actor", {
			{"texture billboard actor", TextureBillboardFunc},
			});
	}
}