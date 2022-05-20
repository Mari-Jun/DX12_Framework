#include <include/client_core.h>

#include "object/actor/rotating_cube.h"

#include "object/level/rotating_cube_level.h"
#include "object/levelsetting/rotating_cube_level_node_manager.h"

namespace simulation
{
	RotatingCubeLevelInitNodeManager::RotatingCubeLevelInitNodeManager()
	{
		static auto transform_func = [](const SPtr<RotatingCubeLevel>& level)
		{
			static Vec3 pos = Vec3(0.f, 0.f, 500.f);
			ImGui::DragFloat3("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f");
			level->SetInitPos(pos);

			static Vec3 scale = Vec3(1.f, 1.f, 1.f);
			ImGui::DragFloat3("Scale", (float*)&scale, 0.05f, 0.0f, FLT_MAX, "%.2f");
			level->SetInitScale(scale);
		};

		static auto shadow_func = [](const SPtr<RotatingCubeLevel>& level)
		{
			static bool enable_shadow = false;

			ImGui::Checkbox("enable shadow", &enable_shadow);
			level->EnableDirectionalLightShadow(enable_shadow);
		};

		RegisterSettingHeaderNode("Rotating Cube", { { "transform", transform_func } });
		RegisterSettingHeaderNode("Directional Light", { { "shadow", shadow_func } });
	}

	RotatingCubeLevelRuntimeNodeManager::RotatingCubeLevelRuntimeNodeManager()
	{
		static auto transform_func = [](const SPtr<RotatingCubeLevel>& level)
		{
			const auto& cube = level->GetRotatingCube();

			Vec3 pos = cube->GetPosition();
			if (ImGui::DragFloat3("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
			{
				cube->SetPosition(pos);
			}

			Vec3 scale = cube->GetScale();
			if (ImGui::DragFloat3("Scale", (float*)&scale, 0.05f, 0.0f, FLT_MAX, "%.2f"))
			{
				cube->SetScale(scale);
			}
		};

		RegisterSettingHeaderNode("Rotating Cube", { { "transform", transform_func } });
	}
}