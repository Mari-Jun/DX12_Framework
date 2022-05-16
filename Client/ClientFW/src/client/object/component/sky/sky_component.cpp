#include "stdafx.h"
#include "client/object/component/sky/sky_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	SkyComponent::SkyComponent(eSkyType type, 
		const std::string& draw_shader_name, const std::string& name)
		: Component(name, 1000), m_sky_type(type)
		, m_draw_shader_name(draw_shader_name)
	{
	}

	bool SkyComponent::InitializeComponent()
	{
		bool ret = Component::InitializeComponent();
		ret &= Render::RegisterSkyComponent(shared_from_this(), m_draw_shader_name);
		return ret;
	}

	void SkyComponent::ShutdownComponent()
	{
		Render::UnregisterSkyComponent(shared_from_this(), m_draw_shader_name);
		Component::ShutdownComponent();
	}
}
