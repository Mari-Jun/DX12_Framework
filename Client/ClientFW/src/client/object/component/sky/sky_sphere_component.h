#pragma once
#include "client/object/component/sky/sky_component.h"

namespace client_fw
{
	class SkySphereComponent : public SkyComponent
	{
	public:
		SkySphereComponent(const std::string& name = "sky sphere component");
		virtual ~SkySphereComponent() = default;

	protected:
		Vec3 m_center_color;
		Vec3 m_apex_color;

	public:
		const Vec3& GetCenterColor() const { return m_center_color; }
		void SetCenterColor(const Vec3& color) { m_center_color = color; }
		const Vec3& GetApexColor() const { return m_apex_color; }
		void SetApexColor(const Vec3& color) { m_apex_color = color; }
	};
}



