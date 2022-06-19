#include <include/client_core.h>
#include <include/dx12_shader_fw.h>

#include <client/object/actor/pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/object/level/gamemode/game_mode_base.h>

#include "object/level/rendering/postprocessing/sobel_edge_level.h"
#include "object/levelsetting/rendering/postprocessing/sobel_edge_level_node_manager.h"

namespace simulation
{
	SobelEdgeLevelInitNodeManager::SobelEdgeLevelInitNodeManager()
	{
	}

	SobelEdgeLevelRuntimeNodeManager::SobelEdgeLevelRuntimeNodeManager()
	{
		static auto CameraSobelEdgeFunc = [](const SPtr<SobelEdgeLevel>& level)
		{
			const auto& player_controller = level->GetGameMode()->GetPlayerController();
			if (player_controller != nullptr)
			{
				const auto& camera = player_controller->GetPlayerCamera();

				ImGui::Checkbox("Use sobel edge", &camera->GetPostProcessingInfo()->use_sobel_edge);
			}
		};

		RegisterSettingHeaderNode("Possessed pawn camera", {
		{ "sobel edge", CameraSobelEdgeFunc },
			});
	}
}