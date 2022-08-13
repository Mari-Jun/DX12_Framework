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

	MultiplePointLightLevelInitNodeManager::MultiplePointLightLevelInitNodeManager()
	{
		static auto PointLightsFunc = [this](const SPtr<MultiplePointLightLevel>& level)
		{
			ImGui::Checkbox("enable shadow", &m_use_shadow);
			ImGuiHelper::ShowInformationSameLine({ u8"Shadow를 사용하게 될 경우 소환 가능한 최대 Light수가 줄어듭니다." });

			ImGui::Checkbox("update intensity", &m_update_intensity);
			ImGuiHelper::ShowInformationSameLine({ u8"Update Intensity를 사용하게 될 경우 프레임이 떨어질 가능성이 높습니다." });

			const int max_point_lights = m_use_shadow ? 30 : 300;
			m_offset = 300.f;
			if (m_use_shadow)
			{
				m_offset = 500.f;
				m_num_of_point_lights = std::min(m_num_of_point_lights, 30);
			}
			ImGui::DragInt("num of point lights in a row", &m_num_of_point_lights, 1.0f, 1, max_point_lights, "%d", ImGuiSliderFlags_AlwaysClamp);
			ImGuiHelper::ShowInformationSameLine({ u8"한줄에 있는 Point light의 수. (즉 소환되는 light의 수는 제곱)" });
		};

		RegisterSettingHeaderNode("Point lights setting", {
			{ "lights setting", PointLightsFunc }
			});
	}

	MultiplePointLightLevelRuntimeNodeManager::MultiplePointLightLevelRuntimeNodeManager()
	{
		static auto PointLightsFunc = [this](const SPtr<MultiplePointLightLevel>& level)
		{
			const auto& point_lights = level->GetPointLights();

			ImGui::BulletText("num of point light : %d", point_lights.size());
		};

		RegisterSettingHeaderNode("Point lights", {
			   { "light count", PointLightsFunc }
			});
	}
}
