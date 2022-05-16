#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	class ShapeComponent : public RenderComponent
	{
	protected:
		ShapeComponent(const std::string& name, const Vec3& extents, 
			const std::string& draw_shader_name);
		virtual ~ShapeComponent() = default;

	protected:
		virtual void UpdateOrientedBox() override;

	protected:
		Vec3 m_extents;

	public:
		const Vec3& GetExtents() const { return m_extents; }
	};
}


