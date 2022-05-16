#include "stdafx.h"
#include "client/object/actor/sky_sphere.h"
#include "client/object/component/sky/sky_sphere_component.h"

namespace client_fw
{
	SkySphere::SkySphere(const std::string& name)
		: Actor(eMobilityState::kStatic, name)
	{
		m_sky_sphere_component = CreateSPtr<SkySphereComponent>();
	}

	bool SkySphere::Initialize()
	{
		bool ret = true;
		ret &= AttachComponent(m_sky_sphere_component);
		return ret;
	}

	void SkySphere::Shutdown()
	{
		m_sky_sphere_component = nullptr;
	}

	void SkySphere::SetCenterColor(const Vec3& color)
	{
		m_sky_sphere_component->SetCenterColor(color);
	}

	void SkySphere::SetApexColor(const Vec3& color)
	{
		m_sky_sphere_component->SetApexColor(color);
	}
}
