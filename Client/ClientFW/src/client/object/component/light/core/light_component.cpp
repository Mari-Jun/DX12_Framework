#include "stdafx.h"
#include "client/object/component/light/core/light_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	LightComponent::LightComponent(eLightType type,
		const std::string& name, const std::string& draw_shader_name)
		: RenderComponent(name, 10, eRenderType::kLight, draw_shader_name)
		, m_light_type(type)
	{
	}

	bool LightComponent::Initialize()
	{
		bool ret = RenderComponent::Initialize();

		UpdateShadowTextureSize();

		return ret;
	}

	void LightComponent::RegisterToVisualOctree()
	{
	}

	void LightComponent::UnregisterFromVisualOctree()
	{
	}

	void LightComponent::SetLightColor(const Vec3& color)
	{
		m_light_color = Vec3(min(color.x, 1.f), min(color.y, 1.f), min(color.z, 1.f));
	}

	SPtr<LightComponent> LightComponent::SharedFromThis()
	{
		return std::static_pointer_cast<LightComponent>(shared_from_this());
	}
}
