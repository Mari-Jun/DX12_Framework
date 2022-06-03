#include <include/client_core.h>
#include <include/dx12_shader_fw.h>

#include <client/asset/texture/texture.h>
#include <client/object/level/gamemode/game_mode_base.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/light.h>
#include <client/object/component/util/render_camera_component.h>

#include "object/level/rendering/basic/gbuffer_level.h"
#include "object/levelsetting/rendering/basic/gbuffer_level_node_manager.h"

namespace simulation
{
	GBufferLevelInitNodeManager::GBufferLevelInitNodeManager()
	{
	}

	GBufferLevelRuntimeNodeManager::GBufferLevelRuntimeNodeManager()
	{
		static auto LightTransformFunc = [](const SPtr<GBufferLevel>& level)
		{
			const auto& directional_light = level->GetDirectionalLight();

			Vec3 rot = quat::QuaternionToEuler(directional_light->GetRotation());
			rot.x = math::ToDegrees(rot.x);
			rot.y = math::ToDegrees(rot.y);
			rot.z = math::ToDegrees(rot.z);
			if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.5f, -180.0f, 180.0f, "%.4f"))
			{
				directional_light->SetRotation(quat::CreateQuaternionFromRollPitchYaw(
					math::ToRadian(rot.x), math::ToRadian(rot.y), math::ToRadian(rot.z)));
			}
		};

		RegisterSettingHeaderNode("Directional light", {
			{ "transform", LightTransformFunc },
			});

		static auto GBufferFunc = [](const SPtr<GBufferLevel>& level)
		{
			const auto& player_controller = level->GetGameMode()->GetPlayerController();
			if (player_controller != nullptr)
			{
				const auto& camera = player_controller->GetPlayerCamera();
				ImVec4 window_rect = ImGuiHelper::GetImGuiWindowBoxSize();
				Vec2 size = Vec2(window_rect.z, window_rect.z / camera->GetViewSize().x * camera->GetViewSize().y);

				if (camera->GetRenderTexture() != nullptr)
				{
					ImGui::BulletText("base color");
					ImGui::Image((ImTextureID)camera->GetRenderTexture()->GetGBufferGPUAddress(0), ImVec2(size.x, size.y));
					ImGui::BulletText("normal");
					ImGui::Image((ImTextureID)camera->GetRenderTexture()->GetGBufferGPUAddress(1), ImVec2(size.x, size.y));
					ImGui::BulletText("additional information");
					ImGui::Image((ImTextureID)camera->GetRenderTexture()->GetGBufferGPUAddress(2), ImVec2(size.x, size.y));
					ImGui::BulletText("depth");
					ImGui::Image((ImTextureID)camera->GetRenderTexture()->GetDSVGPUAddress(), ImVec2(size.x, size.y));
				}
			}
		};

		RegisterSettingHeaderNode("Render camera", {
		{ "g-buffer", GBufferFunc },
			});
	}
}
