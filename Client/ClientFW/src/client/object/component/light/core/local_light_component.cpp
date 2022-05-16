#include "stdafx.h"
#include "client/object/component/light/core/local_light_component.h"

namespace client_fw
{
	LocalLightComponent::LocalLightComponent(eLightType type,
		const std::string& name, const std::string& draw_shader_name)
		: LightComponent(type, name, draw_shader_name)
	{
	}

	bool LocalLightComponent::Initialize()
	{
		bool ret = LightComponent::Initialize();

		UpdateShadowCameraProjection();

		return ret;
	}

	void LocalLightComponent::RegisterToVisualOctree()
	{
		RenderComponent::RegisterToVisualOctree();
	}

	void LocalLightComponent::UnregisterFromVisualOctree()
	{
		RenderComponent::UnregisterFromVisualOctree();
	}

	void LocalLightComponent::SetAttenuationRadius(float radius)
	{
		m_attenuation_radius = std::clamp(radius, 0.0f, s_max_attenuation_radius);
		UpdateShadowCameraProjection();
		m_update_local_matrix = true;
	}
}
