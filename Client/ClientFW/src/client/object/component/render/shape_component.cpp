#include "stdafx.h"
#include "client/object/component/render/shape_component.h"

namespace client_fw
{
	ShapeComponent::ShapeComponent(const std::string& name, const Vec3& extents,
		const std::string& draw_shader_name)
		: RenderComponent(name, 50, eRenderType::kShape, draw_shader_name)
		, m_extents(extents)
	{
		m_hidden_in_game = true;
		m_is_register_render_system = false;
	}

	void ShapeComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(BOrientedBox(vec3::ZERO, m_extents), GetWorldMatrix());
	}

}
