#include <include/client_core.h>

#include "object/level/rotating_cube_level.h"

#include "object/levelsetting/initialize/rotating_cube_level_init_node_manager.h"

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
}