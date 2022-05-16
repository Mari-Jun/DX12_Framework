#include "stdafx.h"
#include "client/object/actor/light.h"


namespace client_fw
{
	DirectionalLight::DirectionalLight(eMobilityState mobility, const std::string& name)
		: Light(mobility, name)
	{
	}

	PointLight::PointLight(eMobilityState mobility, const std::string& name)
		: Light(mobility, name)
	{
	}

	float PointLight::GetAttenuationRadius() const
	{
		return m_light_component->GetAttenuationRadius();
	}

	void PointLight::SetAttenuationRadius(float radius)
	{
		m_light_component->SetAttenuationRadius(radius);
	}

	SpotLight::SpotLight(eMobilityState mobility, const std::string& name)
		: Light(mobility, name)
	{
	}

	float SpotLight::GetAttenuationRadius() const
	{
		return m_light_component->GetAttenuationRadius();
	}

	void SpotLight::SetAttenuationRadius(float radius)
	{
		m_light_component->SetAttenuationRadius(radius);
	}

	float SpotLight::GetConeInnerAngle() const
	{
		return m_light_component->GetConeInnerAngle();
	}

	void SpotLight::SetConeInnerAngle(float degree)
	{
		m_light_component->SetConeInnerAngle(degree);
	}

	float SpotLight::GetConeOuterAngle() const
	{
		return m_light_component->GetConeOuterAngle();
	}

	void SpotLight::SetConeOuterAngle(float degree)
	{
		m_light_component->SetConeOuterAngle(degree);
	}
}