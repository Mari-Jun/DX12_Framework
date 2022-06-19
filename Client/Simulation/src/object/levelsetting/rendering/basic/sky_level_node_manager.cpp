#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/asset/texture/texture.h>
#include <client/object/actor/sky_cube.h>
#include <client/object/component/sky/sky_cube_component.h>
#include <client/util/file_dialog.h>

#include "object/level/rendering/basic/sky_level.h"
#include "object/levelsetting//rendering/basic/sky_level_node_manager.h"

namespace simulation
{
	SkyCubeLevelInitNodeManager::SkyCubeLevelInitNodeManager()
	{
	}

	SkyCubeLevelRuntimeNodeManager::SkyCubeLevelRuntimeNodeManager()
	{
		static auto SkyCubeFunc = [](const SPtr<SkyCubeLevel>& level)
		{
			const auto& sky_cube_component = level->GetSkyCube()->GetSkyCubeComponent();

			const auto& texture = sky_cube_component->GetCubeMapTexture();
			std::string path;
			if (texture != nullptr)
				path = texture->GetPath();

			ImGui::BulletText("Texture path : ");
			ImGui::SameLine();
			ImGui::Text(path.c_str());

			if (ImGui::Button("Open file dialog"))
			{
				path = FileDialog::OpenFile(L"texture file (.dds)\0*.dds");
				if (path.empty() == false)
				{
					sky_cube_component->SetCubeMapTexture(path);
				}
			}
		};

		RegisterSettingHeaderNode("Sky cube", {
			{ "sky cube", SkyCubeFunc },
			});
	}
}