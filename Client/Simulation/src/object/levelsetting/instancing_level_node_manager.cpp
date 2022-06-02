#include <include/client_core.h>

#include "object/level/instancing_level.h"
#include "object/levelsetting/instancing_level_node_manager.h"

namespace simulation
{
	InstancingLevelInitNodeManager::InstancingLevelInitNodeManager()
	{
		static auto SpawnFunc = [this](const SPtr<InstancingLevel>& level)
		{
			ImGui::DragInt("num of x", (int*)&m_num_of_x_actors, 1.0f, 1, 50, "%d", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragInt("num of y", (int*)&m_num_of_y_actors, 1.0f, 1, 50, "%d", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragInt("num of z", (int*)&m_num_of_z_actors, 1.0f, 1, 50, "%d", ImGuiSliderFlags_AlwaysClamp);
		};

		RegisterSettingHeaderNode("Spawn Actor", { {"num of actors", SpawnFunc} });
	}

	InstancingLevelRuntimeNodeManager::InstancingLevelRuntimeNodeManager()
	{
	}
}
