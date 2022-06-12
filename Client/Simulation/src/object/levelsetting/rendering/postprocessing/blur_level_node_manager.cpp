#include <include/client_core.h>
#include <include/dx12_shader_fw.h>

#include <client/asset/texture/texture.h>
#include <client/object/actor/pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/object/level/gamemode/game_mode_base.h>

#include "object/actor/camera_actor.h"
#include "object/level/rendering/postprocessing/blur_level.h"
#include "object/levelsetting/rendering/postprocessing/blur_level_node_manager.h"

namespace simulation
{
	BlurLevelInitNodeManager::BlurLevelInitNodeManager()
	{
	}

	BlurLevelRuntimeNodeManager::BlurLevelRuntimeNodeManager()
	{
		static auto PossessedPawnCameraBlurFunc = [](const SPtr<BlurLevel>& level)
		{
			const auto& pawn = std::dynamic_pointer_cast<CameraPawn>(level->GetGameMode()->GetPlayerController()->GetControlledPawn());

			if (pawn != nullptr)
			{
				const auto& camera = pawn->GetRenderCameraComponent();

				ImGui::Checkbox("Use blur", &camera->GetPostProcessingInfo()->use_blur);
				ImGui::DragFloat("Gaussian sigma", &camera->GetPostProcessingInfo()->blur_sigma, 0.01f, 0.01f, 10.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGuiHelper::ShowInformationSameLine({ u8"가우스 함수의 Sigma 값을 설정합니다. (0.01 ~ 10.0)", u8"exp(-x^2 / (2 * sigma^2)" });
				ImGui::DragInt("Blur count", &camera->GetPostProcessingInfo()->blur_count, 1.0f, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
				ImGuiHelper::ShowInformationSameLine({ u8"Blurring을 하는 횟수입니다. (1 ~ 100)" });
			}
		};

		RegisterSettingHeaderNode("Possessed pawn camera", {
			{ "blur", PossessedPawnCameraBlurFunc },
			});

		static auto UnpossessdPawnSelectFunc = [](const SPtr<BlurLevel>& level)
		{
			ImGuiHelper::ShowInformationSameLine({ u8"Level상에 Spawn되어 있는 Pawn이 보는 Scene을 렌더링합니다." });

			const auto& player_controller = level->GetGameMode()->GetPlayerController();
			const auto& pawns = level->GetSpawnedPawns();

			for (const auto& pawn : pawns)
			{
				if (ImGui::Button(pawn->GetName().c_str()))
				{
					pawn->Possess(player_controller);
				}
			}
		};

		RegisterSettingHeaderNode("Unpossessed pawns", { { "select pawn", UnpossessdPawnSelectFunc } });
	}
}
