#include <include/client_core.h>

#include <client/object/actor/pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/object/level/gamemode/game_mode_base.h>

#include "object/actor/camera_actor.h"
#include "object/level/component/player/possess_pawn_level.h"
#include "object/levelsetting/component/player/possess_pawn_level_node_manager.h"

namespace simulation
{
	PossessPawnLevelInitNodeManager::PossessPawnLevelInitNodeManager()
	{
	}

	PossessPawnLevelRuntimeNodeManager::PossessPawnLevelRuntimeNodeManager()
	{
		static auto PossessedPawnTransformFunc = [](const SPtr<PossessPawnLevel>& level)
		{
			const auto& pawn = level->GetGameMode()->GetPlayerController()->GetControlledPawn();

			if (pawn != nullptr)
			{
				ImGui::BeginDisabled();

				Vec3 pos = pawn->GetPosition();
				ImGui::DragFloat3("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f");

				Vec3 rot = quat::QuaternionToEuler(pawn->GetRotation());
				rot.x = math::ToDegrees(rot.x);
				rot.y = math::ToDegrees(rot.y);
				rot.z = math::ToDegrees(rot.z);
				ImGui::DragFloat3("Rotation", (float*)&rot, 0.5f, -180.0f, 180.0f, "%.4f");

				ImGui::EndDisabled();
			}
		};

		static auto PossessedPawnCameraOffsetFunc = [](const SPtr<PossessPawnLevel>& level)
		{
			const auto& pawn = std::dynamic_pointer_cast<CameraPawn>(level->GetGameMode()->GetPlayerController()->GetControlledPawn());

			if (pawn != nullptr)
			{
				const auto& camera = pawn->GetRenderCameraComponent();

				Vec3 pos = camera->GetLocalPosition();
				if (ImGui::DragFloat3("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
					camera->SetLocalPosition(pos);

				Vec3 rot = quat::QuaternionToEuler(camera->GetLocalRotation());
				rot.x = math::ToDegrees(rot.x);
				rot.y = math::ToDegrees(rot.y);
				rot.z = math::ToDegrees(rot.z);
				if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.5f, -180.0f, 180.0f, "%.4f"))
				{
					camera->SetLocalRotation(quat::CreateQuaternionFromRollPitchYaw(
						math::ToRadian(rot.x), math::ToRadian(rot.y), math::ToRadian(rot.z)));
				}
			}
		};

		RegisterSettingHeaderNode("Possessed pawn", {
			{ "transform", PossessedPawnTransformFunc },
			{ "camera transform", PossessedPawnCameraOffsetFunc },
			});

		static auto UnpossessdPawnSelectFunc = [](const SPtr<PossessPawnLevel>& level)
		{
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
