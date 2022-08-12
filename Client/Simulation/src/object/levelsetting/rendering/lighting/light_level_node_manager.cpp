#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/object/actor/light.h>

#include "object/level/rendering/lighting/light_level.h"
#include "object/levelsetting/rendering/lighting/light_level_node_manager.h"
#include "object/levelsetting/core/level_setting_helper.h"

namespace simulation
{
	LightLevelInitNodeManager::LightLevelInitNodeManager()
	{
		static auto ShadowSizeFunc = [this](const SPtr<LightLevel>& level)
		{
			ImGui::DragInt("directional light shadow size", &m_direction_light_shadow_size, 1.0f, 256, 8192, "%d", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragInt("pointal light shadow size", &m_point_light_shadow_size, 1.0f, 32, 2048, "%d", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragInt("spotal light shadow size", &m_spot_light_shadow_size, 1.0f, 32, 4096, "%d", ImGuiSliderFlags_AlwaysClamp);
		};

		RegisterSettingHeaderNode("Shadow size", {
			{ "shadow size", ShadowSizeFunc }
			});
	}

	LightLevelRuntimeNodeManager::LightLevelRuntimeNodeManager()
	{
		static auto DirectionLightFunc = [](const SPtr<DirectionalLight>& light)
		{
			if (ImGui::TreeNode(light->GetName().c_str()))
			{
				LevelSettingHelper::DirectionalLightFunc(light);
				ImGui::TreePop();
			}
		};

		static auto PointLightFunc = [](const SPtr<PointLight>& light)
		{
			if (ImGui::TreeNode(light->GetName().c_str()))
			{
				LevelSettingHelper::PointLightFunc(light);
				ImGui::TreePop();
			}
		};

		static auto SpotLightFunc = [](const SPtr<SpotLight>& light)
		{
			if (ImGui::TreeNode(light->GetName().c_str()))
			{
				LevelSettingHelper::SpotLightFunc(light);
				ImGui::TreePop();
			}
		};

		static auto LightLevelDirectionalLightFunc = [](const SPtr<LightLevel>& level)
		{
			DirectionLightFunc(level->GetDirectionalLight(0));
			DirectionLightFunc(level->GetDirectionalLight(1));
		};

		static auto LightLevelPointLightFunc = [](const SPtr<LightLevel>& level)
		{
			PointLightFunc(level->GetPointLight(0));
			PointLightFunc(level->GetPointLight(1));
			PointLightFunc(level->GetPointLight(2));
		};

		static auto LightLevelSpotLightFunc = [](const SPtr<LightLevel>& level)
		{
			SpotLightFunc(level->GetSpotLight(0));
			SpotLightFunc(level->GetSpotLight(1));
			SpotLightFunc(level->GetSpotLight(2));
		};

		RegisterSettingHeaderNode("Lights", {
			{ "directional lights", LightLevelDirectionalLightFunc },
			{ "point lights", LightLevelPointLightFunc },
			{ "spot lights", LightLevelSpotLightFunc },
			});
	}
}
