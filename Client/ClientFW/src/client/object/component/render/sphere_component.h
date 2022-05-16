#pragma once
#include "client/object/component/render/shape_component.h"

namespace client_fw
{
	class SphereComponent : public ShapeComponent
	{
	public:
		SphereComponent(float radius, const std::string& name = "sphere component");
		virtual ~SphereComponent() = default;

	public:
		void SetExtents(float radius) { m_extents = Vec3(radius, radius, radius); }

	protected:
		SPtr<SphereComponent> SharedFromThis();
	};
}


