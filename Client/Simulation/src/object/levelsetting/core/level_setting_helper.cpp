#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/object/actor/core/actor.h>
#include <client/object/actor/light.h>

#include "object/levelsetting/core/level_setting_manager.h"
#include "object/levelsetting/core/level_setting_helper.h"

namespace simulation
{
	void LevelSettingHelper::ActorPositionFunc(const SPtr<Actor>& actor)
	{
		Vec3 pos = actor->GetPosition();
		if (ImGui::DragFloat3("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
		{
			actor->SetPosition(pos);
		}
	}

	void LevelSettingHelper::ActorRotationFunc(const SPtr<Actor>& actor)
	{
		Vec3 rot = quat::QuaternionToEuler(actor->GetRotation());
		rot.x = math::ToDegrees(rot.x);
		rot.y = math::ToDegrees(rot.y);
		rot.z = math::ToDegrees(rot.z);
		if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.5f, -180.0f, 180.0f, "%.4f"))
		{
			actor->SetRotation(quat::CreateQuaternionFromRollPitchYaw(
				math::ToRadian(rot.x), math::ToRadian(rot.y), math::ToRadian(rot.z)));
		}
	}

	void LevelSettingHelper::ActorScaleFunc(const SPtr<Actor>& actor)
	{
		Vec3 scale = actor->GetScale();
		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.05f, 0.0f, FLT_MAX, "%.2f"))
		{
			actor->SetScale(scale);
		}
	}

	void LevelSettingHelper::DirectionalLightFunc(const SPtr<DirectionalLight>& light)
	{
		LevelSettingHelper::ActorRotationFunc(light);

		Vec3 color = light->GetLightColor();
		if (ImGui::ColorEdit3("Color", (float*)&color))
		{
			light->SetLightColor(color);
		}

		float intensity = light->GetLightIntensity();
		if (ImGui::DragFloat("Intensity", &intensity, 0.05f, 0.0f, 1000.0f, "%.4f"))
		{
			light->SetLightIntensity(intensity);
		}

		bool visible = light->GetShadowVisibility();
		if (ImGui::Checkbox("shadow visibility", &visible))
		{
			light->SetShadowVisibility(visible);
		}
	}

	void LevelSettingHelper::PointLightFunc(const SPtr<PointLight>& light)
	{
		LevelSettingHelper::ActorPositionFunc(light);

		Vec3 color = light->GetLightColor();
		if (ImGui::ColorEdit3("Color", (float*)&color))
		{
			light->SetLightColor(color);
		}

		float intensity = light->GetLightIntensity();
		if (ImGui::DragFloat("Intensity", &intensity, 0.05f, 0.0f, 10'000'000.f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
		{
			light->SetLightIntensity(intensity);
		}

		bool visible = light->GetShadowVisibility();
		if (ImGui::Checkbox("shadow visibility", &visible))
		{
			light->SetShadowVisibility(visible);
		}

		float attenuation_radius = light->GetAttenuationRadius();
		if (ImGui::DragFloat("attenuation radius", &attenuation_radius, 1.f, 1.f, 4096.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
		{
			light->SetAttenuationRadius(attenuation_radius);
		}
	}

	void LevelSettingHelper::SpotLightFunc(const SPtr<SpotLight>& light)
	{
		LevelSettingHelper::ActorPositionFunc(light);
		LevelSettingHelper::ActorRotationFunc(light);

		Vec3 color = light->GetLightColor();
		if (ImGui::ColorEdit3("Color", (float*)&color))
		{
			light->SetLightColor(color);
		}

		float intensity = light->GetLightIntensity();
		if (ImGui::DragFloat("Intensity", &intensity, 0.05f, 0.0f, 10'000'000.f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
		{
			light->SetLightIntensity(intensity);
		}

		bool visible = light->GetShadowVisibility();
		if (ImGui::Checkbox("shadow visibility", &visible))
		{
			light->SetShadowVisibility(visible);
		}

		float attenuation_radius = light->GetAttenuationRadius();
		if (ImGui::DragFloat("attenuation radius", &attenuation_radius, 1.f, 1.f, 4096.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
		{
			light->SetAttenuationRadius(attenuation_radius);
		}

		float inner_angle = light->GetConeInnerAngle();
		inner_angle = math::ToDegrees(inner_angle);
		if (ImGui::DragFloat("inner angle", &inner_angle, 0.1f, 0.f, 75.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
		{
			light->SetConeInnerAngle(inner_angle);
		}

		float outer_angle = light->GetConeOuterAngle();
		outer_angle = math::ToDegrees(outer_angle);
		if (ImGui::DragFloat("outer angle", &outer_angle, 0.1f, 0.f, 75.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
		{
			light->SetConeOuterAngle(outer_angle);
		}
	}
}