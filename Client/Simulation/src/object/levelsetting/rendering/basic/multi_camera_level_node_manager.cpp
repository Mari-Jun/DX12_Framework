#include <include/client_core.h>
#include <include/dx12_shader_fw.h>

#include <client/asset/texture/texture.h>
#include <client/object/component/util/render_camera_component.h>

#include "object/actor/camera_actor.h"
#include "object/level/rendering/basic/multi_camera_level.h"
#include "object/levelsetting/rendering/basic/multi_camera_level_node_manager.h"

namespace simulation
{
	MultiCameraLevelInitNodeManager::MultiCameraLevelInitNodeManager()
	{
		static auto CameraInitFunc = [this](const SPtr<MultiCameraLevel>& level)
		{
			ImGui::DragInt("num of camera", (int*)&m_num_of_camera, 1.0f, 1, 120, "%d", ImGuiSliderFlags_AlwaysClamp);
			ImGuiHelper::ShowInformationSameLine({ u8"레벨에 스폰될 카메라의 수를 설정합니다. (1 ~ 120)" });

			ImGui::DragFloat("camera offset", (float*)&m_camera_offset, 1.0f, 500.0f, FLT_MAX, "%.4f", ImGuiSliderFlags_AlwaysClamp);
			ImGuiHelper::ShowInformationSameLine({ u8"플레이어 카메라와의 거리를 설정합니다. (최소 500)" });
		};

		RegisterSettingHeaderNode("Camera Actors", { { "init camera", CameraInitFunc } });
	}

	MultiCameraLevelRuntimeNodeManager::MultiCameraLevelRuntimeNodeManager()
	{
		static auto CameraInfoFunc = [this](const SPtr<MultiCameraLevel>& level)
		{
			ImVec4 window_rect = ImGuiHelper::GetImGuiWindowBoxSize();

			const auto& camera_actors = level->GetCameraActors();

			for (const auto& camera : camera_actors)
			{
				if (ImGui::TreeNode(camera->GetName().c_str()))
				{
					ImGui::BulletText("transform");

					Vec3 pos = camera->GetPosition();
					if (ImGui::DragFloat3("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
					{
						camera->SetPosition(pos);
					}

					Vec3 rot = quat::QuaternionToEuler(camera->GetRotation());
					rot.x = math::ToDegrees(rot.x);
					rot.y = math::ToDegrees(rot.y);
					rot.z = math::ToDegrees(rot.z);
					if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.5f, -180.0f, 180.0f, "%.4f"))
					{
						camera->SetRotation(quat::CreateQuaternionFromRollPitchYaw(
							math::ToRadian(rot.x), math::ToRadian(rot.y), math::ToRadian(rot.z)));
					}

					ImGui::Separator();
					ImGui::BulletText("render texture");

					const auto& render_camera = camera->GetRenderCameraComponent();
					ImGui::Image((ImTextureID)render_camera->GetRenderTexture()->GetGPUAddress(), ImVec2(window_rect.z, window_rect.z / 4 * 3));
					ImGui::TreePop();
				}
			}
		};

		RegisterSettingHeaderNode("Camera Actors", { { "camera info", CameraInfoFunc } });
	}
}