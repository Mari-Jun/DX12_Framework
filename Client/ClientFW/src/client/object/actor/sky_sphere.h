#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	class SkySphereComponent;

	class SkySphere : public Actor
	{
	public:
		SkySphere(const std::string& name = "sky sphere");
		virtual ~SkySphere() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	protected:
		SPtr<SkySphereComponent> m_sky_sphere_component;

	public:
		void SetCenterColor(const Vec3& color);
		void SetApexColor(const Vec3& color);
	};


}

