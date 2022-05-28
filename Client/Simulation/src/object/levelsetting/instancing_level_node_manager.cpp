#include <include/client_core.h>

#include "object/level/instancing_level.h"
#include "object/levelsetting/instancing_level_node_manager.h"

namespace simulation
{
	InstancingLevelInitNodeManager::InstancingLevelInitNodeManager()
	{
		static auto SpawnFunc = [](const SPtr<InstancingLevel>& level)
		{
			static UINT num_of_x_actors = 10;
			static UINT num_of_y_actors = 10;
			static UINT num_of_z_actors = 10;

			if (ImGui::DragInt("num of x", (int*)&num_of_x_actors, 1.0f, 1, 50, "%d", ImGuiSliderFlags_AlwaysClamp))
				level->SetNumOfXActors(num_of_x_actors);
			if (ImGui::DragInt("num of y", (int*)&num_of_y_actors, 1.0f, 1, 50, "%d", ImGuiSliderFlags_AlwaysClamp))
				level->SetNumOfYActors(num_of_y_actors);
			if (ImGui::DragInt("num of z", (int*)&num_of_z_actors, 1.0f, 1, 50, "%d", ImGuiSliderFlags_AlwaysClamp))
				level->SetNumOfZActors(num_of_z_actors);
		};

		RegisterSettingHeaderNode("Spawn Actor", { {"num of actors", SpawnFunc} });
	}

	InstancingLevelRuntimeNodeManager::InstancingLevelRuntimeNodeManager()
	{
	}
}
