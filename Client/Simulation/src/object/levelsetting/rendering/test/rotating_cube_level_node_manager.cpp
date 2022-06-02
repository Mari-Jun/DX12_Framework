#include <include/client_core.h>

#include <client/object/actor/light.h>
#include <client/object/component/util/rotating_movement_component.h>

#include "object/actor/rotating_cube.h"

#include "object/level/rendering/test/rotating_cube_level.h"
#include "object/levelsetting/rendering/test/rotating_cube_level_node_manager.h"

namespace simulation
{
	RotatingCubeLevelInitNodeManager::RotatingCubeLevelInitNodeManager()
	{
		static auto TransformFunc = [this](const SPtr<RotatingCubeLevel>& level)
		{
			ImGui::DragFloat3("Position", (float*)&m_init_pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f");
			ImGui::DragFloat3("Scale", (float*)&m_init_scale, 0.05f, 0.0f, FLT_MAX, "%.2f");
		};

		RegisterSettingHeaderNode("Rotating Cube", { { "transform", TransformFunc } });

		static auto ShadowFunc = [this](const SPtr<RotatingCubeLevel>& level)
		{
			ImGui::Checkbox("enable shadow", &m_enable_dir_light_shadow);
		};

		RegisterSettingHeaderNode("Directional Light", { { "shadow", ShadowFunc } });
	}

	RotatingCubeLevelRuntimeNodeManager::RotatingCubeLevelRuntimeNodeManager()
	{
		static auto CubeTransformFunc = [](const SPtr<RotatingCubeLevel>& level)
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

		static auto RotatingCompFunc = [](const SPtr<RotatingCubeLevel>& level)
		{
			const auto& rot_comp = level->GetRotatingCube()->GetRotatingComponent();

			Vec3 rotating_rate = rot_comp->GetRotatingRate();
			if (ImGui::DragFloat3("rotating rate", (float*)&rotating_rate, 0.5f, -FLT_MAX, FLT_MAX, "%.3f"))
			{
				rot_comp->SetRotatingRate(rotating_rate);
			}
		};

		RegisterSettingHeaderNode("Rotating cube", { 
			{ "transform", CubeTransformFunc },
			{ "rotating component", RotatingCompFunc }
			});

		static auto LightTransformFunc = [](const SPtr<RotatingCubeLevel>& level)
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

		static auto LightInfoFunc = [](const SPtr<RotatingCubeLevel>& level)
		{
			const auto& directional_light = level->GetDirectionalLight();

			Vec3 color = directional_light->GetLightColor();
			if (ImGui::ColorEdit3("Color", (float*)&color))
			{
				directional_light->SetLightColor(color);
			}

			float intensity = directional_light->GetLightIntensity();
			if (ImGui::DragFloat("Intensity", &intensity, 0.05f, 0.0f, 1000.0f, "%.4f"))
			{
				directional_light->SetLightIntensity(intensity);
			}
		};

		static auto ShadowVisibilityFunc = [](const SPtr<RotatingCubeLevel>& level)
		{
			const auto& directional_light = level->GetDirectionalLight();

			bool visible = directional_light->GetShadowVisibility();

			if (directional_light->IsUseShadow() == false)
				ImGui::BeginDisabled(true);
		
			if (ImGui::Checkbox("shadow visibility", &visible))
			{
				//현재 Visibility에 오류가 존재함
				directional_light->SetShadowVisibility(visible);
			}

			if (directional_light->IsUseShadow() == false)
				ImGui::EndDisabled();
		};

		RegisterSettingHeaderNode("Directional light", {
			{ "transform", LightTransformFunc },
			{ "light info", LightInfoFunc },
			{ "shadow visibility", ShadowVisibilityFunc },
			});
	}
}